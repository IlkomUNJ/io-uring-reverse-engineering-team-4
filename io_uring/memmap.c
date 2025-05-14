// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/io_uring.h>
#include <linux/io_uring_types.h>
#include <asm/shmparam.h>

#include "memmap.h"
#include "kbuf.h"
#include "rsrc.h"

/**
 * This function allocates a compound memory block of the specified size
 * and maps the allocated pages into the provided array. It calculates the
 * order of allocation based on the size and ensures the order does not
 * exceed the maximum allowed page order. If the allocation is successful,
 * the function returns the virtual address of the allocated memory block.
 * Otherwise, it returns an error pointer indicating the failure.
 */
static void *io_mem_alloc_compound(struct page **pages, int nr_pages,
				   size_t size, gfp_t gfp)
{
	struct page *page;
	int i, order;

	order = get_order(size);
	if (order > MAX_PAGE_ORDER)
		return ERR_PTR(-ENOMEM);
	else if (order)
		gfp |= __GFP_COMP;

	page = alloc_pages(gfp, order);
	if (!page)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < nr_pages; i++)
		pages[i] = page + i;

	return page_address(page);
}

/**
 * This function pins a range of user pages in memory, ensuring they remain
 * resident for the duration of an I/O operation. It calculates the number of
 * pages required to cover the specified memory range, allocates an array to
 * store the page pointers, and attempts to pin the pages using
 * `pin_user_pages_fast`.
 */
struct page **io_pin_pages(unsigned long uaddr, unsigned long len, int *npages)
{
	unsigned long start, end, nr_pages;
	struct page **pages;
	int ret;

	if (check_add_overflow(uaddr, len, &end))
		return ERR_PTR(-EOVERFLOW);
	if (check_add_overflow(end, PAGE_SIZE - 1, &end))
		return ERR_PTR(-EOVERFLOW);

	end = end >> PAGE_SHIFT;
	start = uaddr >> PAGE_SHIFT;
	nr_pages = end - start;
	if (WARN_ON_ONCE(!nr_pages))
		return ERR_PTR(-EINVAL);
	if (WARN_ON_ONCE(nr_pages > INT_MAX))
		return ERR_PTR(-EOVERFLOW);

	pages = kvmalloc_array(nr_pages, sizeof(struct page *), GFP_KERNEL);
	if (!pages)
		return ERR_PTR(-ENOMEM);

	ret = pin_user_pages_fast(uaddr, nr_pages, FOLL_WRITE | FOLL_LONGTERM,
					pages);
	/* success, mapped all pages */
	if (ret == nr_pages) {
		*npages = nr_pages;
		return pages;
	}

	/* partial map, or didn't map anything */
	if (ret >= 0) {
		/* if we did partial map, release any pages we did get */
		if (ret)
			unpin_user_pages(pages, ret);
		ret = -EFAULT;
	}
	kvfree(pages);
	return ERR_PTR(ret);
}

enum {
	/* memory was vmap'ed for the kernel, freeing the region vunmap's it */
	IO_REGION_F_VMAP			= 1,
	/* memory is provided by user and pinned by the kernel */
	IO_REGION_F_USER_PROVIDED		= 2,
	/* only the first page in the array is ref'ed */
	IO_REGION_F_SINGLE_REF			= 4,
};

/**
 * This function releases resources associated with a memory-mapped region.
 * It performs the following operations:
 * 
 * 1. If the region has associated pages:
 *    - Determines the number of references to the pages based on the flags.
 *    - If the pages are user-provided, unpins them using unpin_user_pages().
 *      Otherwise, releases them using release_pages().
 *    - Frees the memory allocated for the pages using kvfree().
 * 
 * 2. If the region is VMAP-mapped and has a valid pointer, unmaps it using vunmap().
 * 
 * 3. If the region has pages and is associated with a user, unaccounts the memory
 *    usage for the user using __io_unaccount_mem().
 * 
 * 4. Clears the memory-mapped region structure by zeroing it out with memset().
 */
void io_free_region(struct io_ring_ctx *ctx, struct io_mapped_region *mr)
{
	if (mr->pages) {
		long nr_refs = mr->nr_pages;

		if (mr->flags & IO_REGION_F_SINGLE_REF)
			nr_refs = 1;

		if (mr->flags & IO_REGION_F_USER_PROVIDED)
			unpin_user_pages(mr->pages, nr_refs);
		else
			release_pages(mr->pages, nr_refs);

		kvfree(mr->pages);
	}
	if ((mr->flags & IO_REGION_F_VMAP) && mr->ptr)
		vunmap(mr->ptr);
	if (mr->nr_pages && ctx->user)
		__io_unaccount_mem(ctx->user, mr->nr_pages);

	memset(mr, 0, sizeof(*mr));
}

/**
 * This function initializes the `ptr` member of the given `io_mapped_region`
 * structure. It first checks if the pages in the region can be coalesced into
 * a single buffer using `io_check_coalesce_buffer`. If coalescing is possible
 * and the number of folios is 1, the pointer is set to the address of the
 * first page. Otherwise, the function attempts to map the pages into a
 * contiguous virtual memory area using `vmap`. If successful, the pointer is
 * set to the mapped address, and the `IO_REGION_F_VMAP` flag is added to the
 * region's flags. If `vmap` fails, the function returns `-ENOMEM`.
 */
static int io_region_init_ptr(struct io_mapped_region *mr)
{
	struct io_imu_folio_data ifd;
	void *ptr;

	if (io_check_coalesce_buffer(mr->pages, mr->nr_pages, &ifd)) {
		if (ifd.nr_folios == 1) {
			mr->ptr = page_address(mr->pages[0]);
			return 0;
		}
	}
	ptr = vmap(mr->pages, mr->nr_pages, VM_MAP, PAGE_KERNEL);
	if (!ptr)
		return -ENOMEM;

	mr->ptr = ptr;
	mr->flags |= IO_REGION_F_VMAP;
	return 0;
}

/**
 * This function pins a range of user-provided memory pages into memory, ensuring
 * they remain resident for the duration of the I/O operation. It calculates the
 * size of the memory region based on the number of pages and attempts to pin the
 * pages using the io_pin_pages function. If successful, the pinned pages are
 * stored in the io_mapped_region structure, and the region is marked as user-provided..
 */
static int io_region_pin_pages(struct io_ring_ctx *ctx,
				struct io_mapped_region *mr,
				struct io_uring_region_desc *reg)
{
	unsigned long size = mr->nr_pages << PAGE_SHIFT;
	struct page **pages;
	int nr_pages;

	pages = io_pin_pages(reg->user_addr, size, &nr_pages);
	if (IS_ERR(pages))
		return PTR_ERR(pages);
	if (WARN_ON_ONCE(nr_pages != mr->nr_pages))
		return -EFAULT;

	mr->pages = pages;
	mr->flags |= IO_REGION_F_USER_PROVIDED;
	return 0;
}

/**
 * This function allocates memory pages for a specified I/O region. It first
 * attempts to allocate a compound page using `io_mem_alloc_compound`. If that
 * fails, it falls back to bulk allocation of individual pages using
 * `alloc_pages_bulk_node`. The allocated pages are stored in the `mr->pages`
 * array, and the memory mapping offset is set in the `reg->mmap_offset`.
 */
static int io_region_allocate_pages(struct io_ring_ctx *ctx,
				    struct io_mapped_region *mr,
				    struct io_uring_region_desc *reg,
				    unsigned long mmap_offset)
{
	gfp_t gfp = GFP_KERNEL_ACCOUNT | __GFP_ZERO | __GFP_NOWARN;
	unsigned long size = mr->nr_pages << PAGE_SHIFT;
	unsigned long nr_allocated;
	struct page **pages;
	void *p;

	pages = kvmalloc_array(mr->nr_pages, sizeof(*pages), gfp);
	if (!pages)
		return -ENOMEM;

	p = io_mem_alloc_compound(pages, mr->nr_pages, size, gfp);
	if (!IS_ERR(p)) {
		mr->flags |= IO_REGION_F_SINGLE_REF;
		goto done;
	}

	nr_allocated = alloc_pages_bulk_node(gfp, NUMA_NO_NODE,
					     mr->nr_pages, pages);
	if (nr_allocated != mr->nr_pages) {
		if (nr_allocated)
			release_pages(pages, nr_allocated);
		kvfree(pages);
		return -ENOMEM;
	}
done:
	reg->mmap_offset = mmap_offset;
	mr->pages = pages;
	return 0;
}

/**
 * This function initializes a memory-mapped region based on the provided
 * region descriptor. It validates the input parameters, calculates the
 * number of pages required, and either pins user memory pages or allocates
 * kernel memory pages for the region. It also initializes the region's
 * pointer for further use.
 */
int io_create_region(struct io_ring_ctx *ctx, struct io_mapped_region *mr,
		     struct io_uring_region_desc *reg,
		     unsigned long mmap_offset)
{
	int nr_pages, ret;
	u64 end;

	if (WARN_ON_ONCE(mr->pages || mr->ptr || mr->nr_pages))
		return -EFAULT;
	if (memchr_inv(&reg->__resv, 0, sizeof(reg->__resv)))
		return -EINVAL;
	if (reg->flags & ~IORING_MEM_REGION_TYPE_USER)
		return -EINVAL;
	/* user_addr should be set IFF it's a user memory backed region */
	if ((reg->flags & IORING_MEM_REGION_TYPE_USER) != !!reg->user_addr)
		return -EFAULT;
	if (!reg->size || reg->mmap_offset || reg->id)
		return -EINVAL;
	if ((reg->size >> PAGE_SHIFT) > INT_MAX)
		return -E2BIG;
	if ((reg->user_addr | reg->size) & ~PAGE_MASK)
		return -EINVAL;
	if (check_add_overflow(reg->user_addr, reg->size, &end))
		return -EOVERFLOW;

	nr_pages = reg->size >> PAGE_SHIFT;
	if (ctx->user) {
		ret = __io_account_mem(ctx->user, nr_pages);
		if (ret)
			return ret;
	}
	mr->nr_pages = nr_pages;

	if (reg->flags & IORING_MEM_REGION_TYPE_USER)
		ret = io_region_pin_pages(ctx, mr, reg);
	else
		ret = io_region_allocate_pages(ctx, mr, reg, mmap_offset);
	if (ret)
		goto out_free;

	ret = io_region_init_ptr(mr);
	if (ret)
		goto out_free;
	return 0;
out_free:
	io_free_region(ctx, mr);
	return ret;
}

/**
 * This function creates a memory-mapped region in a thread-safe manner. It first creates
 * a temporary mapped region and then copies it to the provided mapped region pointer
 * after acquiring the necessary lock. This ensures that the region is safely published
 * and can be accessed without holding the full uring_lock, relying only on the mmap_lock.
 */
int io_create_region_mmap_safe(struct io_ring_ctx *ctx, struct io_mapped_region *mr,
				struct io_uring_region_desc *reg,
				unsigned long mmap_offset)
{
	struct io_mapped_region tmp_mr;
	int ret;

	memcpy(&tmp_mr, mr, sizeof(tmp_mr));
	ret = io_create_region(ctx, &tmp_mr, reg, mmap_offset);
	if (ret)
		return ret;

	/*
	 * Once published mmap can find it without holding only the ->mmap_lock
	 * and not ->uring_lock.
	 */
	guard(mutex)(&ctx->mmap_lock);
	memcpy(mr, &tmp_mr, sizeof(tmp_mr));
	return 0;
}

/**
 * This function determines the appropriate memory-mapped region within the
 * io_uring context based on the provided page offset. The offset is shifted
 * and masked to identify the specific region type. The function supports the
 * following region types:
 *
 * - IORING_OFF_SQ_RING: Submission Queue Ring region.
 * - IORING_OFF_CQ_RING: Completion Queue Ring region.
 * - IORING_OFF_SQES: Submission Queue Entries region.
 * - IORING_OFF_PBUF_RING: Provided Buffer Ring region, identified by a buffer
 *   group ID (bgid).
 * - IORING_MAP_OFF_PARAM_REGION: Parameter region.
 * - IORING_MAP_OFF_ZCRX_REGION: Zero-copy receive region.
 */
static struct io_mapped_region *io_mmap_get_region(struct io_ring_ctx *ctx,
						   loff_t pgoff)
{
	loff_t offset = pgoff << PAGE_SHIFT;
	unsigned int bgid;

	switch (offset & IORING_OFF_MMAP_MASK) {
	case IORING_OFF_SQ_RING:
	case IORING_OFF_CQ_RING:
		return &ctx->ring_region;
	case IORING_OFF_SQES:
		return &ctx->sq_region;
	case IORING_OFF_PBUF_RING:
		bgid = (offset & ~IORING_OFF_MMAP_MASK) >> IORING_OFF_PBUF_SHIFT;
		return io_pbuf_get_region(ctx, bgid);
	case IORING_MAP_OFF_PARAM_REGION:
		return &ctx->param_region;
	case IORING_MAP_OFF_ZCRX_REGION:
		return &ctx->zcrx_region;
	}
	return NULL;
}

/**
 * This function checks if the provided memory-mapped region is valid within the
 * io_uring context. It ensures that the region is properly set and not flagged
 * as user-provided. If the region is invalid, it returns an error pointer with
 * the appropriate error code.
 */
static void *io_region_validate_mmap(struct io_ring_ctx *ctx,
				     struct io_mapped_region *mr)
{
	lockdep_assert_held(&ctx->mmap_lock);

	if (!io_region_is_set(mr))
		return ERR_PTR(-EINVAL);
	if (mr->flags & IO_REGION_F_USER_PROVIDED)
		return ERR_PTR(-EINVAL);

	return io_region_get_ptr(mr);
}

/**
 * This function checks if the memory mapping request is valid by retrieving
 * the mapped region associated with the given page offset and validating it.
 * If the region is not found or the validation fails, an error pointer is returned.
 */
static void *io_uring_validate_mmap_request(struct file *file, loff_t pgoff,
					    size_t sz)
{
	struct io_ring_ctx *ctx = file->private_data;
	struct io_mapped_region *region;

	region = io_mmap_get_region(ctx, pgoff);
	if (!region)
		return ERR_PTR(-EINVAL);
	return io_region_validate_mmap(ctx, region);
}

#ifdef CONFIG_MMU

/**
 * This function maps a specified memory region into a given virtual memory area
 * (VMA) while ensuring that the number of pages mapped does not exceed the
 * specified maximum. It sets the VM_DONTEXPAND flag on the VMA to prevent
 * expansion and uses vm_insert_pages to insert the pages into the VMA.
 */
static int io_region_mmap(struct io_ring_ctx *ctx,
			  struct io_mapped_region *mr,
			  struct vm_area_struct *vma,
			  unsigned max_pages)
{
	unsigned long nr_pages = min(mr->nr_pages, max_pages);

	vm_flags_set(vma, VM_DONTEXPAND);
	return vm_insert_pages(vma, vma->vm_start, mr->pages, &nr_pages);
}

/**
 * This function is responsible for validating and setting up memory mappings
 * for io_uring. It ensures that the requested memory region is valid and
 * determines the appropriate page limit based on the type of mapping requested.
 * The function also retrieves or creates the appropriate memory-mapped region
 * and performs the actual mapping.
 */
__cold int io_uring_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct io_ring_ctx *ctx = file->private_data;
	size_t sz = vma->vm_end - vma->vm_start;
	long offset = vma->vm_pgoff << PAGE_SHIFT;
	unsigned int page_limit = UINT_MAX;
	struct io_mapped_region *region;
	void *ptr;

	guard(mutex)(&ctx->mmap_lock);

	ptr = io_uring_validate_mmap_request(file, vma->vm_pgoff, sz);
	if (IS_ERR(ptr))
		return PTR_ERR(ptr);

	switch (offset & IORING_OFF_MMAP_MASK) {
	case IORING_OFF_SQ_RING:
	case IORING_OFF_CQ_RING:
		page_limit = (sz + PAGE_SIZE - 1) >> PAGE_SHIFT;
		break;
	}

	region = io_mmap_get_region(ctx, vma->vm_pgoff);
	return io_region_mmap(ctx, region, vma, page_limit);
}

/**

 * This function is responsible for retrieving an unmapped memory area for
 * io_uring. It ensures that the user-provided address is not used to avoid
 * breaking aliasing rules, as userspace cannot predict the offset address of
 * kernel-allocated memory. The function validates the mmap request and handles
 * architecture-specific requirements for cache aliasing.
 */
unsigned long io_uring_get_unmapped_area(struct file *filp, unsigned long addr,
					 unsigned long len, unsigned long pgoff,
					 unsigned long flags)
{
	struct io_ring_ctx *ctx = filp->private_data;
	void *ptr;

	/*
	 * Do not allow to map to user-provided address to avoid breaking the
	 * aliasing rules. Userspace is not able to guess the offset address of
	 * kernel kmalloc()ed memory area.
	 */
	if (addr)
		return -EINVAL;

	guard(mutex)(&ctx->mmap_lock);

	ptr = io_uring_validate_mmap_request(filp, pgoff, len);
	if (IS_ERR(ptr))
		return -ENOMEM;

	/*
	 * Some architectures have strong cache aliasing requirements.
	 * For such architectures we need a coherent mapping which aliases
	 * kernel memory *and* userspace memory. To achieve that:
	 * - use a NULL file pointer to reference physical memory, and
	 * - use the kernel virtual address of the shared io_uring context
	 *   (instead of the userspace-provided address, which has to be 0UL
	 *   anyway).
	 * - use the same pgoff which the get_unmapped_area() uses to
	 *   calculate the page colouring.
	 * For architectures without such aliasing requirements, the
	 * architecture will return any suitable mapping because addr is 0.
	 */
	filp = NULL;
	flags |= MAP_SHARED;
	pgoff = 0;	/* has been translated to ptr above */
#ifdef SHM_COLOUR
	addr = (uintptr_t) ptr;
	pgoff = addr >> PAGE_SHIFT;
#else
	addr = 0UL;
#endif
	return mm_get_unmapped_area(current->mm, filp, addr, len, pgoff, flags);
}

#else /* !CONFIG_MMU */

/**
 * This function determines whether the memory mapping is allowed based on
 * the flags in the virtual memory area structure. If the mapping is a
 * shared mapping in a no-MMU (Memory Management Unit) environment, it
 * returns 0 to indicate success. Otherwise, it returns -EINVAL to indicate
 * an invalid argument.
 */
int io_uring_mmap(struct file *file, struct vm_area_struct *vma)
{
	return is_nommu_shared_mapping(vma->vm_flags) ? 0 : -EINVAL;
}

/**
 * This function returns the mmap capabilities for a file in a no-MMU (Memory
 * Management Unit) environment. The returned capabilities indicate the
 * supported mapping options, such as direct mapping, read access, and write
 * access.
 */
unsigned int io_uring_nommu_mmap_capabilities(struct file *file)
{
	return NOMMU_MAP_DIRECT | NOMMU_MAP_READ | NOMMU_MAP_WRITE;
}

unsigned long io_uring_get_unmapped_area(struct file *file, unsigned long addr,
					 unsigned long len, unsigned long pgoff,
					 unsigned long flags)
{
	struct io_ring_ctx *ctx = file->private_data;
	void *ptr;

	guard(mutex)(&ctx->mmap_lock);

	ptr = io_uring_validate_mmap_request(file, pgoff, len);
	if (IS_ERR(ptr))
		return PTR_ERR(ptr);

	return (unsigned long) ptr;
}

#endif /* !CONFIG_MMU */

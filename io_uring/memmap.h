#ifndef IO_URING_MEMMAP_H
#define IO_URING_MEMMAP_H

#define IORING_MAP_OFF_PARAM_REGION		0x20000000ULL
#define IORING_MAP_OFF_ZCRX_REGION		0x30000000ULL

/**
 * This function pins a range of user-space memory pages specified by the
 * starting address and length. The pinned pages are returned as an array
 * of `struct page` pointers. The number of pages pinned is stored in the
 * integer pointed to by `npages`.
 */
struct page **io_pin_pages(unsigned long ubuf, unsigned long len, int *npages);

#ifndef CONFIG_MMU
unsigned int io_uring_nommu_mmap_capabilities(struct file *file);
#endif
/**
 * This function is used to find an unmapped memory region that satisfies
 * the specified parameters. It is typically used in the context of io_uring
 * to allocate memory regions for efficient I/O operations.
 */
unsigned long io_uring_get_unmapped_area(struct file *file, unsigned long addr,
					 unsigned long len, unsigned long pgoff,
					 unsigned long flags);
/**
 * This function is responsible for setting up the memory mapping for an io_uring
 * instance. It maps the necessary memory regions to the user space as requested
 * by the virtual memory area (VMA) structure. The mapping allows user-space
 * applications to interact with io_uring efficiently.
 */
int io_uring_mmap(struct file *file, struct vm_area_struct *vma);

/**
 * This function releases the resources associated with a memory-mapped region
 * that was previously allocated or registered with the io_uring context.
 */
void io_free_region(struct io_ring_ctx *ctx, struct io_mapped_region *mr);
int io_create_region(struct io_ring_ctx *ctx, struct io_mapped_region *mr,
		     struct io_uring_region_desc *reg,
		     unsigned long mmap_offset);

int io_create_region_mmap_safe(struct io_ring_ctx *ctx,
				struct io_mapped_region *mr,
				struct io_uring_region_desc *reg,
				unsigned long mmap_offset);

static inline void *io_region_get_ptr(struct io_mapped_region *mr)
{
	return mr->ptr;
}

static inline bool io_region_is_set(struct io_mapped_region *mr)
{
	return !!mr->nr_pages;
}

#endif

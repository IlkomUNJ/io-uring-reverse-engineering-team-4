#ifndef IOU_ALLOC_CACHE_H
#define IOU_ALLOC_CACHE_H

#include <linux/io_uring_types.h>

/*
 * Don't allow the cache to grow beyond this size.
 */
#define IO_ALLOC_CACHE_MAX	128

/// Frees all objects stored in the cache and the cache itself.
/// Takes a custom function pointer to properly free each object.
void io_alloc_cache_free(struct io_alloc_cache *cache,
			 void (*free)(const void *));
/// Initializes the allocation cache with size and clear-byte options.
/// Returns false (0) if successful, or true (1) if allocation fails.
bool io_alloc_cache_init(struct io_alloc_cache *cache,
			 unsigned max_nr, unsigned int size,
			 unsigned int init_bytes);

/// Allocates a new memory object from scratch.
/// If `init_bytes` is set, it clears the beginning of the object.
/// Used when the cache is empty.
void *io_cache_alloc_new(struct io_alloc_cache *cache, gfp_t gfp);

/// Tries to put an object back into the cache for reuse.
/// Returns true if successfully cached, or false if cache is full.
/// Also runs KASAN (memory sanitizer) poison check to help catch bugs.
static inline bool io_alloc_cache_put(struct io_alloc_cache *cache,
				      void *entry)
{
	if (cache->nr_cached < cache->max_cached) {
		if (!kasan_mempool_poison_object(entry))
			return false;
		cache->entries[cache->nr_cached++] = entry;
		return true;
	}
	return false;
}

/// Gets an object from the cache if available; otherwise returns NULL.
/// If KASAN is enabled, it unpoisons and optionally clears the object before returning it.
static inline void *io_alloc_cache_get(struct io_alloc_cache *cache)
{
	if (cache->nr_cached) {
		void *entry = cache->entries[--cache->nr_cached];

		/*
		 * If KASAN is enabled, always clear the initial bytes that
		 * must be zeroed post alloc, in case any of them overlap
		 * with KASAN storage.
		 */
#if defined(CONFIG_KASAN)
		kasan_mempool_unpoison_object(entry, cache->elem_size);
		if (cache->init_clear)
			memset(entry, 0, cache->init_clear);
#endif
		return entry;
	}

	return NULL;
}

/// Allocates an object either from the cache or fresh if cache is empty.
/// This is the main function for grabbing usable memory with fallback.
static inline void *io_cache_alloc(struct io_alloc_cache *cache, gfp_t gfp)
{
	void *obj;

	obj = io_alloc_cache_get(cache);
	if (obj)
		return obj;
	return io_cache_alloc_new(cache, gfp);
}

/// Frees an object either by returning it to the cache or fully freeing it.
/// If the cache is full, it calls `kfree()` to release memory completely.
static inline void io_cache_free(struct io_alloc_cache *cache, void *obj)
{
	if (!io_alloc_cache_put(cache, obj))
		kfree(obj);
}

#endif

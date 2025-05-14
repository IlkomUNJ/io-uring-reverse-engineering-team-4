// SPDX-License-Identifier: GPL-2.0

#include "alloc_cache.h"

/// Frees all memory previously stored in the cache.
/// It goes through each cached entry and frees it using the provided `free` function.
/// After that, it also frees the entire cache storage array.
void io_alloc_cache_free(struct io_alloc_cache *cache,
			 void (*free)(const void *))
{
	void *entry;

	if (!cache->entries)
		return;

	while ((entry = io_alloc_cache_get(cache)) != NULL)
		free(entry);

	kvfree(cache->entries);
	cache->entries = NULL;
}

/* returns false if the cache was initialized properly */
/// Initializes the allocation cache with the given parameters.
/// Allocates memory to store pointers to cached entries.
/// Returns false (0) on success, or true (1) if memory allocation failed.
bool io_alloc_cache_init(struct io_alloc_cache *cache,
			 unsigned max_nr, unsigned int size,
			 unsigned int init_bytes)
{
	cache->entries = kvmalloc_array(max_nr, sizeof(void *), GFP_KERNEL);
	if (!cache->entries)
		return true;

	cache->nr_cached = 0;
	cache->max_cached = max_nr;
	cache->elem_size = size;
	cache->init_clear = init_bytes;
	return false;
}

/// Allocates a new object for the cache with optional memory clearing.
/// Allocates memory of `elem_size`. If `init_clear` is set, it zeroes out the memory.
/// Returns pointer to the new object or NULL if allocation fails.
void *io_cache_alloc_new(struct io_alloc_cache *cache, gfp_t gfp)
{
	void *obj;

	obj = kmalloc(cache->elem_size, gfp);
	if (obj && cache->init_clear)
		memset(obj, 0, cache->init_clear);
	return obj;
}

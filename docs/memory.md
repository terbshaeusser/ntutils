# Memory

The following functions deal with memory.
It is possible to install a custom allocator by overwriting the following weak functions:

* `void *P_nt_custom_malloc(size_t element_size, size_t count)`
* `void *P_nt_custom_calloc(size_t element_size, size_t count)`
* `void *P_nt_custom_realloc(void *ptr, size_t element_size, size_t new_count)`
* `void P_nt_custom_free(void *ptr)`


### nt_alloc

Include: [nt/mem.h](../include/nt/mem.h)

Parameters:

* type: The type the allocated memory will store.
* count (`size_t`, optional): Number of items to allocate. By default this parameter is set to `1`.
* clear (`bool`, optional): Determines if the allocated memory should be filled with 0s. By default this parameter is
  set to `false`.

Return:

* (`type*`) A pointer to the allocated memory.

Allocates memory on the heap with size of the given type * `count`. If the allocating failed, a panic is raised.


### nt_try_alloc

Include: [nt/mem.h](../include/nt/mem.h)

Parameters:

* type: The type the allocated memory will store.
* count (`size_t`, optional): Number of items to allocate. By default this parameter is set to `1`.
* clear (`bool`, optional): Determines if the allocated memory should be filled with 0s. By default this parameter is
  set to `false`.

Return:

* (`type*`) A pointer to the allocated memory or `NULL` if the allocation failed.

Allocates memory on the heap with size of the given type * `count`.


### nt_realloc

Include: [nt/mem.h](../include/nt/mem.h)

Parameters:

* ptr: A pointer to an existing memory block on the heap.
* new_count (`size_t`): Number of items the block should hold after resizing.

Return:

* (`typeof(ptr)`) A pointer to the resized memory.

Resizes an already allocated block on the heap to a new size of the given type * `count`.
If the resize operation fails, a panic is raised.


### nt_try_realloc

Include: [nt/mem.h](../include/nt/mem.h)

Parameters:

* ptr: A pointer to an existing memory block on the heap.
* new_count (`size_t`): Number of items the block should hold after resizing.

Return:

* (`typeof(ptr)`) A pointer to the resized memory.

Resizes an already allocated block on the heap to a new size of the given type * `count`.


### nt_free

Include: [nt/mem.h](../include/nt/mem.h)

Parameters:

* ptr (`void*`): A pointer to an existing memory block on the heap.

Deallocates the block `ptr` is pointing to. Nothing happens if `ptr` is `NULL`.

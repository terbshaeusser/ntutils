# Functions

## Assertions

### nt_assert

Include: `nt/assert.h`

Parameters:

* condition: A condition that is checked to be `!= 0`.
* message (`char const*`, optional): An optional message that is displayed on the screen.

Ensures that a given condition is met. Otherwise a panic is raised with the source location, the condition and an
optional custom message.

Asserts can be disabled by defining the `NT_DISABLE_ASSERTS` macro.


## Memory

The following functions deal with memory.
It is possible to install a custom allocator by overwriting the following weak functions:

* `void *P_nt_custom_malloc(size_t element_size, size_t count)`
* `void *P_nt_custom_calloc(size_t element_size, size_t count)`
* `void *P_nt_custom_realloc(void *ptr, size_t element_size, size_t new_count)`
* `void P_nt_custom_free(void *ptr)`


### nt_alloc

Include: `nt/mem.h`

Parameters:

* type: The type the allocated memory will store.
* count (`size_t`, optional): Number of items to allocate. By default this parameter is set to `1`.
* clear (`bool`, optional): Determines if the allocated memory should be filled with 0s. By default this parameter is
  set to `false`.

Return:

* A pointer to the allocated memory of type `type*`.

Allocates memory on the heap with size of the given type * `count`. If the allocating failed, a panic is raised.


### nt_try_alloc

Include: `nt/mem.h`

Parameters:

* type: The type the allocated memory will store.
* count (`size_t`, optional): Number of items to allocate. By default this parameter is set to `1`.
* clear (`bool`, optional): Determines if the allocated memory should be filled with 0s. By default this parameter is
  set to `false`.

Return:

* A pointer to the allocated memory of type `type*` or `NULL` if the allocation failed.

Allocates memory on the heap with size of the given type * `count`.


### nt_realloc

Include: `nt/mem.h`

Parameters:

* ptr: A pointer to an existing memory block on the heap.
* new_count (`size_t`): Number of items the block should hold after resizing.

Return:

* A pointer to the resized memory of type `type*`.

Resizes an already allocated block on the heap to a new size of the given type * `count`.
If the resize operation fails, a panic is raised.


### nt_try_realloc

Include: `nt/mem.h`

Parameters:

* ptr: A pointer to an existing memory block on the heap.
* new_count (`size_t`): Number of items the block should hold after resizing.

Return:

* A pointer to the resized memory of type `type*` or `NULL` if the operation failed.

Resizes an already allocated block on the heap to a new size of the given type * `count`.


### nt_free

Include: `nt/mem.h`

Parameters:

* ptr (`void*`): A pointer to an existing memory block on the heap.

Deallocates the block `ptr` is pointing to. Nothing happens if `ptr` is `NULL`.


## Panic

### nt_push_panic_handler

Include: `nt/panic.h`

Parameters:

* handler (`nt_panic_handler_t`): Handler function that should be registered.
* ctx (`void*`): Context handler that is passed to the handler function.

Registers a given function as a panic handler. In case `nt_panic` is executed, all registered handlers are called in the
reverse registration order.

`NT_PANIC_MAX_HANDLER_COUNT` specifies how many handlers can be registered at the same time.


### nt_pop_panic_handler

Include: `nt/panic.h`

Removes the panic handler that was registered latest. Handlers are stored in a stack like structure.


### nt_panic

Parameters:

* message (`char const*`): A message that is printed to the screen. Optional following parameters can be used for a
  formatted message.

Calls all registered panic handlers in reverse registration order, prints the passed message on the screen and
terminates with the exit code specified by `NT_PANIC_EXIT_CODE`.

# Functions

## Assertions

### nt_assert

Include: [nt/assert.h](../include/nt/assert.h)

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


## Panic

### nt_push_panic_handler

Include: [nt/panic.h](../include/nt/panic.h)

Parameters:

* handler (`nt_panic_handler_t`): Handler function that should be registered.
* ctx (`void*`): Context handler that is passed to the handler function.

Registers a given function as a panic handler. In case `nt_panic` is executed, all registered handlers are called in the
reverse registration order.

`NT_PANIC_MAX_HANDLER_COUNT` specifies how many handlers can be registered at the same time.


### nt_pop_panic_handler

Include: [nt/panic.h](../include/nt/panic.h)

Removes the panic handler that was registered latest. Handlers are stored in a stack like structure.


### nt_panic

Include: [nt/panic.h](../include/nt/panic.h)

Parameters:

* message (`char const*`): A message that is printed to the screen. Optional following parameters can be used for a
  formatted message.

Calls all registered panic handlers in reverse registration order, prints the passed message on the screen and
terminates with the exit code specified by `NT_PANIC_EXIT_CODE`.


## Test Helpers

### nt_test

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* func (`void(*)()`): A function containing the logic of the test. The name of the function is also displayed on the
  screen.

Executes the given test function and prints an according message on the screen.


### nt_assert_equal

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.
* comparator (`int(*)(type, type)`): A function that can be used to compare the values. This is only necessary for non
  primitive types.

Checks if the expected and the actual value are equal. If not, a panic is raised.


### nt_assert_unequal

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.
* comparator (`int(*)(type, type)`): A function that can be used to compare the values. This is only necessary for non
  primitive types.

Checks if the expected and the actual value are unequal. If not, a panic is raised.


### nt_assert_less

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.

Checks if the actual value is less than the expected one. If not, a panic is raised.


### nt_assert_less_equal

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.

Checks if the actual value is less than or equal the expected one. If not, a panic is raised.


### nt_assert_greater

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.

Checks if the actual value is greater than the expected one. If not, a panic is raised.


### nt_assert_greater_equal

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.

Checks if the actual value is greater than or equal the expected one. If not, a panic is raised.


### nt_assert_true

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* actual: The actual value.

Checks if the actual value evaluates to `true`. If not, a panic is raised.


### nt_assert_false

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* actual: The actual value.

Checks if the actual value evaluates to `false`. If not, a panic is raised.


## Measurement

### nt_measure

Include: [nt/measurement.h](../include/nt/measurement.h)

Parameters:

* code: Code that should be measured.
* result_var: A variable of type `int64_t` where the result should be stored.

Measures the elapsed time a piece of code takes to execute in µs.


### nt_print_measure

Include: [nt/measurement.h](../include/nt/measurement.h)

Parameters:

* time (`int64_t`): The elapsed time to print in µs.

Pretty prints the passed time to the screen.

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


## Circular Buffer

Circular buffer types are defined with the `NT_CIRC_BUF(name, item_type, capacity)` macro. This macro defines the
necessary type with the name `name_t` and additional utility functions to interact with instances of this type.
The functions are also prefixed with the passed name. In case the capacity is not known at compile time, the third
parameter can be omitted. The capacity must then be passed to the `name_new` function.

Example:

```
#include "nt/circular.h"

NT_CIRC_BUF(static_int_buffer, int, 4)
NT_CIRC_BUF(dyn_int_buffer, int)

int main() {
  static_int_buffer_t static_buffer = static_int_buffer_new();
  static_int_buffer_push(&static_buffer, 1);
  static_int_buffer_free(&static_buffer);

  dyn_int_buffer_t dyn_buffer = dyn_int_buffer_new(4);
  dyn_int_buffer_push(&dyn_buffer, 1);
  dyn_int_buffer_free(&dyn_buffer);
  return 0;
}
```

### *_new

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* capacity (`size_t`): The capacity of the new circular buffer instance. This parameter must only be passed if the
  capacity was omitted when defining the buffer type.

Return:

* (`name_t`) A new circular buffer instance with the passed capacity.

Creates a new circular buffer instance and returns it.


### *_free

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance to release.

Releases a previously allocated circular buffer.


### *_capacity

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.

Return:

* (`size_t`) The capacity of the given circular buffer instance.


### *_count

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.

Return:

* (`size_t`) The number of items currently pushed on the given circular buffer instance.


### *_push

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.
* item: The item to push.

Pushes a new item to the end of the given circular buffer instance. The function asserts that the buffer has enough
space for another item.


### *_push_ref

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.
* item: A pointer to the item to push.

Pushes a new item to the end of the given circular buffer instance. The function asserts that the buffer has enough 
space for another item.


### *_pop

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.
* item: A pointer to a place where the popped value should be stored or `NULL`.

Pops the next value from the beginning of a given circular buffer instance. An assert ensures that the buffer is not
empty.


### *_get

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.
* index (`size_t`): The index of the item.

Return:

* A pointer to the item with the given index.

Returns the item with the given index. The index is checked with an assert.


### *_clear

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.

Removes all items from the given circular buffer.


## Unicode

### nt_cp

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* code_point (`uint32_t`): A number representing a code point.

Return:

* (`nt_cp_t`) A struct holding the passed number.

Helper function to convert integers to `nt_cp_t`.


### nt_cp_is_valid

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* cp (`nt_cp_t`): A code point to check.

Check whether the passed code point is a valid Unicode code point.


## UTF-8

### nt_utf8_len

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* cp (`nt_cp_t`): A code point.

Return:

* (`int`) The number of bytes UTF-8 needs to encode the given code point.


### nt_utf8_read

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* str (`char const **`): Pointer to a string.
* len (`size_t *`): Pointer to the length of the string.

Return:

* (`nt_cp_t`) The read code point. If the function fails, an invalid code point is returned.

Reads the next code point from an UTF-8 encoded string and advances `str` / decreases `len` by the number of decoded
bytes.


### nt_utf8_write

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* str (`char **`): Pointer to a string.
* len (`size_t *`): Pointer to the length of the string.
* cp (`nt_cp_t`): A code point.

Writes the passed code point UTF-8 encoded to the beginning to the passed string. `str` is increased / `len` decreased
by the number of required bytes. If the code point is invalid or `len` is too small to hold the code point, nothing is
done.


### nt_utf8_has_bom

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* str (`char const *`): A string.
* len (`size_t`): The length of the string.

Return:

* (`bool`) Returns `true` if the passed string starts with the UTF-8 BOM.


### nt_utf8_write_bom

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* str (`char *`): A string.

Writes the UTF-8 BOM to the passed string. The caller is responsible that at least `NT_UTF8_BOM_LEN` bytes are
available.


## Streams

### nt_stream_from_file

Include: [nt/stream.h](../include/nt/stream.h)

Parameters:

* stream (`nt_stream_t*`): A pointer to an unused stream instance.
* mode (`nt_file_mode_t`): The mode in which the file should be opened.
* path (`char const*`): The path of the file to open.

Return:

* (`bool`) `true` if the stream was successfully opened.

Creates a stream to access a file. The function pointers inside the stream type can be used for interaction and release.


## nt_stream_from_str

Include: [nt/stream.h](../include/nt/stream.h)

Parameters:

* stream (`nt_stream_t*`): A pointer to an unused stream instance.
* str (`char const*`): The string that will be used as source for the stream.
* len (`size_t`, optional): The length of the string. If none is given, the length will be computed with `strlen`.

Creates a read-only stream from a string. The function pointers inside the stream type can be used for interaction and
release.

# Functions

## Assertions

### nt_assert

Include: [nt/assert.h](../include/nt/assert.h)

Parameters:

* condition: A condition that is checked to be `!= 0`.
* message (`char const*`, optional): An optional message that is displayed on the screen.
* args (optional): Var args holding arguments for formatting `message`.

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
* comparator (`nt_comparison_result_t(*)(type, type)`): A function that can be used to compare the values. This is only
  necessary for non primitive types.

Checks if the expected and the actual value are equal. If not, a panic is raised.


### nt_assert_unequal

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.
* comparator (`nt_comparison_result_t(*)(type, type)`): A function that can be used to compare the values. This is only
  necessary for non primitive types.

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


### nt_assert_null

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* actual: The actual value.

Checks if the actual value evaluates to `NULL`. If not, a panic is raised.


### nt_assert_not_null

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* actual: The actual value.

Checks if the actual value evaluates to a value unequal to `NULL`. If not, a panic is raised.


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

* self (`name_t const*`): The circular buffer instance.

Return:

* (`size_t`) The capacity of the given circular buffer instance.


### *_count

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t const*`): The circular buffer instance.

Return:

* (`size_t`) The number of items currently pushed on the given circular buffer instance.


### *_push

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.
* item (`item_type`): The item to push.

Pushes a new item to the end of the given circular buffer instance. The function asserts that the buffer has enough
space for another item.


### *_push_ref

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.
* item (`item_type const*`): A pointer to the item to push.

Pushes a new item to the end of the given circular buffer instance. The function asserts that the buffer has enough 
space for another item.


### *_pop

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.
* item (`item_type*`): A pointer to a place where the popped value should be stored or `NULL`.

Pops the next value from the beginning of a given circular buffer instance. An assert ensures that the buffer is not
empty.


### *_clear

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t*`): The circular buffer instance.

Removes all items from the given circular buffer.


### *_get

Include: [nt/circular.h](../include/nt/circular.h)

Parameters:

* self (`name_t const*`): The circular buffer instance.
* index (`size_t`): The index of the item.

Return:

* (`item_type const*`) A pointer to the item with the given index.

Returns the item with the given index. The index is checked with an assert.


## List

List types are defined with the `NT_LIST(name, item_type)` macro. This macro defines the necessary type with the name
`name_t` and additional utility functions to interact with instances of this type. The functions are also prefixed with
the passed name.

**Hint:** The list implementation uses a continuous block of memory in the background.

Example:

```
#include "nt/list.h"

NT_LIST(int_list, int)

int main() {
  int_list_t list = int_list_new();
  int_list_add(&list, 2);
  int_list_insert(&list, 0, 1);
  int_list_free(&list);
  return 0;
}
```

### *_new

Include: [nt/list.h](../include/nt/list.h)

Return:

* (`name_t`) A new list instance with 0 items.

Creates a new list instance and returns it.


### *_free

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance to release.

Releases a previously allocated list.


### *_capacity

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t const*`): The list instance.

Return:

* (`size_t`) The capacity of the given list instance.


### *_count

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t const*`): The list instance.

Return:

* (`size_t`) The number of items currently stored in the list instance.


### *_reserve

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* amount (`size_t`): The number of items the function should reserve space for.

Reserves space for the given number of items. If the space is already available, the list remains unchanged.


### *_shrink

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.

Reduces the memory used by the list to the exact number of stored items.


### *_add

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* item (`item_type`): The item to add.

Return:

* (`size_t`) The index inside the list where the item was inserted.

Adds an item to the end of the list.


### *_add_ref

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* item (`item_type const*`): A pointer to the item to add.

Return:

* (`size_t`) The index inside the list where the item was inserted.

Adds an item to the end of the list.


### *_insert

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* index (`size_t`): The index the new item should have.
* item (`item_type`): The item to add.

Inserts an item into the list at the given index. The index is checked with an assert.


### *_insert_ref

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* index (`size_t`): The index the new item should have.
* item (`item_type const*`): A pointer to the item to add.

Inserts an item into the list at the given index. The index is checked with an assert.


### *_remove

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.
* index (`size_t`): The index of the item to remove.

Removes the item with the passed index. The index is checked with an assert.


### *_clear

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t*`): The list instance.

Removes all items from the given list.


### *_get

Include: [nt/list.h](../include/nt/list.h)

Parameters:

* self (`name_t const*`): The list instance.
* index (`size_t`): The index of the item.

Return:

* (`item_type const*`) A pointer to the item with the given index.

Returns the item with the given index. The index is checked with an assert.


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


### nt_cp_is_newline

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* cp (`nt_cp_t`): A code point to check.

Check whether the passed code point describes a newline `\n` character.


### nt_cp_is_whitespace

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* cp (`nt_cp_t`): A code point to check.

Check whether the passed code point describes a whitespace character.


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


## Reader

A reader takes a stream as source and reads it code point by code point. Additionally a line and column counter is
available as well as lookaheads can be performed. The number of code points that can be looked ahead are specified with
the `NT_READER_MAX_LOOKAHEAD` macro.


### nt_reader_new

Include: [nt/reader.h](../include/nt/reader.h)

Parameters:

* stream (`nt_stream_t*`): A pointer to a stream that should be used as source.
* decoder (`nt_decoder_t`, optional): A decode function that is used to "convert" the raw data into a code point.

Return:

* (`nt_reader_t`) A new reader instance.


### nt_reader_free

Include: [nt/reader.h](../include/nt/reader.h)

Parameters:

* self (`nt_reader_t*`): A pointer to an initialized reader.

Releases the passed reader instance.


### nt_reader_is_eof

Include: [nt/reader.h](../include/nt/reader.h)

Parameters:

* self (`nt_reader_t const*`): A pointer to an initialized reader.

Return:

* (`bool`) `true` if there are no more code points to read.


### nt_reader_next

Include: [nt/reader.h](../include/nt/reader.h)

Parameters:

* self (`nt_reader_t*`): A pointer to an initialized reader.
* line (`uint32_t*`, optional): A pointer to an integer where the line number of the next code point should be stored or
  `NULL`.
* column (`uint32_t*`, optional): A pointer to an integer where the column number of the next code point should be
  stored or `NULL`.

Return:

* (`nt_cp_t`) The next code point. If the were no more code points to read, an invalid code point is returned.

Reads the next code point from the reader and advances to the next one.


### nt_reader_lh

Include: [nt/reader.h](../include/nt/reader.h)

Parameters:

* self (`nt_reader_t*`): A pointer to an initialized reader.
* n (`int32_t`, optional): The number of code points to look ahead lying between 1 and `NT_READER_MAX_LOOKAHEAD`.
* line (`uint32_t*`, optional): A pointer to an integer where the line number of the code point should be stored or
  `NULL`.
* column (`uint32_t*`, optional): A pointer to an integer where the column number of the code point should be stored or
  `NULL`.

Return:

* (`nt_cp_t`) The looked ahead code point. If the were no more code points to read, an invalid code point is returned.

Looks `n` code points ahead and returns that one without advancing.


## JSON

### nt_json_parse

Include: [nt/json.h](../include/nt/json.h)

Parameters:

* reader (`nt_reader_t*`): A pointer to an initialized reader.
* ctx (`void*`): Pointer to a context that is passed to the evaluation function or `NULL`.
* eval (`nt_json_eval_t`): An evaluation function that is executed for each parsed value.
* error_handler (`nt_json_error_handler_t`): An error handler function that is executed when an error was encountered
  during parsing.

Return:

* (`bool`) `true` if no error occurred during parsing.

Parses the data in a passed reader as JSON formatted data. Each parsed value triggers the `eval` function which can read
the key and value, specify another context or eval function for child values and abort the parsing if necessary.

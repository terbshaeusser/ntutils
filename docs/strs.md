# Strings

The [nt/str.h](../include/nt/str.h) header provides a string implementation with the following properties:

* The length of the string is stored along with its characters.
* Strings can grow and shrink depending on the performed operations.
* Short strings (actual length depending on target architecture) can be stored without using heap space.


### nt_cstr

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* str (`char const*`): A C string.
* len (`size_t`, optional): The length of the C string. If this parameter is missing, the length is computed with
  `strlen`.

Return:

* (`nt_cstr_t`) A struct representing a C string with a known length.


### nt_str_new

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* src (`nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): An already existing string that is used as source.

Return:

* (`nt_str_t`) A string with a copy of the content of `src`.


### nt_str_empty

Include: [nt/str.h](../include/nt/str.h)

Return:

* (`nt_str_t`) A string representing the empty string `""`.


### nt_str_of

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* src (`nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): An already existing string that is used as source.
* count (`size_t`): Number of types the `src` string should be repeated.

Return:

* (`nt_str_t`) A string with a copy of the content of `src` repeated `count` times.


### nt_str_free

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t*`): An initialized string instance.

Releases a previously initialized string instance.


### nt_str_len

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t const*`): An initialized string instance.

Return:

* (`size_t`) The number of bytes stored in the string.


### nt_str_set_len

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t*`): An initialized string instance.
* len (`size_t`): The new length.

Sets the length of a string. The new length must be smaller than or equal the capacity.


### nt_str_capacity

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t const*`): An initialized string instance.

Return:

* (`size_t`) The number of bytes the string could currently store without reallocating memory.


### nt_str_str

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t const*`): An initialized string instance.

Return:

* (`char const*`) A pointer to the beginning of the string. The string is always terminated with a 0 byte.


### nt_str_equal

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t const*`): An initialized string instance.
* other (`nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): Another string to compare to.

Return:

* (`bool`) `true` if both supplied strings have the same content.


### nt_str_reserve

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t*`): An initialized string instance.
* amount (`size_t`): Number of bytes.

Ensures that enough space is available to append `amount` bytes to the string. A potentially necessary memory allocation
is done once and not for every following append operation (if the `amount` parameter was large enough).


### nt_str_shrink

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t*`): An initialized string instance.

Reduces the memory needed by a string to the absolute minimum. No content will be lost by this operation.


### nt_str_append

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t*`): An initialized string instance.
* src (`nt_cstr_t` / `nt_str_t*` / `nt_str_t const*` / `int{8,16,32,64}_t` / `uint{8,16,32,64}_t` / `float` / `double`):
  A value that should be appended.

Appends the given `src` to the string. The memory behind the string is increased automatically when necessary.


### nt_str_format

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* fmt (`char const*`): A format string, followed by an optional list of arguments.

Return:

* (`nt_str_t`) A string with the result.

Performs a format operation like `printf` and returns the result in a string.


### nt_str_vformat

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* fmt (`char const*`): A format string.
* args (`va_list`): The vararg list with the arguments.

Return:

* (`nt_str_t`) A string with the result.

Performs a format operation like `printf` and returns the result in a string.


### nt_str_append_format

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t*`): An initialized string instance.
* fmt (`char const*`): A format string, followed by an optional list of arguments.

Performs a format operation like `printf` and appends the result to the passed string.


### nt_str_append_vformat

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t*`): An initialized string instance.
* fmt (`char const*`): A format string.
* args (`va_list`): The vararg list with the arguments.

Performs a format operation like `printf` and appends the result to the passed string.


### nt_str_find

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t*`): An initialized string instance.
* sub_str (`nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): The string to search.
* start (`size_t`, optional): The position from where the search should start. If missing, the parameter is set to `0`.

Return:

* (`size_t`) The index where the next occurrence of `sub_str` was found or `NT_STR_NOT_FOUND`.

Searches from the specified start to the end of the passed string for a given sub string. Possible errors like an
invalid start index will **NOT** cause a panic but simply the return of `NT_STR_NOT_FOUND`.


### nt_str_rfind

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t*`): An initialized string instance.
* sub_str (`nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): The string to search.
* start (`size_t`, optional): The position from where the search should start. If missing, the parameter points to the
  end of the string. If the value is greater than the length of the string itself, the value will be automatically
  corrected.

Return:

* (`size_t`) The index where the next occurrence of `sub_str` was found or `NT_STR_NOT_FOUND`.

Searches from the specified start to the beginning of the passed string for a given sub string.


### nt_str_slice

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t const*`): An initialized string instance.
* start (`size_t`): The index where the new string should start (inclusive).
* end (`size_t`, optional): The index where the new string should end (inclusive).

Return:

* (`nt_str_t`) A new string instance containing the bytes specified by `start` and `end`.


### nt_str_assign

Include: [nt/str.h](../include/nt/str.h)

Parameters:

* self (`nt_str_t*`): An initialized string instance.
* src (`nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): An already existing string that is used as source.

Assigns the `src` string content to an already existing string. Possibly already allocated memory is reused when
possible.

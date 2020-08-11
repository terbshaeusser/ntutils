# Text Utils

## Unicode

### nt_cp_t

Include: [nt/utf8.h](../include/nt/utf8.h)

The `nt_cp_t` type represents a single Unicode code point.


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

Return:

* (`bool`) `true` if the passed code point is a valid Unicode code point.


### nt_cp_is_newline

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* cp (`nt_cp_t`): A code point to check.

Return:

* (`bool`) `true` if the passed code point describes a newline `\n` character.


### nt_cp_is_whitespace

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* cp (`nt_cp_t`): A code point to check.

Return:

* (`bool`) `true` if the passed code point describes a whitespace character.


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


### nt_utf8_to_wide

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* dst (`wchar_t*`): A pointer to a buffer where the result should be written or `NULL`.
* dst_len (`size_t`): The capacity of the destination buffer in `wchar_t`.
* src (`char const*`): The UTF-8 encoded string that should be converted.
* src_len (`size_t`, optional): The length of the UTF-8 encoded string or `0` to indicate that the function should
  determine the length.

Return:

* (`size_t`) The number of elements written to the destination buffer. If `dst_len == 0`, the number of elements that
  would be necessary for conversion. `0` indicates an error.

Converts an UTF-8 encoded string to a wide string. The function writes a terminating 0 element if `src_len == 0` and
additional space is available in the destination buffer.
This function is only defined for Windows targets.


### nt_utf8_from_wide

Include: [nt/utf8.h](../include/nt/utf8.h)

Parameters:

* dst (`char*`): A pointer to a buffer where the result should be written or `NULL`.
* dst_len (`size_t`): The capacity of the destination buffer.
* src (`wchar_t const*`): The wide string that should be converted.
* src_len (`size_t`, optional): The length of the wide string in `wchar_t` or `0` to indicate that the function should
  determine the length.

Return:

* (`size_t`) The number of elements written to the destination buffer. If `dst_len == 0`, the number of elements that
  would be necessary for conversion. `0` indicates an error.

Converts a wide string to an UTF-8 encoded string. The function writes a terminating 0 element if `src_len == 0` and
additional space is available in the destination buffer.
This function is only defined for Windows targets.


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
A return value of `true` tells the parser to continue while `false` indicates termination.

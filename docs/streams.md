# Streams

Streams provide an abstraction to an object that is able to read and/or write binary data.


### nt_stream_t

A type representing a stream.


### nt_file_mode_t

An enum representing a file mode. The following values are available:

* `NT_FILE_MODE_RO` - Open the file in read-only mode. Writes to this stream will fail with a panic. In case the
  specified file does not exist, `nt_stream_from_file` returns `false`.
* `NT_FILE_MODE_WO` - Open the file in write-only mode. Reads on this stream will fail with a panic. In case the passed
  file does not exist, it will be created. If on the other hand the file already exists, its contents will be discarded.
* `NT_FILE_MODE_RW` - Open the file for reading and writing. In case the file already exists its content will **NOT** be
  discarded. If the file is missing on the system, it will be created.


### nt_stream_from_file

Include: [nt/stream.h](../include/nt/stream.h)

Parameters:

* stream (`nt_stream_t*`): A pointer to an unused stream instance.
* mode (`nt_file_mode_t`): The mode in which the file should be opened.
* path (`char const*`): The path of the file to open.

Return:

* (`bool`) `true` if the stream was successfully opened.

Creates a stream to access a file and places the cursor at the beginning no matter which file mode is chosen.


### nt_stream_from_str

Include: [nt/stream.h](../include/nt/stream.h)

Parameters:

* stream (`nt_stream_t*`): A pointer to an unused stream instance.
* str (`char const*`): The string that will be used as source for the stream.
* len (`size_t`, optional): The length of the string. If none is given, the length will be computed with `strlen`.

Creates a read-only stream from a string. The function pointers inside the stream type can be used for interaction and
release.


### nt_stream_free

Include: [nt/stream.h](../include/nt/stream.h)

Parameters:

* self (`nt_stream_t*`): A pointer to an stream instance.

Releases the given stream instance.


### nt_stream_is_eof

Include: [nt/stream.h](../include/nt/stream.h)

Parameters:

* self (`nt_stream_t const*`): A pointer to an stream instance.

Return:

* (`bool`) `true` if the end of the stream was reached.


### nt_stream_tell

Include: [nt/stream.h](../include/nt/stream.h)

Parameters:

* self (`nt_stream_t const*`): A pointer to an stream instance.

Return:

* (`int64_t`) The current position relative to the beginning of the stream.


### nt_stream_seek

Include: [nt/stream.h](../include/nt/stream.h)

Parameters:

* self (`nt_stream_t*`): A pointer to an stream instance.
* pos (`int64_t`): The new position.
* from_end (`bool`): `true` if the position should be relative to the stream end. This might not be supported by all
  kinds of streams.

Return:

* (`bool`) `true` if the seeking could be performed.

Sets the position of the stream's cursor.


### nt_stream_read

Include: [nt/stream.h](../include/nt/stream.h)

Parameters:

* self (`nt_stream_t*`): A pointer to an stream instance.
* block (`void*`): A pointer to the beginning of a memory block where the results should be written.
* size (`size_t`): The number of bytes to read. Although the type of this parameter is `size_t`, no numbers greater than
  the range of `int32_t` should be used.

Return:

* (`int32_t`) The number of read bytes or a negative number in case of an error.

Reads data from the given stream and advances the cursor by the number of read bytes.


### nt_stream_write

Include: [nt/stream.h](../include/nt/stream.h)

Parameters:

* self (`nt_stream_t*`): A pointer to an stream instance.
* block (`void const*`): A pointer to the beginning of a memory block that should be written.
* size (`size_t`): The number of bytes to write. Although the type of this parameter is `size_t`, no numbers greater
  than the range of `int32_t` should be used.

Return:

* (`int32_t`) The number of written bytes or a negative number in case of an error.

Writes data to the given stream and advances the cursor by the number of written bytes.

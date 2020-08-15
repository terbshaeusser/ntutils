# Files

The [nt/files.h](../include/nt/files.h) header provides functions to interact with the file system. It declares the
following constants:

* `NT_MAX_PATH` - The maximum length of paths on the current platform.

On Windows all functions are able to handle Unicode characters as well as long paths.


### nt_wide_path

Include: [nt/files.h](../include/nt/files.h)

Parameters:

* str (`char*` / `char const*` / `nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): A string that should be "converted" to a Windows wide string path.

Return:

* (`wchar_t const*`) A pointer to a thread local storage containing the conversion result. The results are replaced by
  following calls to this function.

Converts a UTF-8 encoded string to a wide encoded Windows path. This function can handle long paths by prefixing `\\?\`
when necessary.

**Hint:** This function is only available on Windows.


### nt_is_absolute

Include: [nt/files.h](../include/nt/files.h)

Parameters:

* path (`char*` / `char const*` / `nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): A string that should be checked.

Return:

* (`bool`) `true` if the given path is absolute.


### nt_is_relative

Include: [nt/files.h](../include/nt/files.h)

Parameters:

* path (`char*` / `char const*` / `nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): A string that should be checked.

Return:

* (`bool`) `true` if the given path is relative.


### nt_file_exists

Include: [nt/files.h](../include/nt/files.h)

Parameters:

* path (`char*` / `char const*` / `nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): A string that should be checked.

Return:

* (`bool`) `true` if the path exists and points to a file.


### nt_dir_exists

Include: [nt/files.h](../include/nt/files.h)

Parameters:

* path (`char*` / `char const*` / `nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): A string that should be checked.

Return:

* (`bool`) `true` if the path exists and points to a directory.


### nt_mkdir

Include: [nt/files.h](../include/nt/files.h)

Parameters:

* path (`char*` / `char const*` / `nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): A string holding the path where the
  directory should be created.

Return:

* (`bool`) `true` if the directory pointer by `path` did not exist and was successfully created.


### nt_mkdirs

Include: [nt/files.h](../include/nt/files.h)

Parameters:

* path (`char*` / `char const*` / `nt_cstr_t` / `nt_str_t*` / `nt_str_t const*`): A string holding the path where the
  directory should be created.

Return:

* (`bool`) `true` if the directory pointer by `path` was successfully created or did already exist.

Creates all directories if they do not already exist.


### nt_resolve

Include: [nt/files.h](../include/nt/files.h)

Parameters:

* path (`nt_str_t*`): A path that should be resolved to a full absolute path. The result will be stored in the same
  location on success.

Return:

* (`bool`) `true` if the path could be resolved.

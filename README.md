# ntutils

## Features

* Memory utility functions to reduce writing effort
* Advanced assert functions
* Test utility functions
* String type with short string optimization
* UTF-8 support
* Collection types

  * Linear list
  * Circular buffer
  * Queue (TODO)
  * Hash set
  * Hash map
  * Multi value hash map (TODO)

* JSON parser
* File system utility functions


## Restrictions

The following requirements must be met by the C compiler to compile this library:

* C11 support
* Support of the `__VA_OPT__` preprocessor statement
* Support for thread local storage


## Documentation

A detailed description of the library can be found in the following files:

* [docs/conventions.md](docs/conventions.md)

* [docs/containers.md](docs/containers.md)
* [docs/files.md](docs/files.md)
* [docs/memory.md](docs/memory.md)
* [docs/panics.md](docs/panics.md)
* [docs/streams.md](docs/streams.md)
* [docs/strs.md](docs/strs.md)
* [docs/tests.md](docs/tests.md)
* [docs/text.md](docs/text.md)

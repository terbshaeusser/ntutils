# Panic

A panic is a fatal error that can be raised anywhere in code. It cannot be caught. Instead registered cleanup handlers
are called, an error message is printed on the screen and the program terminates with an abnormal exit code.


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

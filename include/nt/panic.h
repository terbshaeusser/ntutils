#ifndef NTUTILS_PANIC_H
#define NTUTILS_PANIC_H

typedef void (*nt_panic_handler_t)(void *ctx);

void nt_push_panic_handler(nt_panic_handler_t handler, void *ctx);

void nt_pop_panic_handler();

void nt_panic(char const *message, ...);

#endif // NTUTILS_PANIC_H

#include "nt/panic.h"
#include "nt/config.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

typedef struct {
  nt_panic_handler_t handler;
  void *ctx;
} handler_entry_t;

static thread_local handler_entry_t entries[NT_PANIC_MAX_HANDLER_COUNT];
static thread_local int entry_count = 0;

void nt_push_panic_handler(nt_panic_handler_t handler, void *ctx) {
  assert(entry_count < NT_PANIC_MAX_HANDLER_COUNT);

  handler_entry_t *entry = &entries[entry_count];
  ++entry_count;

  entry->handler = handler;
  entry->ctx = ctx;
}

void nt_pop_panic_handler() {
  assert(entry_count > 0);

  --entry_count;
}

void nt_panic(char const *message, ...) {
  while (entry_count > 0) {
    handler_entry_t *entry = &entries[entry_count - 1];
    --entry_count;

    entry->handler(entry->ctx);
  }

  printf("A panic caused the program to abort:\n");
  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  exit(NT_PANIC_EXIT_CODE);
}

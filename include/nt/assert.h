#ifndef NTUTILS_ASSERT_H
#define NTUTILS_ASSERT_H

#include "nt/macros.h"
#include "nt/panic.h"
#include <stdlib.h>

#define nt_assert(condition, ...)                                              \
  P_nt_assert(condition, NT_GET_ARG0_OR_DEFAULT(NULL, __VA_ARGS__), NT_LOCATION)

#ifdef NT_DISABLE_ASSERTS
#define P_nt_assert(condition, message, location)
#else
#define P_nt_assert(condition, message, location)                              \
  if (!(condition))                                                            \
  nt_panic("Assertion failed at %s\n\n%s\n\n%s", location,                     \
           NT_STRINGIFY(condition), message ? message : "")
#endif

#endif // NTUTILS_ASSERT_H

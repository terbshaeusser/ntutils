#ifndef NTUTILS_ASSERT_H
#define NTUTILS_ASSERT_H

#include "nt/macros.h"
#include "nt/panic.h"
#include <stdlib.h>

#define nt_assert(condition, ...)                                              \
  P_nt_assert(condition, NT_GET_ARG0_OR_DEFAULT("", __VA_ARGS__),              \
              NT_GET_ARG1_TO_N(__VA_ARGS__))

#define P_nt_assert(condition, message, ...)                                   \
  P_nt_assert2(condition, message, NT_LOCATION __VA_OPT__(, ) __VA_ARGS__)

#ifdef NT_DISABLE_ASSERTS
#define P_nt_assert2(condition, message, location)
#else
#define P_nt_assert2(condition, message, location, ...)                        \
  if (!(condition))                                                            \
  nt_panic("Assertion failed at " location "\n\n%s\n\n" message,               \
           NT_STRINGIFY(condition) __VA_OPT__(, ) __VA_ARGS__)
#endif

#endif // NTUTILS_ASSERT_H

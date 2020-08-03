#ifndef NTUTILS_TEST_H
#define NTUTILS_TEST_H

#include "nt/compare.h"
#include "nt/macros.h"
#include "nt/measurement.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define nt_test(func)                                                          \
  {                                                                            \
    printf("Executing %s ... ", #func);                                        \
    int64_t time;                                                              \
    nt_measure(func(), time);                                                  \
    printf("OK (");                                                            \
    nt_print_measure(time);                                                    \
  }                                                                            \
  printf(")\n")

#define nt_assert_equal(expected, actual, ...)                                 \
  _Generic(expected, int8_t                                                    \
           : P_nt_assert_equal_int, int16_t                                    \
           : P_nt_assert_equal_int, int32_t                                    \
           : P_nt_assert_equal_int, int64_t                                    \
           : P_nt_assert_equal_int, uint8_t                                    \
           : P_nt_assert_equal_uint, uint16_t                                  \
           : P_nt_assert_equal_uint, uint32_t                                  \
           : P_nt_assert_equal_uint, uint64_t                                  \
           : P_nt_assert_equal_uint, float                                     \
           : P_nt_assert_equal_float, double                                   \
           : P_nt_assert_equal_float, char*                                    \
           : P_nt_assert_equal_str, char const*                                \
           : P_nt_assert_equal_str, default                                    \
           : P_nt_assert_equal_any)(expected, actual, #expected, #actual,      \
                                    NT_LOCATION __VA_OPT__(, (void*)           \
                                        NT_GET_ARG0(__VA_ARGS__)))

void P_nt_assert_equal_int(int64_t expected, int64_t actual,
                           char const *expected_str, char const *actual_str,
                           char const *location);

void P_nt_assert_equal_uint(uint64_t expected, uint64_t actual,
                            char const *expected_str, char const *actual_str,
                            char const *location);

void P_nt_assert_equal_float(double expected, double actual,
                             char const *expected_str, char const *actual_str,
                             char const *location);

void P_nt_assert_equal_str(char const *expected, char const *actual,
                           char const *expected_str, char const *actual_str,
                           char const *location);

void P_nt_assert_equal_any(void const *expected, void const *actual,
                           char const *expected_str, char const *actual_str,
                           char const *location,
                           nt_comparator_t(comparator, void const *));

#define nt_assert_unequal(expected, actual, ...)                               \
  _Generic(expected, int8_t                                                    \
           : P_nt_assert_unequal_int, int16_t                                  \
           : P_nt_assert_unequal_int, int32_t                                  \
           : P_nt_assert_unequal_int, int64_t                                  \
           : P_nt_assert_unequal_int, uint8_t                                  \
           : P_nt_assert_unequal_uint, uint16_t                                \
           : P_nt_assert_unequal_uint, uint32_t                                \
           : P_nt_assert_unequal_uint, uint64_t                                \
           : P_nt_assert_unequal_uint, float                                   \
           : P_nt_assert_unequal_float, double                                 \
           : P_nt_assert_unequal_float, char*                                  \
           : P_nt_assert_unequal_str, char const*                              \
           : P_nt_assert_unequal_any)(expected, actual, #expected, #actual,    \
                                      NT_LOCATION __VA_OPT__(, (void*)         \
                                        NT_GET_ARG0(__VA_ARGS__)))

void P_nt_assert_unequal_int(int64_t expected, int64_t actual,
                             char const *expected_str, char const *actual_str,
                             char const *location);

void P_nt_assert_unequal_uint(uint64_t expected, uint64_t actual,
                              char const *expected_str, char const *actual_str,
                              char const *location);

void P_nt_assert_unequal_float(double expected, double actual,
                               char const *expected_str, char const *actual_str,
                               char const *location);

void P_nt_assert_unequal_str(char const *expected, char const *actual,
                             char const *expected_str, char const *actual_str,
                             char const *location);

void P_nt_assert_unequal_any(void const *expected, void const *actual,
                             char const *expected_str, char const *actual_str,
                             char const *location,
                             nt_comparator_t(comparator, void const *));

#define nt_assert_less(expected, actual)                                       \
  _Generic(expected, int8_t                                                    \
           : P_nt_assert_less_int, int16_t                                     \
           : P_nt_assert_less_int, int32_t                                     \
           : P_nt_assert_less_int, int64_t                                     \
           : P_nt_assert_less_int, uint8_t                                     \
           : P_nt_assert_less_uint, uint16_t                                   \
           : P_nt_assert_less_uint, uint32_t                                   \
           : P_nt_assert_less_uint, uint64_t                                   \
           : P_nt_assert_less_uint, float                                      \
           : P_nt_assert_less_float, double                                    \
           : P_nt_assert_less_float)(expected, actual, #expected, #actual,     \
                                     NT_LOCATION)

void P_nt_assert_less_int(int64_t expected, int64_t actual,
                          char const *expected_str, char const *actual_str,
                          char const *location);

void P_nt_assert_less_uint(uint64_t expected, uint64_t actual,
                           char const *expected_str, char const *actual_str,
                           char const *location);

void P_nt_assert_less_float(double expected, double actual,
                            char const *expected_str, char const *actual_str,
                            char const *location);

#define nt_assert_less_equal(expected, actual)                                 \
  _Generic(expected, int8_t                                                    \
           : P_nt_assert_less_equal_int, int16_t                               \
           : P_nt_assert_less_equal_int, int32_t                               \
           : P_nt_assert_less_equal_int, int64_t                               \
           : P_nt_assert_less_equal_int, uint8_t                               \
           : P_nt_assert_less_equal_uint, uint16_t                             \
           : P_nt_assert_less_equal_uint, uint32_t                             \
           : P_nt_assert_less_equal_uint, uint64_t                             \
           : P_nt_assert_less_equal_uint, float                                \
           : P_nt_assert_less_equal_float, double                              \
           : P_nt_assert_less_equal_float)(expected, actual, #expected,        \
                                           #actual, NT_LOCATION)

void P_nt_assert_less_equal_int(int64_t expected, int64_t actual,
                                char const *expected_str,
                                char const *actual_str, char const *location);

void P_nt_assert_less_equal_uint(uint64_t expected, uint64_t actual,
                                 char const *expected_str,
                                 char const *actual_str, char const *location);

void P_nt_assert_less_equal_float(double expected, double actual,
                                  char const *expected_str,
                                  char const *actual_str, char const *location);

#define nt_assert_greater(expected, actual)                                    \
  _Generic(expected, int8_t                                                    \
           : P_nt_assert_greater_int, int16_t                                  \
           : P_nt_assert_greater_int, int32_t                                  \
           : P_nt_assert_greater_int, int64_t                                  \
           : P_nt_assert_greater_int, uint8_t                                  \
           : P_nt_assert_greater_uint, uint16_t                                \
           : P_nt_assert_greater_uint, uint32_t                                \
           : P_nt_assert_greater_uint, uint64_t                                \
           : P_nt_assert_greater_uint, float                                   \
           : P_nt_assert_greater_float, double                                 \
           : P_nt_assert_greater_float)(expected, actual, #expected, #actual,  \
                                        NT_LOCATION)

void P_nt_assert_greater_int(int64_t expected, int64_t actual,
                             char const *expected_str, char const *actual_str,
                             char const *location);

void P_nt_assert_greater_uint(uint64_t expected, uint64_t actual,
                              char const *expected_str, char const *actual_str,
                              char const *location);

void P_nt_assert_greater_float(double expected, double actual,
                               char const *expected_str, char const *actual_str,
                               char const *location);

#define nt_assert_greater_equal(expected, actual)                              \
  _Generic(expected, int8_t                                                    \
           : P_nt_assert_greater_equal_int, int16_t                            \
           : P_nt_assert_greater_equal_int, int32_t                            \
           : P_nt_assert_greater_equal_int, int64_t                            \
           : P_nt_assert_greater_equal_int, uint8_t                            \
           : P_nt_assert_greater_equal_uint, uint16_t                          \
           : P_nt_assert_greater_equal_uint, uint32_t                          \
           : P_nt_assert_greater_equal_uint, uint64_t                          \
           : P_nt_assert_greater_equal_uint, float                             \
           : P_nt_assert_greater_equal_float, double                           \
           : P_nt_assert_greater_equal_float)(expected, actual, #expected,     \
                                              #actual, NT_LOCATION)

void P_nt_assert_greater_equal_int(int64_t expected, int64_t actual,
                                   char const *expected_str,
                                   char const *actual_str,
                                   char const *location);

void P_nt_assert_greater_equal_uint(uint64_t expected, uint64_t actual,
                                    char const *expected_str,
                                    char const *actual_str,
                                    char const *location);

void P_nt_assert_greater_equal_float(double expected, double actual,
                                     char const *expected_str,
                                     char const *actual_str,
                                     char const *location);

#define nt_assert_true(actual) P_nt_assert_true(actual, #actual, NT_LOCATION)

void P_nt_assert_true(bool actual, char const *actual_str,
                      char const *location);

#define nt_assert_false(actual) P_nt_assert_false(actual, #actual, NT_LOCATION)

void P_nt_assert_false(bool actual, char const *actual_str,
                       char const *location);

#define nt_assert_null(actual) P_nt_assert_null(actual, #actual, NT_LOCATION)

void P_nt_assert_null(void const *actual, char const *actual_str,
                      char const *location);

#define nt_assert_not_null(actual)                                             \
  P_nt_assert_not_null(actual, #actual, NT_LOCATION)

void P_nt_assert_not_null(void const *actual, char const *actual_str,
                          char const *location);

#endif // NTUTILS_TEST_H

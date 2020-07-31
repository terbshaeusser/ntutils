#include "nt/test.h"
#include "nt/panic.h"
#include <inttypes.h>
#include <string.h>

void P_nt_assert_equal_int(int64_t expected, int64_t actual,
                           char const *expected_str, char const *actual_str,
                           char const *location) {
  if (expected != actual) {
    nt_panic("Assertion failed at %s\n\n%s == %s\n\nExpected: %" PRId64
             "\nActual:   %" PRId64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_equal_uint(uint64_t expected, uint64_t actual,
                            char const *expected_str, char const *actual_str,
                            char const *location) {
  if (expected != actual) {
    nt_panic("Assertion failed at %s\n\n%s == %s\n\nExpected: %" PRIu64
             "\nActual:   %" PRIu64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_equal_float(double expected, double actual,
                             char const *expected_str, char const *actual_str,
                             char const *location) {
  if (expected != actual) {
    nt_panic(
        "Assertion failed at %s\n\n%s == %s\n\nExpected: %f\nActual:   %f\n",
        location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_equal_str(char const *expected, char const *actual,
                           char const *expected_str, char const *actual_str,
                           char const *location) {
  if (strcmp(expected, actual) != 0) {
    nt_panic(
        "Assertion failed at %s\n\n%s == %s\n\nExpected: %s\nActual:   %s\n",
        location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_equal_any(void const *expected, void const *actual,
                           char const *expected_str, char const *actual_str,
                           char const *location,
                           nt_comparator_t(comparator, void const *)) {
  if (comparator(expected, actual) != NT_COMPARE_EQUAL) {
    nt_panic("Assertion failed at %s\n\n%s == %s\n\n", location, expected_str,
             actual_str);
  }
}

void P_nt_assert_unequal_int(int64_t expected, int64_t actual,
                             char const *expected_str, char const *actual_str,
                             char const *location) {
  if (expected == actual) {
    nt_panic("Assertion failed at %s\n\n%s != %s\n\nExpected: %" PRId64
             "\nActual:   %" PRId64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_unequal_uint(uint64_t expected, uint64_t actual,
                              char const *expected_str, char const *actual_str,
                              char const *location) {
  if (expected == actual) {
    nt_panic("Assertion failed at %s\n\n%s != %s\n\nExpected: %" PRIu64
             "\nActual:   %" PRIu64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_unequal_float(double expected, double actual,
                               char const *expected_str, char const *actual_str,
                               char const *location) {
  if (expected == actual) {
    nt_panic(
        "Assertion failed at %s\n\n%s != %s\n\nExpected: %f\nActual:   %f\n",
        location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_unequal_str(char const *expected, char const *actual,
                             char const *expected_str, char const *actual_str,
                             char const *location) {
  if (strcmp(expected, actual) == 0) {
    nt_panic(
        "Assertion failed at %s\n\n%s != %s\n\nExpected: %s\nActual:   %s\n",
        location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_unequal_any(void const *expected, void const *actual,
                             char const *expected_str, char const *actual_str,
                             char const *location,
                             nt_comparator_t(comparator, void const *)) {
  if (comparator(expected, actual) == NT_COMPARE_EQUAL) {
    nt_panic("Assertion failed at %s\n\n%s != %s\n\n", location, expected_str,
             actual_str);
  }
}

void P_nt_assert_less_int(int64_t expected, int64_t actual,
                          char const *expected_str, char const *actual_str,
                          char const *location) {
  if (expected <= actual) {
    nt_panic("Assertion failed at %s\n\n%s > %s\n\nExpected: %" PRId64
             "\nActual:   %" PRId64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_less_uint(uint64_t expected, uint64_t actual,
                           char const *expected_str, char const *actual_str,
                           char const *location) {
  if (expected <= actual) {
    nt_panic("Assertion failed at %s\n\n%s > %s\n\nExpected: %" PRIu64
             "\nActual:   %" PRIu64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_less_float(double expected, double actual,
                            char const *expected_str, char const *actual_str,
                            char const *location) {
  if (expected <= actual) {
    nt_panic(
        "Assertion failed at %s\n\n%s > %s\n\nExpected: %f\nActual:   %f\n",
        location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_less_equal_int(int64_t expected, int64_t actual,
                                char const *expected_str,
                                char const *actual_str, char const *location) {
  if (expected < actual) {
    nt_panic("Assertion failed at %s\n\n%s >= %s\n\nExpected: %" PRId64
             "\nActual:   %" PRId64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_less_equal_uint(uint64_t expected, uint64_t actual,
                                 char const *expected_str,
                                 char const *actual_str, char const *location) {
  if (expected < actual) {
    nt_panic("Assertion failed at %s\n\n%s >= %s\n\nExpected: %" PRIu64
             "\nActual:   %" PRIu64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_less_equal_float(double expected, double actual,
                                  char const *expected_str,
                                  char const *actual_str,
                                  char const *location) {
  if (expected < actual) {
    nt_panic(
        "Assertion failed at %s\n\n%s >= %s\n\nExpected: %f\nActual:   %f\n",
        location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_greater_int(int64_t expected, int64_t actual,
                             char const *expected_str, char const *actual_str,
                             char const *location) {
  if (expected >= actual) {
    nt_panic("Assertion failed at %s\n\n%s < %s\n\nExpected: %" PRId64
             "\nActual:   %" PRId64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_greater_uint(uint64_t expected, uint64_t actual,
                              char const *expected_str, char const *actual_str,
                              char const *location) {
  if (expected >= actual) {
    nt_panic("Assertion failed at %s\n\n%s < %s\n\nExpected: %" PRIu64
             "\nActual:   %" PRIu64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_greater_float(double expected, double actual,
                               char const *expected_str, char const *actual_str,
                               char const *location) {
  if (expected >= actual) {
    nt_panic(
        "Assertion failed at %s\n\n%s < %s\n\nExpected: %f\nActual:   %f\n",
        location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_greater_equal_int(int64_t expected, int64_t actual,
                                   char const *expected_str,
                                   char const *actual_str,
                                   char const *location) {
  if (expected > actual) {
    nt_panic("Assertion failed at %s\n\n%s <= %s\n\nExpected: %" PRId64
             "\nActual:   %" PRId64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_greater_equal_uint(uint64_t expected, uint64_t actual,
                                    char const *expected_str,
                                    char const *actual_str,
                                    char const *location) {
  if (expected > actual) {
    nt_panic("Assertion failed at %s\n\n%s <= %s\n\nExpected: %" PRIu64
             "\nActual:   %" PRIu64 "\n",
             location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_greater_equal_float(double expected, double actual,
                                     char const *expected_str,
                                     char const *actual_str,
                                     char const *location) {
  if (expected > actual) {
    nt_panic(
        "Assertion failed at %s\n\n%s <= %s\n\nExpected: %f\nActual:   %f\n",
        location, expected_str, actual_str, expected, actual);
  }
}

void P_nt_assert_true(bool actual, char const *actual_str,
                      char const *location) {
  if (!actual) {
    nt_panic("Assertion failed at %s\n\n%s == true\n\n", location, actual_str);
  }
}

void P_nt_assert_false(bool actual, char const *actual_str,
                       char const *location) {
  if (actual) {
    nt_panic("Assertion failed at %s\n\n%s == false\n\n", location, actual_str);
  }
}

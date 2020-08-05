#include "nt/set.h"
#include "nt/test.h"

NT_SET(int_set, int)

static nt_comparison_result_t compare_ints(int const *a, int const *b) {
  if (*a == *b) {
    return NT_COMPARE_EQUAL;
  }

  if (*a < *b) {
    return NT_COMPARE_ASC;
  }

  return NT_COMPARE_DESC;
}

static void test_add() {
  int_set_t set = int_set_new(nt_murmur3_hasher, compare_ints);

  nt_assert_equal(0, int_set_count(&set));

  for (int i = 0; i < 1000; ++i) {
    nt_assert_true(int_set_add(&set, i));
    nt_assert_equal(i + 1, int_set_count(&set));
    nt_assert_true(int_set_contains(&set, i));
  }
  nt_assert_false(int_set_add(&set, 1));

  int_set_free(&set);
}

static void test_remove() {
  int_set_t set = int_set_new(nt_murmur3_hasher, compare_ints);

  nt_assert_equal(0, int_set_count(&set));
  for (int i = 0; i < 3; ++i) {
    nt_assert_true(int_set_add(&set, i));
    nt_assert_true(int_set_contains(&set, i));
  }

  nt_assert_true(int_set_remove(&set, 1));
  nt_assert_false(int_set_remove(&set, 1));
  nt_assert_true(int_set_contains(&set, 0));
  nt_assert_true(int_set_contains(&set, 2));

  int_set_free(&set);
}

static void test_clear() {
  int_set_t set = int_set_new(nt_murmur3_hasher, compare_ints);

  for (int i = 0; i < 1000; ++i) {
    nt_assert_true(int_set_add(&set, i));
    nt_assert_true(int_set_contains(&set, i));
  }

  int_set_clear(&set);

  for (int i = 0; i < 1000; ++i) {
    nt_assert_false(int_set_contains(&set, i));
  }

  nt_assert_equal(0, int_set_count(&set));

  int_set_free(&set);
}

static void test_iterator() {
  int_set_t set = int_set_new(nt_murmur3_hasher, compare_ints);

  for (int i = 0; i < 1000; ++i) {
    nt_assert_true(int_set_add(&set, i));
    nt_assert_true(int_set_contains(&set, i));
  }

  for (int i = 50; i < 100; ++i) {
    nt_assert_true(int_set_remove(&set, i));
    nt_assert_false(int_set_contains(&set, i));
  }

  int_set_iterator_t itr = int_set_iterate(&set, NULL);
  for (int i = 0; i < 1000; ++i) {
    if (i >= 50 && i < 100) {
      continue;
    }

    int const *key = int_set_iterator_next(&itr);
    nt_assert_not_null(key);
    nt_assert_less(1000, *key);
    nt_assert_greater_equal(0, *key);
    nt_assert_false(*key >= 50 && *key < 100);
  }

  nt_assert_null(int_set_iterator_next(&itr));

  int_set_free(&set);
}

int main() {
  nt_test(test_add);
  nt_test(test_remove);
  nt_test(test_clear);
  nt_test(test_iterator);

  return 0;
}

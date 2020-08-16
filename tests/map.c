#include "nt/map.h"
#include "nt/test.h"

NT_MAP(int_map, int, char const *)

enum { NUMBERS_COUNT = 10 };

static char const *const numbers[NUMBERS_COUNT] = {"0", "1", "2", "3", "4",
                                                   "5", "6", "7", "8", "9"};

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
  int_map_t map = int_map_new(nt_murmur3_hasher, compare_ints);

  nt_assert_equal(0, int_map_count(&map));

  for (int i = 0; i < NUMBERS_COUNT; ++i) {
    nt_assert_true(int_map_add(&map, i, numbers[i]));
    nt_assert_equal(i + 1, int_map_count(&map));
    nt_assert_true(int_map_contains(&map, i));
    char const *const *value = int_map_get(&map, i);
    nt_assert_not_null(value);
    nt_assert_equal(numbers[i], *value);
  }

  nt_assert_false(int_map_add(&map, 0, numbers[0]));

  int_map_free(&map);
}

static void test_add_multi() {
  int_map_t map = int_map_new(nt_murmur3_hasher, compare_ints);

  nt_assert_equal(0, int_map_count(&map));

  for (int i = 0; i < NUMBERS_COUNT; ++i) {
    int_map_add_multi(&map, 0, numbers[i]);
    nt_assert_equal(i + 1, int_map_count(&map));
    nt_assert_true(int_map_contains(&map, 0));
    char const *const *value = int_map_get(&map, 0);
    nt_assert_not_null(value);
    nt_assert_equal(numbers[i], *value);
  }

  int_map_add_multi(&map, 1, numbers[1]);
  nt_assert_equal(NUMBERS_COUNT + 1, int_map_count(&map));
  nt_assert_true(int_map_contains(&map, 1));

  int_map_remove(&map, 0);
  nt_assert_equal(NUMBERS_COUNT, int_map_count(&map));
  nt_assert_true(int_map_contains(&map, 0));

  int_map_iterator_t itr = int_map_iterate(&map, NULL);
  int_map_pair_t pair;

  for (int i = 0; i < NUMBERS_COUNT; ++i) {
    pair = int_map_iterator_next(&itr);
    nt_assert_not_null(pair.key);
    nt_assert_not_null(pair.value);
    nt_assert_true(*pair.key == 0 || *pair.key == 1);
  }

  nt_assert_null(int_map_iterator_next(&itr).key);

  int key = 0;
  itr = int_map_iterate(&map, &key);

  for (int i = 0; i < NUMBERS_COUNT - 1; ++i) {
    pair = int_map_iterator_next(&itr);
    nt_assert_not_null(pair.key);
    nt_assert_not_null(pair.value);
    nt_assert_equal(0, *pair.key);
  }

  nt_assert_null(int_map_iterator_next(&itr).key);

  int_map_free(&map);
}

static void test_put() {
  int_map_t map = int_map_new(nt_murmur3_hasher, compare_ints);

  nt_assert_equal(0, int_map_count(&map));
  int_map_put(&map, 1, numbers[1]);
  nt_assert_equal(1, int_map_count(&map));

  char const *const *value = int_map_get(&map, 1);
  nt_assert_not_null(value);
  nt_assert_equal(numbers[1], *value);

  int_map_put(&map, 1, numbers[2]);
  nt_assert_equal(1, int_map_count(&map));

  value = int_map_get(&map, 1);
  nt_assert_not_null(value);
  nt_assert_equal(numbers[2], *value);

  int_map_free(&map);
}

static void test_remove() {
  int_map_t map = int_map_new(nt_murmur3_hasher, compare_ints);

  for (int i = 0; i < NUMBERS_COUNT; ++i) {
    nt_assert_true(int_map_add(&map, i, numbers[i]));
  }

  nt_assert_true(int_map_remove(&map, 1));
  nt_assert_false(int_map_remove(&map, 1));
  nt_assert_true(int_map_contains(&map, 0));
  char const *const *value = int_map_get(&map, 0);
  nt_assert_not_null(value);
  nt_assert_equal(numbers[0], *value);
  nt_assert_true(int_map_contains(&map, 2));
  value = int_map_get(&map, 2);
  nt_assert_not_null(value);
  nt_assert_equal(numbers[2], *value);

  int_map_free(&map);
}

static void test_clear() {
  int_map_t map = int_map_new(nt_murmur3_hasher, compare_ints);

  for (int i = 0; i < NUMBERS_COUNT; ++i) {
    nt_assert_true(int_map_add(&map, i, numbers[i]));
  }

  int_map_clear(&map);

  for (int i = 0; i < NUMBERS_COUNT; ++i) {
    nt_assert_false(int_map_contains(&map, i));
  }

  nt_assert_equal(0, int_map_count(&map));

  int_map_free(&map);
}

static void test_iterator() {
  int_map_t map = int_map_new(nt_murmur3_hasher, compare_ints);

  for (int i = 0; i < NUMBERS_COUNT; ++i) {
    nt_assert_true(int_map_add(&map, i, numbers[i]));
  }

  nt_assert_true(int_map_remove(&map, 1));
  nt_assert_false(int_map_contains(&map, 1));

  int_map_iterator_t itr = int_map_iterate(&map, NULL);
  int_map_pair_t pair;

  for (int i = 0; i < NUMBERS_COUNT; ++i) {
    if (i == 1) {
      continue;
    }

    pair = int_map_iterator_next(&itr);
    nt_assert_not_null(pair.key);
    nt_assert_not_null(pair.value);
    nt_assert_unequal(1, *pair.key);
    nt_assert_unequal(numbers[1], *pair.value);
  }

  nt_assert_null(int_map_iterator_next(&itr).key);

  int key = 0;
  itr = int_map_iterate(&map, &key);

  pair = int_map_iterator_next(&itr);
  nt_assert_not_null(pair.key);
  nt_assert_not_null(pair.value);
  nt_assert_equal(0, *pair.key);
  nt_assert_equal(numbers[0], *pair.value);

  nt_assert_null(int_map_iterator_next(&itr).key);

  key = 1;
  itr = int_map_iterate(&map, &key);

  nt_assert_null(int_map_iterator_next(&itr).key);

  int_map_free(&map);
}

int main() {
  nt_test(test_add);
  nt_test(test_add_multi);
  nt_test(test_put);
  nt_test(test_remove);
  nt_test(test_clear);
  nt_test(test_iterator);

  return 0;
}

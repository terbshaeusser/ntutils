#include "nt/list.h"
#include "nt/test.h"

NT_LIST(int_list, int)

static void test_add() {
  int count = 10000;
  int_list_t list = int_list_new();

  for (int i = 0; i < count; ++i) {
    int_list_add(&list, i + 1);
  }

  nt_assert_equal(count, int_list_count(&list));

  for (int i = 0; i < count; ++i) {
    nt_assert_equal(i + 1, *int_list_get(&list, i));
  }

  int_list_free(&list);
}

static void test_add_reserve() {
  int count = 10000;
  int_list_t list = int_list_new();

  int_list_reserve(&list, count);
  for (int i = 0; i < count; ++i) {
    int_list_add(&list, i + 1);
  }

  nt_assert_equal(count, int_list_count(&list));

  for (int i = 0; i < count; ++i) {
    nt_assert_equal(i + 1, *int_list_get(&list, i));
  }

  int_list_free(&list);
}

static void test_insert() {
  int count = 10000;
  int_list_t list = int_list_new();

  for (int i = 0; i < count; ++i) {
    int_list_insert(&list, 0, i + 1);
  }

  nt_assert_equal(count, int_list_count(&list));

  for (int i = 0; i < count; ++i) {
    nt_assert_equal(count - i, *int_list_get(&list, i));
  }

  int_list_free(&list);
}

static void test_remove() {
  int_list_t list = int_list_new();

  for (int i = 0; i < 3; ++i) {
    int_list_add(&list, i + 1);
  }
  nt_assert_equal(3, int_list_count(&list));

  int_list_remove(&list, 1);
  nt_assert_equal(2, int_list_count(&list));
  nt_assert_equal(1, *int_list_get(&list, 0));
  nt_assert_equal(3, *int_list_get(&list, 1));

  int_list_remove(&list, 0);
  nt_assert_equal(1, int_list_count(&list));
  nt_assert_equal(3, *int_list_get(&list, 0));

  int_list_remove(&list, 0);
  nt_assert_equal(0, int_list_count(&list));

  int_list_free(&list);
}

int main() {
  nt_test(test_add);
  nt_test(test_add_reserve);
  nt_test(test_insert);
  nt_test(test_remove);

  return 0;
}

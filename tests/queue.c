#include "nt/queue.h"
#include "nt/test.h"

NT_QUEUE(int_queue, int)

static void test_add() {
  int_queue_t queue = int_queue_new();

  nt_assert_equal(0, int_queue_count(&queue));

  for (int i = 0; i < 1000; ++i) {
    int_queue_add(&queue, i);
    nt_assert_equal(i + 1, int_queue_count(&queue));
  }

  int_queue_free(&queue);
}

static void test_take() {
  int_queue_t queue = int_queue_new();

  nt_assert_equal(0, int_queue_count(&queue));

  for (int i = 0; i < 1000; ++i) {
    int_queue_add(&queue, i);
    nt_assert_equal(i + 1, int_queue_count(&queue));
  }

  for (int i = 0; i < 1000; ++i) {
    nt_assert_equal(i, int_queue_peek(&queue));
    nt_assert_equal(i, int_queue_take(&queue));
    nt_assert_equal(999 - i, int_queue_count(&queue));
  }

  int_queue_free(&queue);
}

static void test_clear() {
  int_queue_t queue = int_queue_new();

  nt_assert_equal(0, int_queue_count(&queue));

  for (int j = 0; j < 2; ++j) {
    for (int i = 0; i < 1000; ++i) {
      int_queue_add(&queue, i);
      nt_assert_equal(i + 1, int_queue_count(&queue));
    }

    int_queue_clear(&queue);
    nt_assert_equal(0, int_queue_count(&queue));
  }

  int_queue_free(&queue);
}

static void test_iterate() {
  int_queue_t queue = int_queue_new();

  nt_assert_equal(0, int_queue_count(&queue));

  for (int i = 0; i < 1000; ++i) {
    int_queue_add(&queue, i);
    nt_assert_equal(i + 1, int_queue_count(&queue));
  }

  for (int j = 0; j < 2; ++j) {
    int_queue_iterator_t itr = int_queue_iterate(&queue);
    for (int i = 0; i < 1000; ++i) {
      int const *ptr = int_queue_iterator_next(&itr);

      nt_assert_not_null(ptr);
      nt_assert_equal(i, *ptr);
    }
    nt_assert_null(int_queue_iterator_next(&itr));
  }

  int_queue_free(&queue);
}

int main() {
  nt_test(test_add);
  nt_test(test_take);
  nt_test(test_clear);
  nt_test(test_iterate);

  return 0;
}

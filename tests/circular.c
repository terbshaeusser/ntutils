#include "nt/circular.h"
#include "nt/test.h"

NT_CIRC_BUF(int_buffer, int)

static void test_push() {
  int_buffer_t buffer = int_buffer_new(4);

  int_buffer_push(&buffer, 1);
  nt_assert_equal(1, int_buffer_count(&buffer));
  nt_assert_equal(1, *int_buffer_get(&buffer, 0));

  int_buffer_push(&buffer, 2);
  nt_assert_equal(2, int_buffer_count(&buffer));
  nt_assert_equal(1, *int_buffer_get(&buffer, 0));
  nt_assert_equal(2, *int_buffer_get(&buffer, 1));

  int_buffer_push(&buffer, 3);
  nt_assert_equal(3, int_buffer_count(&buffer));
  nt_assert_equal(1, *int_buffer_get(&buffer, 0));
  nt_assert_equal(2, *int_buffer_get(&buffer, 1));
  nt_assert_equal(3, *int_buffer_get(&buffer, 2));

  int_buffer_push(&buffer, 4);
  nt_assert_equal(4, int_buffer_count(&buffer));
  nt_assert_equal(1, *int_buffer_get(&buffer, 0));
  nt_assert_equal(2, *int_buffer_get(&buffer, 1));
  nt_assert_equal(3, *int_buffer_get(&buffer, 2));
  nt_assert_equal(4, *int_buffer_get(&buffer, 3));

  int_buffer_free(&buffer);
}

static void test_pop() {
  int_buffer_t buffer = int_buffer_new(4);

  int_buffer_push(&buffer, 1);
  nt_assert_equal(1, int_buffer_count(&buffer));

  int item;
  int_buffer_pop(&buffer, &item);
  nt_assert_equal(1, item);
  nt_assert_equal(0, int_buffer_count(&buffer));

  int_buffer_push(&buffer, 2);
  int_buffer_push(&buffer, 3);
  nt_assert_equal(2, int_buffer_count(&buffer));

  int_buffer_pop(&buffer, &item);
  nt_assert_equal(2, item);
  nt_assert_equal(1, int_buffer_count(&buffer));

  int_buffer_push(&buffer, 4);
  int_buffer_push(&buffer, 5);
  int_buffer_push(&buffer, 6);
  nt_assert_equal(4, int_buffer_count(&buffer));
  nt_assert_equal(3, *int_buffer_get(&buffer, 0));
  nt_assert_equal(4, *int_buffer_get(&buffer, 1));
  nt_assert_equal(5, *int_buffer_get(&buffer, 2));
  nt_assert_equal(6, *int_buffer_get(&buffer, 3));

  int_buffer_free(&buffer);
}

typedef struct {
  int a;
  float b;
} some_struct_t;

NT_CIRC_BUF(some_struct_buffer, some_struct_t)

static void test_struct() {
  some_struct_buffer_t buffer = some_struct_buffer_new(2);

  some_struct_t some_struct = {22, 2.5f};
  some_struct_buffer_push(&buffer, some_struct);
  nt_assert_equal(1, some_struct_buffer_count(&buffer));
  nt_assert_equal(22, some_struct_buffer_get(&buffer, 0)->a);
  nt_assert_equal(2.5f, some_struct_buffer_get(&buffer, 0)->b);

  some_struct.a = 33;
  some_struct_buffer_push_ref(&buffer, &some_struct);
  nt_assert_equal(2, some_struct_buffer_count(&buffer));
  nt_assert_equal(22, some_struct_buffer_get(&buffer, 0)->a);
  nt_assert_equal(2.5f, some_struct_buffer_get(&buffer, 0)->b);
  nt_assert_equal(33, some_struct_buffer_get(&buffer, 1)->a);
  nt_assert_equal(2.5f, some_struct_buffer_get(&buffer, 1)->b);

  some_struct_buffer_free(&buffer);
}

static void test_clear() {
  int_buffer_t buffer = int_buffer_new(4);

  int_buffer_push(&buffer, 1);
  int_buffer_push(&buffer, 2);
  int_buffer_push(&buffer, 3);
  int_buffer_push(&buffer, 4);
  nt_assert_equal(4, int_buffer_count(&buffer));

  int_buffer_clear(&buffer);
  nt_assert_equal(0, int_buffer_count(&buffer));

  int_buffer_free(&buffer);
}

int main() {
  nt_test(test_push);
  nt_test(test_pop);
  nt_test(test_struct);
  nt_test(test_clear);

  return 0;
}

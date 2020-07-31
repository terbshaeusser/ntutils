#include "nt/mem.h"
#include "nt/test.h"

static void test_single_alloc() {
  int *ptr = nt_alloc(int);
  *ptr = 22;
  nt_free(ptr);
}

static void test_multi_alloc() {
  int *ptr = nt_alloc(int, 4);
  for (int i = 0; i < 4; ++i) {
    ptr[i] = i;
  }
  nt_free(ptr);
}

static void test_alloc_clean() {
  int *ptr = nt_alloc(int, 1024, true);
  for (int i = 0; i < 1024; ++i) {
    nt_assert_equal(0, ptr[i]);
  }
  nt_free(ptr);
}

int main() {
  nt_test(test_single_alloc);
  nt_test(test_multi_alloc);
  nt_test(test_alloc_clean);

  return 0;
}

#include "nt/str.h"
#include "nt/test.h"

static void test_short() {
  nt_str_t str = nt_str_new(nt_cstr("Hello!"));
  nt_assert_equal(6, nt_str_len(&str));
  nt_assert_equal("Hello!", nt_str_str(&str));
  nt_assert_true(((char const *)&str) == nt_str_str(&str));

  nt_str_t str2 = nt_str_new(&str);
  nt_assert_equal(6, nt_str_len(&str2));
  nt_assert_equal("Hello!", nt_str_str(&str2));
  nt_assert_true(((char const *)&str2) == nt_str_str(&str2));

  nt_str_free(&str2);
  nt_str_free(&str);
}

static void test_long() {
  nt_str_t str = nt_str_new(nt_cstr("This is a long string that won't fit into "
                                    "a string without heap allocation."));
  nt_assert_equal(75, nt_str_len(&str));
  nt_assert_equal("This is a long string that won't fit into a string without "
                  "heap allocation.",
                  nt_str_str(&str));
  nt_assert_false(((char const *)&str) == nt_str_str(&str));

  nt_str_t str2 = nt_str_new(&str);
  nt_assert_equal(75, nt_str_len(&str2));
  nt_assert_equal("This is a long string that won't fit into a string without "
                  "heap allocation.",
                  nt_str_str(&str2));
  nt_assert_false(((char const *)&str2) == nt_str_str(&str2));

  nt_str_free(&str2);
  nt_str_free(&str);
}

static void test_empty() {
  nt_str_t str = nt_str_empty();
  nt_assert_equal(0, nt_str_len(&str));
  nt_assert_equal("", nt_str_str(&str));
  nt_assert_true(((char const *)&str) == nt_str_str(&str));

  nt_str_t str2 = nt_str_new(&str);
  nt_assert_equal(0, nt_str_len(&str2));
  nt_assert_equal("", nt_str_str(&str2));
  nt_assert_true(((char const *)&str2) == nt_str_str(&str2));

  nt_str_t str3 = nt_str_new(nt_cstr(""));
  nt_assert_equal(0, nt_str_len(&str3));
  nt_assert_equal("", nt_str_str(&str3));
  nt_assert_true(((char const *)&str3) == nt_str_str(&str3));

  nt_str_free(&str3);
  nt_str_free(&str2);
  nt_str_free(&str);
}

static void test_str_of() {
  nt_str_t str = nt_str_of(nt_cstr("a"), 3);
  nt_assert_equal(3, nt_str_len(&str));
  nt_assert_equal("aaa", nt_str_str(&str));

  nt_str_t str2 = nt_str_of(nt_cstr("a"), 0);
  nt_assert_equal(0, nt_str_len(&str2));
  nt_assert_equal("", nt_str_str(&str2));

  nt_str_t str3 = nt_str_of(nt_cstr(""), 3);
  nt_assert_equal(0, nt_str_len(&str3));
  nt_assert_equal("", nt_str_str(&str3));
  nt_assert_true(((char const *)&str3) == nt_str_str(&str3));

  nt_str_free(&str3);
  nt_str_free(&str2);
  nt_str_free(&str);
}

static void test_equal() {
  nt_str_t str = nt_str_new(nt_cstr("Hello!"));
  nt_str_t str2 = nt_str_new(&str);
  nt_str_t str3 = nt_str_new(nt_cstr("Hello"));
  nt_str_t str4 = nt_str_new(nt_cstr("Hello!a"));

  nt_assert_true(nt_str_equal(&str, &str2));
  nt_assert_true(nt_str_equal(&str, nt_cstr("Hello!")));
  nt_assert_false(nt_str_equal(&str, &str3));
  nt_assert_false(nt_str_equal(&str, &str4));

  nt_str_free(&str4);
  nt_str_free(&str3);
  nt_str_free(&str2);
  nt_str_free(&str);
}

static void test_shrink() {
  nt_str_t short_str = nt_str_new(nt_cstr("Hello!"));
  nt_assert_true(((char const *)&short_str) == nt_str_str(&short_str));
  nt_str_shrink(&short_str);
  nt_assert_true(((char const *)&short_str) == nt_str_str(&short_str));

  nt_str_t long_str =
      nt_str_new(nt_cstr("This is a long string that won't fit into a string "
                         "without heap allocation."));
  nt_assert_false(((char const *)&long_str) == nt_str_str(&long_str));
  nt_str_shrink(&long_str);
  nt_assert_false(((char const *)&long_str) == nt_str_str(&long_str));

  nt_str_reserve(&short_str, 50);
  nt_assert_false(((char const *)&short_str) == nt_str_str(&short_str));
  nt_str_shrink(&short_str);
  nt_assert_true(((char const *)&short_str) == nt_str_str(&short_str));

  nt_str_free(&long_str);
  nt_str_free(&short_str);
}

static void test_append() {
  nt_str_t str = nt_str_new(nt_cstr("Hello"));
  nt_str_t str2 = nt_str_new(nt_cstr("World!"));

  nt_str_append(&str, nt_cstr(", "));
  nt_str_append(&str, &str2);
  nt_assert_equal(13, nt_str_len(&str));
  nt_assert_equal("Hello, World!", nt_str_str(&str));

  nt_str_append(&str, 1);
  nt_str_append(&str, 100u);
  nt_assert_equal(17, nt_str_len(&str));
  nt_assert_equal("Hello, World!1100", nt_str_str(&str));

  nt_str_append(&str, 1.1f);
  nt_str_append(&str, 2.1);
  nt_assert_equal(23, nt_str_len(&str));
  nt_assert_equal("Hello, World!11001.12.1", nt_str_str(&str));

  nt_str_free(&str2);
  nt_str_free(&str);
}

static void test_format() {
  nt_str_t str = nt_str_format("%d%s", 1, "a");
  nt_assert_equal(2, nt_str_len(&str));
  nt_assert_equal("1a", nt_str_str(&str));

  nt_str_append_format(&str, "%s", "!");
  nt_assert_equal(3, nt_str_len(&str));
  nt_assert_equal("1a!", nt_str_str(&str));

  nt_str_free(&str);
}

static void test_find() {
  nt_str_t str = nt_str_new(nt_cstr("Hello World!"));

  size_t index = nt_str_find(&str, nt_cstr("Wor"));
  nt_assert_equal(6, index);

  index = nt_str_find(&str, nt_cstr("War"));
  nt_assert_equal(NT_STR_NOT_FOUND, index);

  index = nt_str_find(&str, nt_cstr("o"), 5);
  nt_assert_equal(7, index);

  index = nt_str_find(&str, nt_cstr("o"), 7);
  nt_assert_equal(7, index);

  index = nt_str_find(&str, nt_cstr("o"), 8);
  nt_assert_equal(NT_STR_NOT_FOUND, index);

  nt_str_free(&str);
}

static void test_rfind() {
  nt_str_t str = nt_str_new(nt_cstr("Hello World!"));

  size_t index = nt_str_rfind(&str, nt_cstr("Wor"));
  nt_assert_equal(6, index);

  index = nt_str_rfind(&str, nt_cstr("War"));
  nt_assert_equal(NT_STR_NOT_FOUND, index);

  index = nt_str_rfind(&str, nt_cstr("o"), 8);
  nt_assert_equal(7, index);

  index = nt_str_rfind(&str, nt_cstr("o"), 7);
  nt_assert_equal(7, index);

  index = nt_str_rfind(&str, nt_cstr("o"), 6);
  nt_assert_equal(4, index);

  index = nt_str_rfind(&str, nt_cstr("o"), 3);
  nt_assert_equal(NT_STR_NOT_FOUND, index);

  nt_str_t str2 = nt_str_new(nt_cstr("Hellor World!"));

  index = nt_str_rfind(&str2, nt_cstr("or"));
  nt_assert_equal(8, index);

  index = nt_str_rfind(&str2, nt_cstr("or"), 8);
  nt_assert_equal(8, index);

  index = nt_str_rfind(&str2, nt_cstr("or"), 7);
  nt_assert_equal(4, index);

  nt_str_free(&str2);
  nt_str_free(&str);
}

int main() {
  nt_test(test_short);
  nt_test(test_long);
  nt_test(test_empty);
  nt_test(test_str_of);
  nt_test(test_equal);
  nt_test(test_shrink);
  nt_test(test_append);
  nt_test(test_format);
  nt_test(test_find);
  nt_test(test_rfind);

  return 0;
}

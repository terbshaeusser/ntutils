#include "nt/reader.h"
#include "nt/test.h"

static void test_empty() {
  nt_stream_t stream;
  nt_stream_from_str(&stream, "");
  nt_reader_t reader = nt_reader_new(&stream);
  uint32_t line;
  uint32_t column;

  nt_assert_true(nt_reader_is_eof(&reader));
  nt_assert_false(nt_cp_is_valid(nt_reader_lh(&reader, 1, &line, &column)));
  nt_assert_equal(1, line);
  nt_assert_equal(1, column);
  nt_assert_false(nt_cp_is_valid(nt_reader_next(&reader, &line, &column)));
  nt_assert_equal(1, line);
  nt_assert_equal(1, column);
  nt_assert_false(nt_cp_is_valid(nt_reader_lh(&reader, 1, &line, &column)));
  nt_assert_equal(1, line);
  nt_assert_equal(1, column);

  nt_reader_free(&reader);
  nt_stream_free(&stream);
}

static void test_simple() {
  nt_stream_t stream;
  nt_stream_from_str(&stream, "abc");
  nt_reader_t reader = nt_reader_new(&stream);
  uint32_t line;
  uint32_t column;

  nt_assert_false(nt_reader_is_eof(&reader));
  nt_assert_equal('a', nt_reader_next(&reader, &line, &column).code_point);
  nt_assert_equal(1, line);
  nt_assert_equal(1, column);

  nt_assert_false(nt_reader_is_eof(&reader));
  nt_assert_equal('b', nt_reader_next(&reader, &line, &column).code_point);
  nt_assert_equal(1, line);
  nt_assert_equal(2, column);

  nt_assert_false(nt_reader_is_eof(&reader));
  nt_assert_equal('c', nt_reader_next(&reader, &line, &column).code_point);
  nt_assert_equal(1, line);
  nt_assert_equal(3, column);

  nt_assert_true(nt_reader_is_eof(&reader));
  nt_assert_false(nt_cp_is_valid(nt_reader_next(&reader, &line, &column)));
  nt_assert_equal(1, line);
  nt_assert_equal(4, column);

  nt_reader_free(&reader);
  nt_stream_free(&stream);
}

static void test_utf8() {
  nt_stream_t stream;
  nt_stream_from_str(&stream, "y"
                              "\xC3"
                              "\xA4"
                              "\xE2"
                              "\x82"
                              "\xAC"
                              "\xF0"
                              "\x9D"
                              "\x84"
                              "\x9E");
  nt_reader_t reader = nt_reader_new(&stream);
  uint32_t line;
  uint32_t column;

  nt_assert_false(nt_reader_is_eof(&reader));
  nt_assert_equal('y', nt_reader_next(&reader, &line, &column).code_point);
  nt_assert_equal(1, line);
  nt_assert_equal(1, column);

  nt_assert_false(nt_reader_is_eof(&reader));
  nt_assert_equal(0xE4u, nt_reader_next(&reader, &line, &column).code_point);
  nt_assert_equal(1, line);
  nt_assert_equal(2, column);

  nt_assert_false(nt_reader_is_eof(&reader));
  nt_assert_equal(0x20ACu, nt_reader_next(&reader, &line, &column).code_point);
  nt_assert_equal(1, line);
  nt_assert_equal(3, column);

  nt_assert_false(nt_reader_is_eof(&reader));
  nt_assert_equal(0x1D11Eu, nt_reader_next(&reader, &line, &column).code_point);
  nt_assert_equal(1, line);
  nt_assert_equal(4, column);

  nt_assert_true(nt_reader_is_eof(&reader));
  nt_assert_false(nt_cp_is_valid(nt_reader_next(&reader, &line, &column)));
  nt_assert_equal(1, line);
  nt_assert_equal(5, column);

  nt_reader_free(&reader);
  nt_stream_free(&stream);
}

static void test_lookahead() {
  nt_stream_t stream;
  nt_stream_from_str(&stream, "abc");
  nt_reader_t reader = nt_reader_new(&stream);
  uint32_t line;
  uint32_t column;

  nt_assert_false(nt_reader_is_eof(&reader));
  nt_assert_equal('a', nt_reader_lh(&reader, 1, &line, &column).code_point);
  nt_assert_equal(1, line);
  nt_assert_equal(1, column);

  nt_assert_false(nt_reader_is_eof(&reader));
  nt_assert_equal('b', nt_reader_lh(&reader, 2, &line, &column).code_point);
  nt_assert_equal(1, line);
  nt_assert_equal(2, column);

  nt_assert_false(nt_reader_is_eof(&reader));
  nt_assert_equal('c', nt_reader_lh(&reader, 3, &line, &column).code_point);
  nt_assert_equal(1, line);
  nt_assert_equal(3, column);

  nt_assert_true(nt_reader_is_eof(&reader));
  nt_assert_false(nt_cp_is_valid(nt_reader_lh(&reader, 4, &line, &column)));
  nt_assert_equal(1, line);
  nt_assert_equal(4, column);

  nt_reader_free(&reader);
  nt_stream_free(&stream);
}

int main() {
  nt_test(test_empty);
  nt_test(test_simple);
  nt_test(test_utf8);
  nt_test(test_lookahead);

  return 0;
}

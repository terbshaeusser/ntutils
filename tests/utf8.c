#include "nt/utf8.h"
#include "nt/test.h"

typedef struct {
  uint32_t code_point;
  int len;
  char const *str;
} glyph_t;

enum { TEST_GLYPH_NUM = 4 };

static const glyph_t test_glyphs[TEST_GLYPH_NUM] = {(glyph_t){0x79u, 1, "y"},
                                                    (glyph_t){0xE4u, 2,
                                                              "\xC3"
                                                              "\xA4"},
                                                    (glyph_t){0x20ACu, 3,
                                                              "\xE2"
                                                              "\x82"
                                                              "\xAC"},
                                                    (glyph_t){0x1D11Eu, 4,
                                                              "\xF0"
                                                              "\x9D"
                                                              "\x84"
                                                              "\x9E"}};

static void test_utf8_len() {
  for (int i = 0; i < TEST_GLYPH_NUM; ++i) {
    nt_assert_equal(test_glyphs[i].len,
                    nt_utf8_len(nt_cp(test_glyphs[i].code_point)));
  }
}

static void test_utf8_read() {
  for (int i = 0; i < TEST_GLYPH_NUM; ++i) {
    char const *str = test_glyphs[i].str;
    size_t len = test_glyphs[i].len;

    nt_assert_equal(test_glyphs[i].code_point,
                    nt_utf8_read(&str, &len).code_point);
    nt_assert_true(str == test_glyphs[i].str + test_glyphs[i].len);
    nt_assert_equal(0, len);
  }
}

static void test_utf8_write() {
  for (int i = 0; i < TEST_GLYPH_NUM; ++i) {
    char buffer[8];
    size_t len = sizeof(buffer);

    char *str = buffer;
    nt_utf8_write(&str, &len, nt_cp(test_glyphs[i].code_point));

    nt_assert_equal(test_glyphs[i].len, sizeof(buffer) - len);
    nt_assert_equal(test_glyphs[i].len, str - buffer);
    buffer[test_glyphs[i].len] = 0;
    nt_assert_equal(test_glyphs[i].str, buffer);
  }
}

static void test_utf8_has_bom() {
  const uint8_t UTF8_BOM[NT_UTF8_BOM_LEN] = {0xEFu, 0xBBu, 0xBFu};

  nt_assert_true(nt_utf8_has_bom((char const *)UTF8_BOM, NT_UTF8_BOM_LEN));
  nt_assert_false(nt_utf8_has_bom((char const *)UTF8_BOM, NT_UTF8_BOM_LEN - 1));
  nt_assert_false(nt_utf8_has_bom("", 0));
  nt_assert_false(nt_utf8_has_bom("abcdefgh", 8));
}

int main() {
  nt_test(test_utf8_len);
  nt_test(test_utf8_read);
  nt_test(test_utf8_write);
  nt_test(test_utf8_has_bom);

  return 0;
}

#include "nt/files.h"
#include "nt/test.h"

#ifdef _WIN32
#define LONG_PATH                                                              \
  "C:\\this\\is\\a\\very\\long\\path\\to\\test\\thefunctionality\\ofthent_"    \
  "wide_"                                                                      \
  "pathfunction\\properly\\this\\is\\a\\very\\long\\path\\to\\test\\thefuncti" \
  "onality\\ofthent_wide_"                                                     \
  "pathfunction\\properly\\this\\is\\a\\very\\long\\path\\to\\test\\thefuncti" \
  "onality\\ofthent_wide_"                                                     \
  "pathfunction\\properly\\this\\is\\a\\very\\long\\path\\to\\test\\thefuncti" \
  "onality\\ofthent_wide_pathfunction\\properly"

#define LONG_PATH_WIDE                                                         \
  L"C:\\this\\is\\a\\very\\long\\path\\to\\test\\thefunctionality\\ofthent_"   \
  L"wide_"                                                                     \
  L"pathfunction\\properly\\this\\is\\a\\very\\long\\path\\to\\test\\thefunct" \
  L"ionality\\ofthent_wide_"                                                   \
  L"pathfunction\\properly\\this\\is\\a\\very\\long\\path\\to\\test\\thefunct" \
  L"ionality\\ofthent_wide_"                                                   \
  L"pathfunction\\properly\\this\\is\\a\\very\\long\\path\\to\\test\\thefunct" \
  L"ionality\\ofthent_wide_pathfunction\\properly"

#define LONG_PATH_WIDE2                                                        \
  L"C:\\this\\is\\a\\very\\long\\path\\to\\test\\thefunctionality\\ofthent_"   \
  L"wide_"                                                                     \
  L"pathfunction\\properly\\this\\is\\a\\very\\long\\path\\to\\test\\thefunct" \
  L"ionality\\ofthent_wide_"                                                   \
  L"pathfunction\\properly\\this\\is\\a\\very\\long\\path\\to\\test\\thefunct" \
  L"ionality\\ofthent_wide_"                                                   \
  L"pathfunction\\properly\\this\\is\\a\\very\\long\\path\\to\\test\\thefunct" \
  L"ionality\\abc"

static void test_wide_path() {
  nt_assert_equal(L"relative", nt_wide_path("relative"), wcscmp);

  nt_assert_equal(L"relative/../.", nt_wide_path("relative/../."), wcscmp);

  nt_assert_equal(L"c:\\absolute", nt_wide_path("c:\\absolute"), wcscmp);

  nt_assert_equal(L"\\\\absolute", nt_wide_path("\\\\absolute"), wcscmp);

  nt_assert_equal(L"\\\\?\\" LONG_PATH_WIDE, nt_wide_path(LONG_PATH), wcscmp);

  nt_assert_equal(L"\\\\?\\" LONG_PATH_WIDE2,
                  nt_wide_path(LONG_PATH "/../\\./../abc/."), wcscmp);
}
#endif

int main() {
#ifdef _WIN32
  nt_test(test_wide_path);
#endif

  return 0;
}

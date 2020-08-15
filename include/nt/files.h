#ifndef NTUTILS_FILES_H
#define NTUTILS_FILES_H

#include "nt/str.h"

#ifdef _WIN32
#define NT_MAX_PATH 32767

#define nt_wide_path(str)                                                      \
  _Generic(str, char*                                                          \
           : P_nt_wide_path_cstr_wo_len, char const*                           \
           : P_nt_wide_path_cstr_wo_len, nt_cstr_t                             \
           : P_nt_wide_path_cstr, nt_str_t*                                    \
           : P_nt_wide_path_str, nt_str_t const*                               \
           : P_nt_wide_path_str                                                \
  )(str)

wchar_t const *P_nt_wide_path_cstr_wo_len(char const *str);

wchar_t const *P_nt_wide_path_cstr(nt_cstr_t str);

wchar_t const *P_nt_wide_path_str(nt_str_t const *str);
#else
#include <limits.h>

#define NT_MAX_PATH PATH_MAX
#endif

#define nt_is_absolute(path)                                                   \
  _Generic(path, char*                                                         \
           : P_nt_is_absolute_cstr_no_len, char const*                         \
           : P_nt_is_absolute_cstr_no_len, nt_cstr_t                           \
           : P_nt_is_absolute_cstr, nt_str_t*                                  \
           : P_nt_is_absolute_str, nt_str_t const*                             \
           : P_nt_is_absolute_str                                              \
  )(path)

bool P_nt_is_absolute_cstr_no_len(char const *path);

bool P_nt_is_absolute_cstr(nt_cstr_t path);

bool P_nt_is_absolute_str(nt_str_t const *path);

#define nt_is_relative(path) !nt_is_absolute(path)

#define nt_file_exists(path)                                                   \
  _Generic(path, char*                                                         \
           : P_nt_file_exists_cstr_no_len, char const*                         \
           : P_nt_file_exists_cstr_no_len, nt_cstr_t                           \
           : P_nt_file_exists_cstr, nt_str_t*                                  \
           : P_nt_file_exists_str, nt_str_t const*                             \
           : P_nt_file_exists_str                                              \
  )(path)

bool P_nt_file_exists_cstr_no_len(char const *path);

bool P_nt_file_exists_cstr(nt_cstr_t path);

bool P_nt_file_exists_str(nt_str_t const *path);

#define nt_dir_exists(path)                                                    \
  _Generic(path, char*                                                         \
           : P_nt_dir_exists_cstr_no_len, char const*                          \
           : P_nt_dir_exists_cstr_no_len, nt_cstr_t                            \
           : P_nt_dir_exists_cstr, nt_str_t*                                   \
           : P_nt_dir_exists_str, nt_str_t const*                              \
           : P_nt_dir_exists_str                                               \
  )(path)

bool P_nt_dir_exists_cstr_no_len(char const *path);

bool P_nt_dir_exists_cstr(nt_cstr_t path);

bool P_nt_dir_exists_str(nt_str_t const *path);

#define nt_mkdir(path)                                                         \
  _Generic(path, char*                                                         \
           : P_nt_mkdir_cstr_no_len, char const*                               \
           : P_nt_mkdir_cstr_no_len, nt_cstr_t                                 \
           : P_nt_mkdir_cstr, nt_str_t*                                        \
           : P_nt_mkdir_str, nt_str_t const*                                   \
           : P_nt_mkdir_str                                                    \
  )(path)

bool P_nt_mkdir_cstr_no_len(char const *path);

bool P_nt_mkdir_cstr(nt_cstr_t path);

bool P_nt_mkdir_str(nt_str_t const *path);

#define nt_mkdirs(path)                                                        \
  _Generic(path, char*                                                         \
           : P_nt_mkdirs_cstr_no_len, char const*                              \
           : P_nt_mkdirs_cstr_no_len, nt_cstr_t                                \
           : P_nt_mkdirs_cstr, nt_str_t*                                       \
           : P_nt_mkdirs_str, nt_str_t const*                                  \
           : P_nt_mkdirs_str                                                   \
  )(path)

bool P_nt_mkdirs_cstr_no_len(char const *path);

bool P_nt_mkdirs_cstr(nt_cstr_t path);

bool P_nt_mkdirs_str(nt_str_t const *path);

bool nt_resolve(nt_str_t *path);

#endif // NTUTILS_FILES_H

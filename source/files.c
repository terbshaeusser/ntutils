#include "nt/files.h"
#include "nt/assert.h"
#include "nt/thread.h"

#ifdef _WIN32
#include "nt/utf8.h"
#include <windows.h>

#ifndef WC_ERR_INVALID_CHARS
#define WC_ERR_INVALID_CHARS 0x0080
#endif

DWORD GetFinalPathNameByHandleW(HANDLE hFile, LPWSTR lpszFilePath,
                                DWORD cchFilePath, DWORD dwFlags);

enum { LONG_PATH_PREFIX_LEN = 4 };
static const wchar_t long_path_prefix[LONG_PATH_PREFIX_LEN + 1] = L"\\\\?\\";
static thread_local wchar_t conv_buffer[NT_MAX_PATH];

wchar_t const *P_nt_wide_path_cstr_wo_len(char const *str) {
  return P_nt_wide_path_cstr(nt_cstr(str));
}

static size_t canonicalize(wchar_t *str, size_t len) {
  wchar_t const *start = str;
  wchar_t const *end = str + len;

  if (len >= LONG_PATH_PREFIX_LEN &&
      memcmp(start, long_path_prefix, LONG_PATH_PREFIX_LEN * sizeof(wchar_t)) ==
          0) {
    start += LONG_PATH_PREFIX_LEN - 1;
  }

  wchar_t const *rptr = start;
  wchar_t *wptr = (wchar_t *)start;

  while (rptr < end) {
    wchar_t c = *rptr;
    ++rptr;

    if (c == L'/') {
      c = L'\\';
    }

    if (c == L'\\') {
      size_t diff = wptr - start;

      if (diff > 0 && wptr[-1] == L'\\') {
        continue;
      } else if (diff > 2 && wptr[-1] == L'.' && wptr[-2] == L'.' &&
                 wptr[-3] == L'\\') {
        wchar_t *prev_dir_ptr = wptr - 4;
        bool found = false;

        while (true) {
          if (prev_dir_ptr < start) {
            break;
          }

          if (*prev_dir_ptr == L'\\') {
            found = true;
            break;
          }

          --prev_dir_ptr;
        }

        if (found) {
          wptr = prev_dir_ptr + 1;
          continue;
        }
      } else if (diff > 1 && wptr[-1] == L'.' && wptr[-2] == L'\\') {
        wptr -= 1;
        continue;
      }
    }

    if (rptr != wptr || c == L'/') {
      *wptr = c;
    }

    ++wptr;
  }

  size_t diff = wptr - start;
  if (diff > 2 && wptr[-1] == L'.' && wptr[-2] == L'\\') {
    wptr -= 2;
  }

  if (wptr < end) {
    *wptr = 0;
  }

  return wptr - str;
}

wchar_t const *P_nt_wide_path_cstr(nt_cstr_t str) {
  if (str.len == 0) {
    return NULL;
  }

  if (str.len < MAX_PATH ||
      strncmp("\\\\?\\", str.str, LONG_PATH_PREFIX_LEN) == 0) {
    size_t written =
        nt_utf8_to_wide(conv_buffer, NT_MAX_PATH - 1, str.str, str.len);
    if (written == 0) {
      return NULL;
    }

    conv_buffer[written] = 0;
    return conv_buffer;
  }

  memcpy(conv_buffer, long_path_prefix, LONG_PATH_PREFIX_LEN * sizeof(wchar_t));
  size_t written =
      nt_utf8_to_wide(conv_buffer + LONG_PATH_PREFIX_LEN,
                      NT_MAX_PATH - 1 - LONG_PATH_PREFIX_LEN, str.str, str.len);
  if (written == 0) {
    return NULL;
  }

  conv_buffer[LONG_PATH_PREFIX_LEN + written] = 0;

  canonicalize(conv_buffer, LONG_PATH_PREFIX_LEN + written);

  return conv_buffer;
}

wchar_t const *P_nt_wide_path_str(nt_str_t const *str) {
  return P_nt_wide_path_cstr(nt_cstr(nt_str_str(str), nt_str_len(str)));
}
#else
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

static thread_local char conv_buffer[NT_MAX_PATH];
#endif

bool P_nt_is_absolute_cstr_no_len(char const *path) {
#ifdef _WIN32
  return *path == '/' || *path == '\\' || (*path != 0 && path[1] == ':');
#else
  return *path == '/';
#endif
}

bool P_nt_is_absolute_cstr(nt_cstr_t path) {
#ifdef _WIN32
  return (path.len > 1 && path.str[1] == ':') ||
         (path.len > 0 && path.str[0] == '/') ||
         (path.len > 0 && path.str[0] == '\\');
#else
  return path.len > 0 && path.str[0] == '/';
#endif
}

bool P_nt_is_absolute_str(nt_str_t const *path) {
  return P_nt_is_absolute_cstr(nt_cstr(nt_str_str(path), nt_str_len(path)));
}

bool P_nt_file_exists_cstr_no_len(char const *path) {
#ifdef _WIN32
  DWORD attrs = GetFileAttributesW(nt_wide_path(path));

  return attrs != INVALID_FILE_ATTRIBUTES &&
         (attrs & FILE_ATTRIBUTE_DIRECTORY) == 0;
#else
  struct stat s;
  return stat(path, &s) == 0 && S_ISREG(s.st_mode);
#endif
}

bool P_nt_file_exists_cstr(nt_cstr_t path) {
  return P_nt_file_exists_cstr_no_len(path.str);
}

bool P_nt_file_exists_str(nt_str_t const *path) {
  return P_nt_file_exists_cstr_no_len(nt_str_str(path));
}

bool P_nt_dir_exists_cstr_no_len(char const *path) {
#ifdef _WIN32
  DWORD attrs = GetFileAttributesW(nt_wide_path(path));

  return attrs != INVALID_FILE_ATTRIBUTES &&
         (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
  struct stat s;
  return stat(path, &s) == 0 && S_ISDIR(s.st_mode);
#endif
}

bool P_nt_dir_exists_cstr(nt_cstr_t path) {
  return P_nt_dir_exists_cstr_no_len(path.str);
}

bool P_nt_dir_exists_str(nt_str_t const *path) {
  return P_nt_dir_exists_cstr_no_len(nt_str_str(path));
}

bool P_nt_mkdir_cstr_no_len(char const *path) {
#ifdef _WIN32
  return CreateDirectoryW(nt_wide_path(path), NULL) != 0;
#else
  return mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
#endif
}

bool P_nt_mkdir_cstr(nt_cstr_t path) {
  return P_nt_mkdir_cstr_no_len(path.str);
}

bool P_nt_mkdir_str(nt_str_t const *path) {
  return P_nt_mkdir_cstr_no_len(nt_str_str(path));
}

bool P_nt_mkdirs_cstr_no_len(char const *path) {
  return P_nt_mkdirs_cstr(nt_cstr(path));
}

bool P_nt_mkdirs_cstr(nt_cstr_t path) {
  nt_assert(path.len <= NT_MAX_PATH);

#ifdef _WIN32
  wchar_t *ptr = (wchar_t *)nt_wide_path(path);
  if (wcsncmp(ptr, long_path_prefix, LONG_PATH_PREFIX_LEN) == 0) {
    ptr += LONG_PATH_PREFIX_LEN;
  } else if (wcsncmp(ptr, L"\\\\", 2) == 0) {
    ptr += 2;
  }

  while (true) {
    wchar_t c = *ptr;
    if (c == 0) {
      break;
    }

    if (c == L'/' || c == L'\\') {
      *ptr = 0;
      if (CreateDirectoryW(conv_buffer, NULL) == 0 &&
          GetLastError() != ERROR_ALREADY_EXISTS) {
        return false;
      }
      *ptr = c;
    }

    ++ptr;
  }

  return CreateDirectoryW(conv_buffer, NULL) != 0 ||
         GetLastError() == ERROR_ALREADY_EXISTS;
#else
  memcpy(conv_buffer, path.str, path.len);
  char *ptr = conv_buffer;
  char const *end = ptr + path.len;

  while (ptr < end) {
    ptr = memchr(ptr, '/', end - ptr);
    if (!ptr) {
      break;
    }

    *ptr = 0;
    if (mkdir(conv_buffer, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0 &&
        errno != EEXIST) {
      return false;
    }
    *ptr = '/';
    ++ptr;
  }

  return mkdir(conv_buffer, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 ||
         errno == EEXIST;
#endif
}

bool P_nt_mkdirs_str(nt_str_t const *path) {
  return P_nt_mkdirs_cstr(nt_cstr(nt_str_str(path), nt_str_len(path)));
}

bool nt_resolve(nt_str_t *path) {
#ifdef _WIN32
  HANDLE handle =
      CreateFileW(nt_wide_path(path), 0,
                  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
                  OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
  if (handle == INVALID_HANDLE_VALUE) {
    return false;
  }

  DWORD len = GetFinalPathNameByHandleW(handle, conv_buffer, NT_MAX_PATH,
                                        FILE_NAME_NORMALIZED);
  CloseHandle(handle);

  if (len == 0) {
    return false;
  }

  int u8_len = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, conv_buffer,
                                   len, NULL, 0, NULL, NULL);
  if (u8_len == 0) {
    return false;
  }

  size_t old_len = nt_str_len(path);
  if (old_len < (size_t)u8_len) {
    nt_str_reserve(path, u8_len - (size_t)old_len);
  }

  WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, conv_buffer, len,
                      (char *)nt_str_str(path), u8_len, NULL, NULL);
  nt_str_set_len(path, u8_len);
  return true;
#else
  if (!realpath(nt_str_str(path), conv_buffer)) {
    return false;
  }

  nt_str_assign(path, nt_cstr(conv_buffer));
  return true;
#endif
}

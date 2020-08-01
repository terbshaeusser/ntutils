#ifndef NTUTILS_STREAM_H
#define NTUTILS_STREAM_H

#include "nt/macros.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct P_nt_stream_t;

typedef void (*nt_stream_free_t)(struct P_nt_stream_t *self);

typedef bool (*nt_stream_is_eof_t)(struct P_nt_stream_t const *self);

typedef int64_t (*nt_stream_tell_t)(struct P_nt_stream_t const *self);

typedef bool (*nt_stream_seek_t)(struct P_nt_stream_t *self, int64_t pos,
                                 bool from_end);

typedef int32_t (*nt_stream_read_t)(struct P_nt_stream_t *self, void *block,
                                    size_t size);

typedef int32_t (*nt_stream_write_t)(struct P_nt_stream_t *self,
                                     void const *block, size_t size);

typedef struct P_nt_stream_t {
  union {
    FILE *file;
    struct {
      char const *str;
      size_t len;
      size_t pos;
    };
  } P_data;
  nt_stream_free_t free;
  nt_stream_is_eof_t is_eof;
  nt_stream_tell_t tell;
  nt_stream_seek_t seek;
  nt_stream_read_t read;
  nt_stream_write_t write;
} nt_stream_t;

typedef enum {
  NT_FILE_MODE_RO,
  NT_FILE_MODE_WO,
  NT_FILE_MODE_RW
} nt_file_mode_t;

bool nt_stream_from_file(nt_stream_t *stream, nt_file_mode_t mode,
                         char const *path);

#define nt_stream_from_str(stream, str, ...)                                   \
  P_nt_stream_from_str(stream, str,                                            \
                       NT_GET_ARG0_OR_DEFAULT(strlen(str), __VA_ARGS__))

void P_nt_stream_from_str(nt_stream_t *stream, char const *str, size_t len);

#endif // NTUTILS_STREAM_H

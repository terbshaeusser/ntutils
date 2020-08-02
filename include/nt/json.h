#ifndef NTUTILS_JSON_H
#define NTUTILS_JSON_H

#include "nt/reader.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  union {
    size_t index;
    size_t len;
  };
  char const *name;
} nt_json_key_t;

typedef enum {
  NT_JSON_TYPE_STRING,
  NT_JSON_TYPE_INT,
  NT_JSON_TYPE_FLOAT,
  NT_JSON_TYPE_NULL,
  NT_JSON_TYPE_BOOL,
  NT_JSON_TYPE_ARRAY,
  NT_JSON_TYPE_OBJECT
} nt_json_type_t;

typedef struct {
  nt_json_type_t type;
  union {
    bool bool_value;
    int64_t int_value;
    double float_value;
    bool closed;
  };
  size_t len;
  char const *str_repr;
  uint32_t line;
  uint32_t column;
} nt_json_value_t;

struct P_nt_json_eval_with_ctx_t;

typedef bool (*nt_json_eval_t)(void *ctx, nt_json_key_t const *key,
                               nt_json_value_t const *value,
                               struct P_nt_json_eval_with_ctx_t *child_eval);

typedef struct P_nt_json_eval_with_ctx_t {
  void *ctx;
  nt_json_eval_t eval;
} nt_json_eval_with_ctx_t;

typedef void (*nt_json_error_handler_t)(void *ctx, uint32_t line,
                                        uint32_t column, char const *msg,
                                        size_t msg_len);

bool nt_json_parse(nt_reader_t *reader, void *ctx, nt_json_eval_t eval,
                   nt_json_error_handler_t error_handler);

#endif // NTUTILS_JSON_H

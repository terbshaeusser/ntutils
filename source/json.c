#include "nt/json.h"
#include "nt/thread.h"
#include <stdlib.h>

typedef struct {
  nt_reader_t *reader;
  void *ctx;
  nt_json_eval_t eval;
  void *error_ctx;
  nt_json_error_handler_t error_handler;
  int depth;
} parse_ctx_t;

#define report_error(parse_ctx, line, column, msg)                             \
  (parse_ctx)->error_handler((parse_ctx)->error_ctx, line, column, msg,        \
                             strlen(msg))

static nt_cp_t lh_non_ws(parse_ctx_t *parse_ctx, uint32_t *line,
                         uint32_t *column) {
  while (true) {
    nt_cp_t cp = nt_reader_lh(parse_ctx->reader, 1, line, column);
    if (!nt_cp_is_whitespace(cp)) {
      return cp;
    }

    nt_reader_next(parse_ctx->reader);
  }
}

static bool is_separator(nt_cp_t cp) {
  return (cp.code_point < 'a' || cp.code_point > 'z') &&
         (cp.code_point < 'A' || cp.code_point > 'Z') &&
         (cp.code_point < '0' || cp.code_point > '9') && cp.code_point != '_';
}

enum { MAX_NUM_LEN = 64 };

static bool read_num_value(parse_ctx_t *parse_ctx, nt_json_key_t const *key) {
  char num[MAX_NUM_LEN + 1] = {0};
  char *ptr = num;
  size_t len = MAX_NUM_LEN;
  uint32_t line;
  uint32_t column;
  size_t old_len = len;
  nt_utf8_write(&ptr, &len, nt_reader_next(parse_ctx->reader, &line, &column));
  bool is_float = false;

  while (old_len != len) {
    nt_cp_t cp = nt_reader_lh(parse_ctx->reader);
    if (is_separator(cp)) {
      if (cp.code_point == '.' || cp.code_point == '+' ||
          cp.code_point == '-') {
        is_float = true;
      } else {
        break;
      }
    }

    old_len = len;
    nt_utf8_write(&ptr, &len, nt_reader_next(parse_ctx->reader));
  }

  len = MAX_NUM_LEN - len;
  nt_json_value_t value = {is_float ? NT_JSON_TYPE_FLOAT : NT_JSON_TYPE_INT,
                           {0},
                           len,
                           num,
                           line,
                           column};

  char *end_ptr;
  if (is_float) {
    value.float_value = strtod(num, &end_ptr);
    if (*end_ptr) {
      report_error(parse_ctx, line, column,
                   "Invalid floating point number literal");
      return false;
    }
  } else {
    value.int_value = strtoll(num, &end_ptr, 10);
    if (*end_ptr) {
      report_error(parse_ctx, line, column, "Invalid integer number literal");
      return false;
    }
  }

  nt_json_eval_with_ctx_t child_eval;
  return parse_ctx->eval(parse_ctx->ctx, key, &value, &child_eval);
}

static bool read_hex(parse_ctx_t *parse_ctx, char *buffer) {
  nt_cp_t cp = nt_reader_next(parse_ctx->reader);
  if ((cp.code_point < 'a' || cp.code_point > 'f') &&
      (cp.code_point < 'A' || cp.code_point > 'F') &&
      (cp.code_point < '0' || cp.code_point > '9')) {
    return false;
  }

  *buffer = (char)cp.code_point;
  return true;
}

static bool read_str(parse_ctx_t *parse_ctx, char *str, size_t *str_len,
                     uint32_t *line, uint32_t *column) {
  char *ptr = str;
  size_t len = *str_len;
  nt_reader_next(parse_ctx->reader, line, column);

  while (true) {
    uint32_t cp_line;
    uint32_t cp_column;
    nt_cp_t cp = nt_reader_next(parse_ctx->reader, &cp_line, &cp_column);

    if (!nt_cp_is_valid(cp)) {
      report_error(parse_ctx, *line, *column, "String literal is not closed");
      return false;
    }

    if (cp.code_point == '"') {
      break;
    }

    if (cp.code_point == '\\') {
      cp = nt_reader_next(parse_ctx->reader);

      switch (cp.code_point) {
      case '"':
      case '\\':
      case '/':
        break;
      case 'b':
        cp.code_point = '\b';
        break;
      case 'f':
        cp.code_point = '\f';
        break;
      case 'n':
        cp.code_point = '\n';
        break;
      case 'r':
        cp.code_point = '\r';
        break;
      case 't':
        cp.code_point = '\t';
        break;
      case 'u': {
        char hex[5] = {0};
        bool valid = true;

        for (int i = 0; i < 4; ++i) {
          if (!read_hex(parse_ctx, hex + i)) {
            valid = false;
            break;
          }
        }

        if (valid) {
          cp.code_point = strtol(hex, NULL, 16);
          break;
        }
      }
        // fallthrough
      default:
        report_error(parse_ctx, cp_line, cp_column,
                     "Invalid string escape sequence");
        return false;
      }
    }

    size_t old_len = len;
    nt_utf8_write(&ptr, &len, cp);
    if (old_len == len) {
      report_error(parse_ctx, *line, *column,
                   "String literal exceeds maximum literal size");
      return false;
    }
  }

  *str_len -= len;
  return true;
}

static thread_local char str_literal_buf[NT_JSON_MAX_STR_LITERAL_LEN + 1];

static bool read_str_value(parse_ctx_t *parse_ctx, nt_json_key_t const *key) {
  uint32_t line;
  uint32_t column;
  size_t len = NT_JSON_MAX_STR_LITERAL_LEN;

  if (!read_str(parse_ctx, str_literal_buf, &len, &line, &column)) {
    return false;
  }

  nt_json_value_t value = {NT_JSON_TYPE_STRING, {0},  len,
                           str_literal_buf,     line, column};
  nt_json_eval_with_ctx_t child_eval = {parse_ctx->ctx, parse_ctx->eval};

  return parse_ctx->eval(parse_ctx->ctx, key, &value, &child_eval);
}

static bool read_value(parse_ctx_t *parse_ctx, nt_json_key_t const *key);

static bool read_object_value(parse_ctx_t *parse_ctx,
                              nt_json_key_t const *key) {
  uint32_t line;
  uint32_t column;
  nt_reader_next(parse_ctx->reader, &line, &column);

  nt_json_value_t value = {NT_JSON_TYPE_OBJECT, {false}, 0, NULL, line, column};
  nt_json_eval_with_ctx_t child_eval = {parse_ctx->ctx, parse_ctx->eval};

  if (!parse_ctx->eval(parse_ctx->ctx, key, &value, &child_eval)) {
    return false;
  }

  if (lh_non_ws(parse_ctx, NULL, NULL).code_point == '}') {
    nt_reader_next(parse_ctx->reader);
  } else {
    parse_ctx_t child_parse_ctx = {
        parse_ctx->reader,    child_eval.ctx,           child_eval.eval,
        parse_ctx->error_ctx, parse_ctx->error_handler, parse_ctx->depth + 1};

    char key_name[NT_JSON_MAX_KEY_LEN + 1];

    while (true) {
      size_t key_len = NT_JSON_MAX_KEY_LEN;
      if (!read_str(&child_parse_ctx, key_name, &key_len, &line, &column)) {
        return false;
      }
      key_name[key_len] = 0;

      if (lh_non_ws(parse_ctx, NULL, NULL).code_point != ':') {
        report_error(parse_ctx, line, column, "Expected :");
        return false;
      }
      nt_reader_next(parse_ctx->reader);

      nt_json_key_t child_key = {{key_len}, key_name};
      if (!read_value(&child_parse_ctx, &child_key)) {
        return false;
      }

      nt_cp_t cp = lh_non_ws(parse_ctx, &line, &column);
      if (cp.code_point == '}') {
        nt_reader_next(parse_ctx->reader);
        break;
      }

      if (cp.code_point != ',') {
        report_error(parse_ctx, line, column, "Expected , or }");
        return false;
      }

      nt_reader_next(parse_ctx->reader);
      lh_non_ws(parse_ctx, NULL, NULL);
    }
  }

  value.closed = true;
  return parse_ctx->eval(parse_ctx->ctx, key, &value, &child_eval);
}

static bool read_array_value(parse_ctx_t *parse_ctx, nt_json_key_t const *key) {
  uint32_t line;
  uint32_t column;
  nt_reader_next(parse_ctx->reader, &line, &column);

  nt_json_value_t value = {NT_JSON_TYPE_ARRAY, {false}, 0, NULL, line, column};
  nt_json_eval_with_ctx_t child_eval = {parse_ctx->ctx, parse_ctx->eval};

  if (!parse_ctx->eval(parse_ctx->ctx, key, &value, &child_eval)) {
    return false;
  }

  if (lh_non_ws(parse_ctx, NULL, NULL).code_point == ']') {
    nt_reader_next(parse_ctx->reader);
  } else {
    parse_ctx_t child_parse_ctx = {
        parse_ctx->reader,    child_eval.ctx,           child_eval.eval,
        parse_ctx->error_ctx, parse_ctx->error_handler, parse_ctx->depth + 1};

    size_t index = 0;
    while (true) {
      nt_json_key_t child_key = {{index}, NULL};
      if (!read_value(&child_parse_ctx, &child_key)) {
        return false;
      }

      ++index;

      nt_cp_t cp = lh_non_ws(parse_ctx, &line, &column);
      if (cp.code_point == ']') {
        nt_reader_next(parse_ctx->reader);
        break;
      }

      if (cp.code_point != ',') {
        report_error(parse_ctx, line, column, "Expected , or ]");
        return false;
      }

      nt_reader_next(parse_ctx->reader);
      lh_non_ws(parse_ctx, NULL, NULL);
    }
  }

  value.closed = true;
  return parse_ctx->eval(parse_ctx->ctx, key, &value, &child_eval);
}

enum { MAX_KEYWORD_LEN = 5 };

static bool read_keyword_value(parse_ctx_t *parse_ctx,
                               nt_json_key_t const *key) {
  char keyword[MAX_KEYWORD_LEN];
  char *ptr = keyword;
  size_t len = MAX_KEYWORD_LEN;
  uint32_t line;
  uint32_t column;
  size_t old_len = len;
  nt_utf8_write(&ptr, &len, nt_reader_next(parse_ctx->reader, &line, &column));

  while (old_len != len && !is_separator(nt_reader_lh(parse_ctx->reader))) {
    old_len = len;
    nt_utf8_write(&ptr, &len, nt_reader_next(parse_ctx->reader));
  }

  len = MAX_KEYWORD_LEN - len;
  nt_json_value_t value = {0, {0}, 0, NULL, line, column};

  if (len == 4 && memcmp(keyword, "null", 4) == 0) {
    value.type = NT_JSON_TYPE_NULL;
  } else if (len == 4 && memcmp(keyword, "true", 4) == 0) {
    value.type = NT_JSON_TYPE_BOOL;
    value.bool_value = true;
  } else if (len == 5 && memcmp(keyword, "false", 5) == 0) {
    value.type = NT_JSON_TYPE_BOOL;
    value.bool_value = false;
  } else {
    report_error(parse_ctx, line, column, "Invalid value");
    return false;
  }

  nt_json_eval_with_ctx_t child_eval;
  return parse_ctx->eval(parse_ctx->ctx, key, &value, &child_eval);
}

static bool read_value(parse_ctx_t *parse_ctx, nt_json_key_t const *key) {
  uint32_t line;
  uint32_t column;
  nt_cp_t cp = lh_non_ws(parse_ctx, &line, &column);

  if (parse_ctx->depth > NT_JSON_MAX_DEPTH) {
    report_error(parse_ctx, line, column, "Maximum allowed depth exceeded");
    return false;
  }

  if (!nt_cp_is_valid(cp)) {
    report_error(parse_ctx, line, column, "Expected value");
    return false;
  }

  switch (cp.code_point) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '-':
    return read_num_value(parse_ctx, key);
  case '"':
    return read_str_value(parse_ctx, key);
  case '{':
    return read_object_value(parse_ctx, key);
  case '[':
    return read_array_value(parse_ctx, key);
  case 'f':
  case 'n':
  case 't':
    return read_keyword_value(parse_ctx, key);
  default:
    report_error(parse_ctx, line, column,
                 "Invalid character outside of string");
    return false;
  }
}

bool nt_json_parse(nt_reader_t *reader, void *ctx, nt_json_eval_t eval,
                   nt_json_error_handler_t error_handler) {
  parse_ctx_t parse_ctx = {reader, ctx, eval, ctx, error_handler, 0};

  if (!read_value(&parse_ctx, NULL)) {
    return false;
  }

  if (!nt_reader_is_eof(reader)) {
    uint32_t line;
    uint32_t column;
    nt_reader_lh(reader, 1, &line, &column);

    report_error(&parse_ctx, line, column, "Expected end of data");
    return false;
  }

  return true;
}

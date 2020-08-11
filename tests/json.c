#include "nt/json.h"
#include "nt/test.h"

static bool eval_true(void *ctx, nt_json_key_t const *key,
                      nt_json_value_t const *value,
                      struct P_nt_json_eval_with_ctx_t *child_eval) {
  nt_assert_null(key);
  nt_assert_equal(NT_JSON_TYPE_BOOL, value->type);
  nt_assert_true(value->bool_value);
  nt_assert_equal(1, value->line);
  nt_assert_equal(1, value->column);
  return true;
}

static void no_error_handler(void *ctx, uint32_t line, uint32_t column,
                             char const *msg, size_t msg_len) {
  nt_assert_true(false);
}

static void test_true() {
  char const *str = "true";

  nt_stream_t stream;
  nt_stream_from_str(&stream, str);
  nt_reader_t reader = nt_reader_new(&stream);

  nt_assert_true(nt_json_parse(&reader, NULL, eval_true, no_error_handler));

  nt_stream_free(&stream);
}

static bool eval_zero(void *ctx, nt_json_key_t const *key,
                      nt_json_value_t const *value,
                      struct P_nt_json_eval_with_ctx_t *child_eval) {
  nt_assert_null(key);
  nt_assert_equal(NT_JSON_TYPE_INT, value->type);
  nt_assert_equal(0, value->int_value);
  nt_assert_equal(1, value->line);
  nt_assert_equal(1, value->column);
  return true;
}

static void test_zero() {
  char const *str = "0";

  nt_stream_t stream;
  nt_stream_from_str(&stream, str);
  nt_reader_t reader = nt_reader_new(&stream);

  nt_assert_true(nt_json_parse(&reader, NULL, eval_zero, no_error_handler));

  nt_stream_free(&stream);
}

static bool eval_float(void *ctx, nt_json_key_t const *key,
                       nt_json_value_t const *value,
                       struct P_nt_json_eval_with_ctx_t *child_eval) {
  nt_assert_null(key);
  nt_assert_equal(NT_JSON_TYPE_FLOAT, value->type);
  nt_assert_equal(0.12E+5, value->float_value);
  nt_assert_equal(7, value->len);
  nt_assert_equal("0.12E+5", value->str_repr);
  nt_assert_equal(1, value->line);
  nt_assert_equal(1, value->column);
  return true;
}

static void test_float() {
  char const *str = "0.12E+5";

  nt_stream_t stream;
  nt_stream_from_str(&stream, str);
  nt_reader_t reader = nt_reader_new(&stream);

  nt_assert_true(nt_json_parse(&reader, NULL, eval_float, no_error_handler));

  nt_stream_free(&stream);
}

static bool eval_string(void *ctx, nt_json_key_t const *key,
                        nt_json_value_t const *value,
                        struct P_nt_json_eval_with_ctx_t *child_eval) {
  nt_assert_null(key);
  nt_assert_equal(NT_JSON_TYPE_STRING, value->type);
  nt_assert_equal(3, value->len);
  nt_assert_equal("abc", value->str_repr);
  nt_assert_equal(1, value->line);
  nt_assert_equal(1, value->column);
  return true;
}

static void test_string() {
  char const *str = "\"abc\"";

  nt_stream_t stream;
  nt_stream_from_str(&stream, str);
  nt_reader_t reader = nt_reader_new(&stream);

  nt_assert_true(nt_json_parse(&reader, NULL, eval_string, no_error_handler));

  nt_stream_free(&stream);
}

static bool eval_string_escape(void *ctx, nt_json_key_t const *key,
                               nt_json_value_t const *value,
                               struct P_nt_json_eval_with_ctx_t *child_eval) {
  nt_assert_null(key);
  nt_assert_equal(NT_JSON_TYPE_STRING, value->type);
  nt_assert_equal(10, value->len);
  nt_assert_equal("\"\\\n\r\t\f\b\xE1\x80\x90", value->str_repr);
  nt_assert_equal(1, value->line);
  nt_assert_equal(1, value->column);
  return true;
}

static void test_string_escape() {
  char const *str = "\"\\\"\\\\\\n\\r\\t\\f\\b\\u1010\"";

  nt_stream_t stream;
  nt_stream_from_str(&stream, str);
  nt_reader_t reader = nt_reader_new(&stream);

  nt_assert_true(
      nt_json_parse(&reader, NULL, eval_string_escape, no_error_handler));

  nt_stream_free(&stream);
}

typedef struct {
  bool opened;
  int32_t next_value;
} array_ctx_t;

static bool eval_array_nested(void *ctx, nt_json_key_t const *key,
                              nt_json_value_t const *value,
                              struct P_nt_json_eval_with_ctx_t *child_eval) {
  array_ctx_t *array_ctx = ctx;
  nt_assert_less_equal(3, array_ctx->next_value);

  nt_assert_null(key->name);
  nt_assert_equal(array_ctx->next_value - 1, key->index);
  nt_assert_equal(NT_JSON_TYPE_INT, value->type);
  nt_assert_equal(array_ctx->next_value, value->int_value);

  ++array_ctx->next_value;
  return true;
}

static bool eval_array(void *ctx, nt_json_key_t const *key,
                       nt_json_value_t const *value,
                       struct P_nt_json_eval_with_ctx_t *child_eval) {
  array_ctx_t *array_ctx = ctx;

  nt_assert_null(key);
  nt_assert_equal(NT_JSON_TYPE_ARRAY, value->type);
  if (array_ctx->opened) {
    nt_assert_true(value->closed);
  } else {
    nt_assert_false(value->closed);
    array_ctx->opened = true;
  }
  nt_assert_equal(1, value->line);
  nt_assert_equal(1, value->column);

  child_eval->eval = eval_array_nested;
  return true;
}

static void test_array() {
  char const *str = "[1,2,3]";

  nt_stream_t stream;
  nt_stream_from_str(&stream, str);
  nt_reader_t reader = nt_reader_new(&stream);

  array_ctx_t ctx = {false, 1};
  nt_assert_true(nt_json_parse(&reader, &ctx, eval_array, no_error_handler));
  nt_assert_equal(4, ctx.next_value);

  nt_stream_free(&stream);
}

typedef struct {
  bool opened;
  int32_t next_value;
} object_ctx_t;

static bool eval_object_nested(void *ctx, nt_json_key_t const *key,
                               nt_json_value_t const *value,
                               struct P_nt_json_eval_with_ctx_t *child_eval) {
  object_ctx_t *object_ctx = ctx;

  nt_assert_less_equal(2, object_ctx->next_value);

  nt_assert_not_null(key->name);
  nt_assert_equal(1, key->len);
  nt_assert_equal(object_ctx->next_value == 1 ? "a" : "b", key->name);

  nt_assert_equal(NT_JSON_TYPE_INT, value->type);
  nt_assert_equal(object_ctx->next_value, value->int_value);

  ++object_ctx->next_value;
  return true;
}

static bool eval_object(void *ctx, nt_json_key_t const *key,
                        nt_json_value_t const *value,
                        struct P_nt_json_eval_with_ctx_t *child_eval) {
  object_ctx_t *object_ctx = ctx;

  nt_assert_null(key);
  nt_assert_equal(NT_JSON_TYPE_OBJECT, value->type);
  if (object_ctx->opened) {
    nt_assert_true(value->closed);
  } else {
    nt_assert_false(value->closed);
    object_ctx->opened = true;
  }
  nt_assert_equal(1, value->line);
  nt_assert_equal(1, value->column);

  child_eval->eval = eval_object_nested;
  return true;
}

static void test_object() {
  char const *str = "{\"a\":1,\"b\":2}";

  nt_stream_t stream;
  nt_stream_from_str(&stream, str);
  nt_reader_t reader = nt_reader_new(&stream);

  object_ctx_t ctx = {false, 1};
  nt_assert_true(nt_json_parse(&reader, &ctx, eval_object, no_error_handler));
  nt_assert_equal(3, ctx.next_value);

  nt_stream_free(&stream);
}

static bool
eval_complex_nested_array(void *ctx, nt_json_key_t const *key,
                          nt_json_value_t const *value,
                          struct P_nt_json_eval_with_ctx_t *child_eval) {
  nt_assert_null(key->name);
  nt_assert_equal(0, key->index);
  nt_assert_equal(NT_JSON_TYPE_INT, value->type);
  nt_assert_equal(1, value->int_value);
  nt_assert_equal(2, value->line);
  nt_assert_equal(14, value->column);

  return true;
}

static bool eval_complex_nested(void *ctx, nt_json_key_t const *key,
                                nt_json_value_t const *value,
                                struct P_nt_json_eval_with_ctx_t *child_eval) {
  nt_assert_not_null(key);
  bool is_array = strcmp("array", key->name) == 0;

  if (is_array) {
    nt_assert_equal(NT_JSON_TYPE_ARRAY, value->type);
    nt_assert_equal(2, value->line);
    nt_assert_equal(12, value->column);
    child_eval->eval = eval_complex_nested_array;
  } else {
    nt_assert_equal("null", key->name);
    nt_assert_equal(NT_JSON_TYPE_NULL, value->type);
    nt_assert_equal(3, value->line);
    nt_assert_equal(11, value->column);
  }

  return true;
}

static bool eval_complex(void *ctx, nt_json_key_t const *key,
                         nt_json_value_t const *value,
                         struct P_nt_json_eval_with_ctx_t *child_eval) {
  nt_assert_null(key);
  nt_assert_equal(NT_JSON_TYPE_OBJECT, value->type);
  nt_assert_equal(1, value->line);
  nt_assert_equal(1, value->column);

  child_eval->eval = eval_complex_nested;
  return true;
}

static void test_complex() {
  char const *str = "{\n  \"array\": [ 1 ],\n  \"null\": null\n}";

  nt_stream_t stream;
  nt_stream_from_str(&stream, str);
  nt_reader_t reader = nt_reader_new(&stream);

  nt_assert_true(nt_json_parse(&reader, NULL, eval_complex, no_error_handler));

  nt_stream_free(&stream);
}

int main() {
  nt_test(test_true);
  nt_test(test_zero);
  nt_test(test_float);
  nt_test(test_string);
  nt_test(test_string_escape);
  nt_test(test_array);
  nt_test(test_object);
  nt_test(test_complex);

  return 0;
}

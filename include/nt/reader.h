#ifndef NTUTILS_READER_H
#define NTUTILS_READER_H

#include "nt/circular.h"
#include "nt/config.h"
#include "nt/stream.h"
#include "nt/utf8.h"

typedef nt_cp_t (*nt_decoder_t)(char const **str, size_t *len);

typedef struct {
  nt_cp_t cp;
  uint32_t line;
  uint32_t column;
} P_nt_reader_lh_t;

NT_CIRC_BUF(P_nt_reader_lh_buff, P_nt_reader_lh_t, NT_READER_MAX_LOOKAHEAD)

typedef struct {
  nt_stream_t *P_stream;
  nt_decoder_t P_decoder;
  uint32_t P_next_line;
  uint32_t P_next_column;
  P_nt_reader_lh_buff_t P_lookaheads;
  uint8_t P_buffer[NT_READER_INTERN_BUFFER_SIZE];
  int32_t P_next;
  int32_t P_filled;
} nt_reader_t;

#define nt_reader_new(stream, ...)                                             \
  P_nt_reader_new(stream, NT_GET_ARG0_OR_DEFAULT(nt_utf8_read, __VA_ARGS__))

nt_reader_t P_nt_reader_new(nt_stream_t *stream, nt_decoder_t decoder);

void nt_reader_free(nt_reader_t *self);

bool nt_reader_is_eof(nt_reader_t const *self);

#define nt_reader_next(self, ...)                                              \
  P_nt_reader_next(self, NT_GET_ARG0_OR_DEFAULT(NULL, __VA_ARGS__),            \
                   NT_GET_ARG1_OR_DEFAULT(NULL, __VA_ARGS__))

nt_cp_t P_nt_reader_next(nt_reader_t *self, uint32_t *line, uint32_t *column);

#define nt_reader_lh(self, ...)                                                \
  P_nt_reader_lh(self, NT_GET_ARG0_OR_DEFAULT(1, __VA_ARGS__),                 \
                 NT_GET_ARG1_OR_DEFAULT(NULL, __VA_ARGS__),                    \
                 NT_GET_ARG2_OR_DEFAULT(NULL, __VA_ARGS__))

nt_cp_t P_nt_reader_lh(nt_reader_t *self, int32_t n, uint32_t *line,
                       uint32_t *column);

#endif // NTUTILS_READER_H

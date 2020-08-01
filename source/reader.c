#include "nt/reader.h"
#include "nt/assert.h"

nt_reader_t P_nt_reader_new(nt_stream_t *stream, nt_decoder_t decoder) {
  return (nt_reader_t){stream, decoder, 1, 1, P_nt_reader_lh_buff_new(),
                       {0},    0,       0};
}

void nt_reader_free(nt_reader_t *self) {
  P_nt_reader_lh_buff_free(&self->P_lookaheads);
}

bool nt_reader_is_eof(nt_reader_t const *self) {
  return self->P_next == self->P_filled &&
         self->P_stream->is_eof(self->P_stream);
}

static nt_cp_t next_without_lh(nt_reader_t *self, uint32_t *line,
                               uint32_t *column) {
  size_t len = (size_t)(self->P_filled - self->P_next);

  if (len < 4) {
    if (len > 0) {
      memmove(self->P_buffer, &self->P_buffer[self->P_next], len);
    }

    int32_t n = self->P_stream->read(self->P_stream, &self->P_buffer[len],
                                     NT_READER_INTERN_BUFFER_SIZE - len);
    if (n < 0) {
      nt_panic("An error occurred on reading from a reader input stream");
    }

    self->P_filled = len + n;
    len = self->P_filled;
  }

  char const *ptr = (char const *)&self->P_buffer[self->P_next];
  nt_cp_t cp = self->P_decoder(&ptr, &len);

  self->P_next = self->P_filled - (int32_t)len;

  if (line) {
    *line = self->P_next_line;
  }

  if (column) {
    *column = self->P_next_column;
  }

  if (nt_cp_is_newline(cp)) {
    ++self->P_next_line;
    self->P_next_column = 1;
  } else if (nt_cp_is_valid(cp)) {
    ++self->P_next_column;
  }

  return cp;
}

nt_cp_t P_nt_reader_next(nt_reader_t *self, uint32_t *line, uint32_t *column) {
  if (P_nt_reader_lh_buff_count(&self->P_lookaheads) > 0) {
    P_nt_reader_lh_t lh;
    P_nt_reader_lh_buff_pop(&self->P_lookaheads, &lh);

    if (line) {
      *line = lh.line;
    }

    if (column) {
      *column = lh.column;
    }

    return lh.cp;
  }

  return next_without_lh(self, line, column);
}

nt_cp_t P_nt_reader_lh(nt_reader_t *self, int32_t n, uint32_t *line,
                       uint32_t *column) {
  nt_assert(n > 0, "The number of code points to look ahead must be > 0");
  nt_assert(n <= NT_READER_MAX_LOOKAHEAD,
            "The number of code points to look ahead must be <= the maximum "
            "lookahead of %d",
            NT_READER_MAX_LOOKAHEAD);

  while ((size_t)n < P_nt_reader_lh_buff_count(&self->P_lookaheads)) {
    uint32_t lh_line;
    uint32_t lh_column;
    nt_cp_t cp = next_without_lh(self, &lh_line, &lh_column);

    P_nt_reader_lh_buff_push(&self->P_lookaheads,
                             (P_nt_reader_lh_t){cp, lh_line, lh_column});
  }

  P_nt_reader_lh_t const *lh =
      P_nt_reader_lh_buff_get(&self->P_lookaheads, n - 1);

  if (line) {
    *line = lh->line;
  }

  if (column) {
    *column = lh->column;
  }

  return lh->cp;
}

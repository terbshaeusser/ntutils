#include "nt/stream.h"
#include "nt/panic.h"

typedef enum {
  FILE_MODE_RB,
  FILE_MODE_WB,
  FILE_MODE_RB_PLUS,
  FILE_MODE_WB_PLUS
} intern_file_mode_t;

#ifdef _WIN32
#include "nt/mem.h"
#include "nt/utf8.h"

#define ftello _ftelli64
#define fseeko _fseeki64

static FILE *fopen_ex(char const *path, intern_file_mode_t mode) {
  wchar_t const *mode_str;

  switch (mode) {
  case FILE_MODE_RB:
    mode_str = L"rb";
    break;
  case FILE_MODE_WB:
    mode_str = L"wb";
    break;
  case FILE_MODE_RB_PLUS:
    mode_str = L"r+b";
    break;
  case FILE_MODE_WB_PLUS:
    mode_str = L"w+b";
    break;
  }

  int32_t len = -1;
  wchar_t *u16_path = nt_utf8_to_wide(path, &len);
  FILE *file = _wfopen(u16_path, mode_str);
  nt_free(u16_path);
  return file;
}
#else
static FILE *fopen_ex(char const *path, intern_file_mode_t mode) {
  char const *mode_str;

  switch (mode) {
  case FILE_MODE_RB:
    mode_str = "rb";
    break;
  case FILE_MODE_WB:
    mode_str = "wb";
    break;
  case FILE_MODE_RB_PLUS:
    mode_str = "r+b";
    break;
  case FILE_MODE_WB_PLUS:
    mode_str = "w+b";
    break;
  }

  return fopen(path, mode_str);
}
#endif

static void file_stream_free(nt_stream_t *self) { fclose(self->P_data.file); }

static bool file_stream_is_eof(nt_stream_t const *self) {
  return feof(self->P_data.file);
}

static int64_t file_stream_tell(nt_stream_t const *self) {
  return ftello(self->P_data.file);
}

static bool file_stream_seek(nt_stream_t *self, int64_t pos, bool from_end) {
  return fseeko(self->P_data.file, pos, from_end ? SEEK_END : SEEK_SET) == 0;
}

static int32_t file_stream_read(nt_stream_t *self, void *block, size_t size) {
  return fread(block, 1, size, self->P_data.file);
}

static int32_t file_stream_write(nt_stream_t *self, void const *block,
                                 size_t size) {
  return fwrite(block, 1, size, self->P_data.file);
}

static int32_t stream_deny_read(nt_stream_t *self, void *block, size_t size) {
  (void)self;
  (void)block;
  (void)size;

  nt_panic("Cannot read from a write-only file stream");
  return 0;
}

static int32_t stream_deny_write(nt_stream_t *self, void const *block,
                                 size_t size) {
  (void)self;
  (void)block;
  (void)size;

  nt_panic("Cannot write to a read-only file stream");
  return 0;
}

static bool open_file_ro(nt_stream_t *stream, char const *path) {
  FILE *file = fopen_ex(path, FILE_MODE_RB);
  if (!file) {
    return false;
  }

  stream->P_data.file = file;
  stream->P_free = file_stream_free;
  stream->P_is_eof = file_stream_is_eof;
  stream->P_tell = file_stream_tell;
  stream->P_seek = file_stream_seek;
  stream->P_read = file_stream_read;
  stream->P_write = stream_deny_write;
  return true;
}

static bool open_file_wo(nt_stream_t *stream, char const *path) {
  FILE *file = fopen_ex(path, FILE_MODE_WB);
  if (!file) {
    return false;
  }

  stream->P_data.file = file;
  stream->P_free = file_stream_free;
  stream->P_is_eof = file_stream_is_eof;
  stream->P_tell = file_stream_tell;
  stream->P_seek = file_stream_seek;
  stream->P_read = stream_deny_read;
  stream->P_write = file_stream_write;
  return true;
}

static bool open_file_rw(nt_stream_t *stream, char const *path) {
  FILE *file = fopen_ex(path, FILE_MODE_RB_PLUS);
  if (!file) {
    file = fopen_ex(path, FILE_MODE_WB_PLUS);
    if (!file) {
      return false;
    }
  }

  stream->P_data.file = file;
  stream->P_free = file_stream_free;
  stream->P_is_eof = file_stream_is_eof;
  stream->P_tell = file_stream_tell;
  stream->P_seek = file_stream_seek;
  stream->P_read = file_stream_read;
  stream->P_write = file_stream_write;
  return true;
}

bool nt_stream_from_file(nt_stream_t *stream, nt_file_mode_t mode,
                         char const *path) {
  switch (mode) {
  case NT_FILE_MODE_RO:
    return open_file_ro(stream, path);
  case NT_FILE_MODE_WO:
    return open_file_wo(stream, path);
  case NT_FILE_MODE_RW:
    return open_file_rw(stream, path);
  default:
    nt_panic("The passed file mode is invalid");
    return false;
  }
}

static void str_stream_free(nt_stream_t *self) { (void)self; }

static bool str_stream_is_eof(nt_stream_t const *self) {
  return self->P_data.pos == self->P_data.len;
}

static int64_t str_stream_tell(nt_stream_t const *self) {
  return (int64_t)self->P_data.pos;
}

static bool str_stream_seek(nt_stream_t *self, int64_t pos, bool from_end) {
  if (pos < 0 || pos > (int64_t)self->P_data.len) {
    return false;
  }

  if (from_end) {
    self->P_data.pos = self->P_data.len - (size_t)pos;
  } else {
    self->P_data.pos = (size_t)pos;
  }

  return true;
}

static int32_t str_stream_read(nt_stream_t *self, void *block, size_t size) {
  size_t result = size;
  size_t diff = self->P_data.len - self->P_data.pos;

  if (result > diff) {
    result = (int32_t)diff;
  }

  if (result) {
    memcpy(block, self->P_data.str + self->P_data.pos, result);
    self->P_data.pos += result;
  }

  return result;
}

void P_nt_stream_from_str(nt_stream_t *stream, char const *str, size_t len) {
  stream->P_data.str = str;
  stream->P_data.len = len;
  stream->P_data.pos = 0;

  stream->P_free = str_stream_free;
  stream->P_is_eof = str_stream_is_eof;
  stream->P_tell = str_stream_tell;
  stream->P_seek = str_stream_seek;
  stream->P_read = str_stream_read;
  stream->P_write = stream_deny_write;
}

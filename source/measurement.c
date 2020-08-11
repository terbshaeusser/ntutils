#include "nt/measurement.h"
#include "nt/assert.h"
#include <inttypes.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

_Static_assert(sizeof(int64_t) == sizeof(LARGE_INTEGER),
               "The size of LARGE_INTEGER and int64_t must be equal");

int64_t P_nt_time_in_us(int64_t *freq, int64_t const *prev) {
  if (!prev) {
    QueryPerformanceFrequency((LARGE_INTEGER *)freq);
  }

  int64_t time;
  QueryPerformanceCounter((LARGE_INTEGER *)&time);

  if (prev) {
    return (time - *prev) * 1000000 / *freq;
  }

  return time;
}
#else
#include <sys/time.h>

int64_t P_nt_time_in_us(int64_t *freq, int64_t const *prev) {
  (void)freq;

  struct timeval tv;
  struct timezone tz;

  int r = gettimeofday(&tv, &tz);
  nt_assert(r == 0);

  int64_t time = tv.tv_sec * 1000000 + tv.tv_usec;

  return prev ? time - *prev : time;
}
#endif

void nt_print_measure(int64_t time) {
  int64_t all_ms = time / 1000;
  int64_t all_s = all_ms / 1000;
  int64_t all_min = all_s / 60;
  int64_t all_hours = all_min / 60;

  int64_t ms = all_ms % 1000;
  int64_t s = all_s % 60;
  int64_t min = all_min % 60;
  int64_t hours = all_hours;

  if (hours > 0) {
    printf("%" PRId64 ".%02" PRId64 " h", hours, min);
  } else if (min > 0) {
    printf("%" PRId64 ".%02" PRId64 " min", min, s);
  } else if (s > 0) {
    printf("%" PRId64 ".%03" PRId64 " s", s, ms);
  } else if (ms > 0) {
    printf("%" PRId64 " ms", ms);
  } else {
    printf("%" PRId64 " µs", time);
  }
}

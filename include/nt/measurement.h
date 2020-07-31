#ifndef NTUTILS_MEASUREMENT_H
#define NTUTILS_MEASUREMENT_H

#include <stdint.h>

#define nt_measure(code, result_var)                                           \
  {                                                                            \
    int64_t P_nt_freq;                                                         \
    result_var = P_nt_time_in_us(&P_nt_freq, NULL);                            \
    code;                                                                      \
    result_var = P_nt_time_in_us(&P_nt_freq, &result_var);                     \
  }

int64_t P_nt_time_in_us(int64_t *freq, int64_t const *prev);

void nt_print_measure(int64_t time);

#endif // NTUTILS_MEASUREMENT_H

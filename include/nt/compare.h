#ifndef NTUTILS_COMPARE_H
#define NTUTILS_COMPARE_H

typedef enum {
  NT_COMPARE_ASC = -1,
  NT_COMPARE_EQUAL,
  NT_COMPARE_DESC
} nt_comparison_result_t;

#define nt_comparator_t(name, type) nt_comparison_result_t (*name)(type, type)

#endif // NTUTILS_COMPARE_H

#ifndef NTUTILS_MACROS_H
#define NTUTILS_MACROS_H

#define NT_GET_ARG0(first, ...) first
#define NT_GET_ARG1(first, second, ...) second
#define NT_GET_ARG2(first, second, third, ...) third
#define NT_GET_ARG3(first, second, third, fourth, ...) fourth
#define NT_GET_ARG4(first, second, third, fourth, fifth, ...) fifth
#define NT_GET_ARG5(first, second, third, fourth, fifth, sixth, ...) sixth
#define NT_GET_ARG6(first, second, third, fourth, fifth, sixth, seventh...)    \
  seventh
#define NT_GET_ARG7(first, second, third, fourth, fifth, sixth, seventh,       \
                    eighth, ...)                                               \
  eighth

#define NT_GET_ARG1_TO_N(first, ...) __VA_ARGS__
#define NT_GET_ARG2_TO_N(first, second, ...) __VA_ARGS__
#define NT_GET_ARG3_TO_N(first, second, third, ...) __VA_ARGS__
#define NT_GET_ARG4_TO_N(first, second, third, fourth, ...) __VA_ARGS__
#define NT_GET_ARG5_TO_N(first, second, third, fourth, fifth, ...) __VA_ARGS__
#define NT_GET_ARG6_TO_N(first, second, third, fourth, fifth, sixth, ...)      \
  __VA_ARGS__
#define NT_GET_ARG7_TO_N(first, second, third, fourth, fifth, sixth, seventh,  \
                         ...)                                                  \
  __VA_ARGS__

#define NT_GET_ARG0_OR_DEFAULT(default_value, ...)                             \
  NT_GET_ARG0(__VA_ARGS__ __VA_OPT__(, ) default_value)
#define NT_GET_ARG1_OR_DEFAULT(default_value, ...)                             \
  NT_GET_ARG1(__VA_ARGS__ __VA_OPT__(, ) default_value, default_value)
#define NT_GET_ARG2_OR_DEFAULT(default_value, ...)                             \
  NT_GET_ARG2(__VA_ARGS__ __VA_OPT__(, ) default_value, default_value,         \
              default_value)
#define NT_GET_ARG3_OR_DEFAULT(default_value, ...)                             \
  NT_GET_ARG3(__VA_ARGS__ __VA_OPT__(, ) default_value, default_value,         \
              default_value, default_value)
#define NT_GET_ARG4_OR_DEFAULT(default_value, ...)                             \
  NT_GET_ARG4(__VA_ARGS__ __VA_OPT__(, ) default_value, default_value,         \
              default_value, default_value, default_value)
#define NT_GET_ARG5_OR_DEFAULT(default_value, ...)                             \
  NT_GET_ARG5(__VA_ARGS__ __VA_OPT__(, ) default_value, default_value,         \
              default_value, default_value, default_value, default_value)
#define NT_GET_ARG6_OR_DEFAULT(default_value, ...)                             \
  NT_GET_ARG6(__VA_ARGS__ __VA_OPT__(, ) default_value, default_value,         \
              default_value, default_value, default_value, default_value,      \
              default_value)
#define NT_GET_ARG7_OR_DEFAULT(default_value, ...)                             \
  NT_GET_ARG7(__VA_ARGS__ __VA_OPT__(, ) default_value, default_value,         \
              default_value, default_value, default_value, default_value,      \
              default_value, default_value)

#define __NT_STRINGIFY(x) #x
#define NT_STRINGIFY(x) __NT_STRINGIFY(x)

#define NT_LOCATION __FILE__ "(" NT_STRINGIFY(__LINE__) ")"

#endif // NTUTILS_MACROS_H

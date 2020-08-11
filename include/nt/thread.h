#ifndef NTUTILS_THREAD_H
#define NTUTILS_THREAD_H

#ifdef _WIN32
#define thread_local __thread
#else
#include <threads.h>
#endif

#endif // NTUTILS_THREAD_H

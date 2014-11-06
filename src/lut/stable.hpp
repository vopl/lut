#ifdef __cplusplus

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <cstddef>
#include <thread>
#include <cstddef>
#include <cstddef>
#include <new>
#include <thread>
#include <mutex>
#include <atomic>
#include <iostream>

#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#if defined(__GNUC__)
#   define likely(x) __builtin_expect(!!(x),1)
#   define unlikely(x) __builtin_expect(!!(x),0)
//#else
//#   define likely(x) (!!(x))
//#   define unlikely(x) (!!(x))
//#endif


namespace std
{
    using size_t = ::size_t;
}
#endif

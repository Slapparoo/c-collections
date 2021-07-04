#ifndef _DATA_TYPES_HH
#define _DATA_TYPES_HH


#include <inttypes.h>
#include "stdio.h"

#ifdef __SUPPORT_QUADMATH_
#include <quadmath.h>
#endif

// ansi console colours
#define ANSI_RED "\033[0;31m"
#define ANSI_BOLD_RED "\033[1;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_BOLD_GREEN "\033[1;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_BOLD_YELLOW "\033[1;33m"
#define ANSI_BLUE "\033[0;34m"
#define ANSI_BOLD_BLUE "\033[1;34m"
#define ANSI_MAGENTA "\033[0;35m"
#define ANSI_BOLD_MAGENTA "\033[1;35m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_BOLD_CYAN "\033[1;36m"
#define ANSI_WHITE "\033[0;37m"
#define ANSI_BOLD_WHITE "\033[1;37m"
#define ANSI_DEFAULT "\033[0m"


typedef int64_t i64;
typedef uint64_t u64;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint16_t u16;
typedef int16_t i16;
typedef int8_t i8;
typedef uint8_t u8;
typedef uint8_t b8;
typedef float f32;
typedef double f64;
typedef enum { false, true } boolean;
typedef boolean bool;
typedef void* Pointer;
typedef char* PChar;
typedef i64 num;
typedef FILE* File;
typedef fpos_t * FilePos;

#define null ((void*)0)

#ifdef __SUPPORT_QUADMATH_
typedef __float128 f128;
typedef long double f80;
#endif

#ifndef PRINT_ERROR
#define PRINT_ERROR 1
#endif
#define printError(fmt, ...) \
        do { if (PRINT_ERROR) fprintf(stdout, ANSI_RED "(ERROR)" ANSI_BOLD_CYAN "[%s:%d]%s(): " ANSI_RED fmt "\n" ANSI_DEFAULT, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)
#ifndef PRINT_DEBUG
#define PRINT_DEBUG 1
#endif
#define printDebug(fmt, ...) \
        do { if (PRINT_DEBUG) fprintf(stdout, ANSI_CYAN "(DEBUG)" ANSI_BOLD_CYAN "[%s:%d]%s(): " ANSI_WHITE fmt "\n" ANSI_DEFAULT, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)
#ifndef PRINT_INFO
#define PRINT_INFO 1
#endif
#define printInfo(fmt, ...) \
        do { if (PRINT_INFO) fprintf(stdout, ANSI_GREEN "(INFO)" ANSI_BOLD_CYAN "[%s:%d]%s(): " ANSI_WHITE fmt "\n" ANSI_DEFAULT, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)
#ifndef PRINT_WARN
#define PRINT_WARN 1
#endif
#define printWarn(fmt, ...) \
        do { if (PRINT_WARN) fprintf(stdout, ANSI_YELLOW "(WARN)" ANSI_BOLD_CYAN "[%s:%d]%s(): " ANSI_BOLD_WHITE fmt "\n" ANSI_DEFAULT, __FILE__, \
                          __LINE__, __func__, __VA_ARGS__); } while (0)

// some usefule macros
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#endif
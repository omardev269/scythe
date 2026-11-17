// BismIllahIRRahmaanIRRaheem
/* scythe types */

#ifndef SC_TYPES_H
#define SC_TYPES_H
#include <core/platform.h>
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;
typedef float float32;
typedef double float64;
#if defined(SC_X64) || defined(SC_ARM64)
typedef uint64 largeuint;
typedef int64 largeint;
typedef float64 largefloat;
#else
typedef uint32 largeuint;
typedef int32 largeint;
typedef float32 largefloat;
#endif // defined(SC_X64) || defined(SC_ARM64)
#endif // !SC_TYPES_H
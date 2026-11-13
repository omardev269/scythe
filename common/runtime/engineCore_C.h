// BismIllahIRRahmaanIRRaheem
/* core engine definitions (C version) */

#ifndef ENGINE_CORE_C_H
#define ENGINE_CORE_C_H
#ifdef _MSC_VER
#pragma region Definitions
#endif // _MSC_VER
#include <stdbool.h>
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef float float32;
#if defined(SC_X64) || defined(SC_ARM64)
typedef unsigned long long ulargeint;
typedef long long largeint;
typedef double largefloat;
#else
typedef unsigned long ulargeint;
typedef long largeint;
typedef float largefloat;
#endif // SC_X64 || SC_ARM64
struct matrix4x4 {
	float32 m[16];
};
#ifndef __cplusplus
struct float2 {
	float32 x, y;
};
struct float3 {
	float32 x, y, z;
};
#if defined(_MSC_VER)
struct __declspec(align(16)) float4 {
#elif defined(__GNUC__) || defined(__clang__) || defined(__TINYC__)
struct __attribute__((aligned(16))) float4 {
#else
#error A 16-byte struct alignment attribute is required for float4
#endif
	float32 x, y, z, w;
};
struct int2 {
	int32 x, y;
};
struct int3 {
	int32 x, y, z;
};
struct int4 {
	int32 x, y, z, w;
};
struct uint2 {
	uint32 x, y;
};
struct uint3 {
	uint32 x, y, z;
};
struct uint4 {
	uint32 x, y, z, w;
};
#endif // !__cplusplus
#ifdef _MSC_VER
#pragma region Compiler
#endif // _MSC_VER
#if defined(__TINYC__)
#define SC_TINYC
#elif defined(__clang__)
#define SC_CLANG
#elif defined(__GNUC__)
#define SC_GCC
#elif defined(_MSC_VER)
#define SC_MSVC
#else
#define SC_UNKNOWN_COMPILER
#endif // __TINYC__
#ifdef _MSC_VER
#pragma endregion
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma region Platform
#endif // _MSC_VER
#if defined(_WIN32)
#define SC_WINDOWS
#elif defined(__ANDROID__)          // before Linux: Android also defines __linux__
#define SC_ANDROID
#elif defined(__linux__)
#define SC_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#define SC_IOS
#else
#define SC_MACOS
#endif
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || \
	  defined(__NetBSD__)  || defined(__DragonFly__)
#define SC_BSD
#else
#define SC_UNKNOWN_PLATFORM
#endif
#ifdef _MSC_VER
#pragma endregion
#endif // _MSC_VER
#ifdef SC_MSVC
#ifdef SC_RELEASE
#define SC_INLINEFUNC __forceinline
#else
#define SC_INLINEFUNC inline
#endif // SC_RELEASE
#else
#define SC_INLINEFUNC inline
#endif // SC_MSVC
#ifdef SC_DEBUG
#define SC_SPECIAL_STATIC static
#else
#define SC_SPECIAL_STATIC static SC_INLINEFUNC
#endif // SC_DEBUG
#ifdef _MSC_VER
#pragma endregion
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma region Functions
#endif // _MSC_VER
#include <stdlib.h>
#include <string.h>
#ifdef SC_WINDOWS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // SC_WINDOWS

// fun definitions

// Currently uses malloc
SC_SPECIAL_STATIC void* sc_malloc(size_t aSize) {
	return malloc(aSize);
}
// Currently uses free
SC_SPECIAL_STATIC void sc_free(void* aPtr) {
	free(aPtr);
}

SC_SPECIAL_STATIC void sc_toggle(bool* aBool) {
	*aBool = !(*aBool);
}

#define SC_TOGGLE(aBool) sc_toggle(&aBool)

// Annex-K stuff

#ifdef SC_MSVC

#include <errno.h>
#include <stdio.h>

SC_SPECIAL_STATIC errno_t sc_annexk_memset(void* aDst, size_t aDstSize, int aChar, size_t aCount) {
	volatile unsigned char* m_Dst = (volatile unsigned char*)aDst;
	size_t m_Count = aCount < aDstSize ? aCount : aDstSize;
	size_t i;
	if (!aDst) {
		return EINVAL;
	}
	for (i = 0; i < m_Count; ++i) {
		m_Dst[i] = (unsigned char)aChar;
	}
	return aCount > aDstSize ? ERANGE : 0;
}

#define SC_ANNEXK_CHECK(fn, expr) \
	do { \
		errno_t sc_err = (expr); \
		if (sc_err != 0) { \
			char sc_msg[256]; \
			sprintf_s(sc_msg, sizeof(sc_msg), \
				"Function: %s\nError code: %d", fn, (int)sc_err); \
			MessageBoxA(NULL, sc_msg, "Engine Core - CRT Error", \
				MB_OK | MB_ICONERROR | MB_TASKMODAL); \
		} \
	} while (0)

// Memory

#define sc_memcpy_s(dst, dstsz, src, count) \
	SC_ANNEXK_CHECK("memcpy_s", memcpy_s(dst, dstsz, src, count))

#define sc_memmove_s(dst, dstsz, src, count) \
	SC_ANNEXK_CHECK("memmove_s", memmove_s(dst, dstsz, src, count))

#define sc_memset_s(dst, dstsz, ch, count) \
	SC_ANNEXK_CHECK("memset_s", sc_annexk_memset(dst, dstsz, ch, count))

#define SC_ZMEM(ptr, size) sc_memset_s(ptr, size, 0, size)

// Strings

#define sc_strcpy_s(dst, dstsz, src) \
	SC_ANNEXK_CHECK("strcpy_s", strcpy_s(dst, dstsz, src))

#define sc_strncpy_s(dst, dstsz, src, count) \
	SC_ANNEXK_CHECK("strncpy_s", strncpy_s(dst, dstsz, src, count))

#define sc_strcat_s(dst, dstsz, src) \
	SC_ANNEXK_CHECK("strcat_s", strcat_s(dst, dstsz, src))

#define sc_strncat_s(dst, dstsz, src, count) \
	SC_ANNEXK_CHECK("strncat_s", strncat_s(dst, dstsz, src, count))

#define sc_strerror_s(dst, dstsz, err) \
	SC_ANNEXK_CHECK("strerror_s", strerror_s(dst, dstsz, err))

#define sc_strtok_s(str, delim, context) \
	strtok_s(str, delim, context)

// Formatted strings

#define sc_sprintf_s(dst, dstsz, ...) \
	SC_ANNEXK_CHECK("sprintf_s", sprintf_s(dst, dstsz, __VA_ARGS__) < 0 ? -1 : 0)

#define sc_snprintf_s(dst, dstsz, count, ...) \
	SC_ANNEXK_CHECK("_snprintf_s", _snprintf_s(dst, dstsz, count, __VA_ARGS__) < 0 ? -1 : 0)

// Environment

#define sc_getenv_s(required, dst, dstsz, name) \
	SC_ANNEXK_CHECK("getenv_s", getenv_s(required, dst, dstsz, name))

#else // !SC_MSVC

// Memory

#define sc_memcpy_s(dst, dstsz, src, count) \
	((void)(dstsz), memcpy(dst, src, count))

#define sc_memmove_s(dst, dstsz, src, count) \
	((void)(dstsz), memmove(dst, src, count))

#define sc_memset_s(dst, dstsz, ch, count) \
	memset(dst, ch, count)

// Strings

#define sc_strcpy_s(dst, dstsz, src) \
	((void)(dstsz), strcpy(dst, src))

#define sc_strncpy_s(dst, dstsz, src, count) \
	((void)(dstsz), strncpy(dst, src, count))

#define sc_strcat_s(dst, dstsz, src) \
	((void)(dstsz), strcat(dst, src))

#define sc_strncat_s(dst, dstsz, src, count) \
	((void)(dstsz), strncat(dst, src, count))

#define sc_strerror_s(dst, dstsz, err) \
	((void)(dstsz), (void)(err), strerror(err))

#define sc_strtok_s(str, delim, context) \
	strtok_r(str, delim, context)

// Formatted strings

#define sc_sprintf_s(dst, dstsz, ...) \
	((void)(dstsz), sprintf(dst, __VA_ARGS__))

#define sc_snprintf_s(dst, dstsz, count, ...) \
	snprintf(dst, dstsz, __VA_ARGS__)

// Environment

#define sc_getenv_s(required, dst, dstsz, name) \
	((void)(*(required) = 0), \
	 (void)(dstsz), \
	 (getenv(name) ? \
		((*(required) = strlen(getenv(name)) + 1), \
		 strcpy(dst, getenv(name)), 0) : -1))

#endif // SC_MSVC

#ifdef _MSC_VER
#pragma endregion
#endif // _MSC_VER
#endif // !ENGINE_CORE_C_H
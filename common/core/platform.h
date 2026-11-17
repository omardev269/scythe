// BismIllahIRRahmaanIRRaheem
/* platform-related definitions */

#ifndef SC_PLATFORM_H
#define SC_PLATFORM_H
#if defined(UNICODE) || defined(_UNICODE)
#define SC_UNICODE
#endif // !defined(UNICODE) || defined(_UNICODE)
#if defined(_M_X64) || defined(__x86_64__)
#define SC_X64
#elif defined(_M_IX86) || defined(__i386__)
#define SC_IA32
#elif defined(_M_ARM64) || defined(__aarch64__)
#define SC_ARM64
#elif defined(_M_ARM) || defined(__arm__)
#define SC_ARM
#endif
#if defined(__linux__) || defined(__linux) || defined(linux)
#define SC_LINUX
#endif // defined(__linux__) || defined(__linux) || defined(linux)
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__NT__)
#define SC_NT
#endif // defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__NT__)
#if defined(__DOS__) || defined(__MSDOS__) || defined(_MSDOS) || defined(MSDOS)
#define SC_DOS
#endif // defined(__DOS__) || defined(__MSDOS__) || defined(_MSDOS) || defined(MSDOS)
#if defined(SC_NT) || defined(SC_DOS)
#define SC_WINDOWS
#if defined(SC_UNICODE)
#define SC_UNICODE_16_WIDE
#endif // defined(SC_UNICODE)
#endif // defined(SC_NT) || defined(SC_DOS)
#if defined(__APPLE__) && defined(__MACH__)
#include <TargetConditionals.h>
#if defined(TARGET_OS_OSX)
#if TARGET_OS_OSX
#define SC_MACOSX
#endif // TARGET_OS_OSX
#elif !defined(TARGET_OS_IPHONE) || !TARGET_OS_IPHONE
#define SC_MACOSX
#endif // defined(TARGET_OS_OSX)
#if defined(TARGET_OS_IOS)
#if TARGET_OS_IOS
#define SC_IOS
#endif // TARGET_OS_IOS
#elif defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define SC_IOS
#endif // defined(TARGET_OS_IOS)
#endif // defined(__APPLE__) && defined(__MACH__)
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__bsdi__)
#define SC_BSD
#endif // defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__bsdi__)
#if defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
#define SC_INTEL
#if defined(__INTEL_LLVM_COMPILER)
#define SC_INTEL_LLVM
#endif // defined(__INTEL_LLVM_COMPILER)
#if defined(__INTEL_COMPILER)
#define SC_INTEL_ORIGINAL
#endif // defined(__INTEL_COMPILER)
#elif defined(__clang__)
#define SC_CLANG
#elif defined(__TINYC__)
#define SC_TINYC
#elif defined(_MSC_VER)
#define SC_MSVC
#elif defined(__GNUC__)
#define SC_GCC
#endif // defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
#endif // !SC_PLATFORM_H
// BismIllahIRRahmaanIRRaheem
/* system header includer */

#ifndef SC_SYSTEM_HEADERS_H
#define SC_SYSTEM_HEADERS_H
#include <core/platform.h>
#ifdef SC_WINDOWS
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif // !defined(WIN32_LEAN_AND_MEAN)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif // !defined(NOMINMAX)
#include <Windows.h>
#else
#include <unistd.h>
#endif // defined(SC_WINDOWS)
#endif // !SC_SYSTEM_HEADERS_H
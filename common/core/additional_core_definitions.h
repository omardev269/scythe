// BismIllahIRRahmaanIRRaheem
/* additional definitions */

#ifndef SC_ADDITIONAL_DEFINITIONS_H
#define SC_ADDITIONAL_DEFINITIONS_H
#include <core/system_headers.h>
#include <core/platform.h>
// VOLATILITY
// -
// Makes the system paranoid if 1
#define SC_VOLATILE 1

#if defined(SC_DEBUG)
#define SC_INLINEFUNC inline
#else
#if defined(SC_MSVC)
#define SC_INLINEFUNC __forceinline
#else
#define SC_INLINEFUNC inline
#endif // defined(SC_MSVC)
#endif // defined(SC_DEBUG)
#if defined(SC_DEBUG)
#define SC_SPECIAL_STATIC static
#else
#define SC_SPECIAL_STATIC static SC_INLINEFUNC
#endif // defined(SC_DEBUG)
#include <stdlib.h>
SC_SPECIAL_STATIC void* sc_alloc(size_t aSize) {
#ifdef SC_NT // VirtualAlloc is Win32 - not DOS (SC_WINDOWS just means do Windows-ey things)
	return (void*)VirtualAlloc(0, aSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	return malloc(aSize);
#endif // SC_NT
}
SC_SPECIAL_STATIC void sc_free(void* aPtr) {
#ifdef SC_NT
#if SC_VOLATILE
	if (VirtualFree(aPtr, 0, MEM_RELEASE) == 0) { // Win32 is not very consistent with return values really ngl
		OutputDebugStringA("Critical error, freeing memory FAILED\n");	// using an engine debugger-related alternative will:
																		// - pollute the header (this should be	C99-compatible btw)
																		// - may rely on the same underlying things used by this
		exit(1);
	}
#else
	VirtualFree(aPtr, 0, MEM_RELEASE);
#endif // SC_VOLATILE
#else
	free(aPtr);
#endif // SC_NT
}
#endif // !SC_ADDITIONAL_DEFINITIONS_H
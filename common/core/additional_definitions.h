// BismIllahIRRahmaanIRRaheem
/* additional definitions */

#ifndef SC_ADDITIONAL_DEFINITIONS_H
#define SC_ADDITIONAL_DEFINITIONS_H
#include <core/platform.h>
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
#if defined(__cplusplus)
#define SC_DISMISS_NOT_NULL(aPtr) { if (aPtr) { aPtr->Dismiss(); aPtr = 0; } }
#define SC_DISMISS_NOT_NULL_CONST(aPtr) { if (aPtr) { aPtr->Dismiss(); } }
#endif // defined(__cplusplus)
#include <stdlib.h>
#include <Windows.h>
SC_SPECIAL_STATIC void* sc_alloc(size_t aSize) {
	return malloc(aSize);
}
SC_SPECIAL_STATIC void sc_free(void* aPtr) {
	free(aPtr);
}
#endif // !SC_ADDITIONAL_DEFINITIONS_H
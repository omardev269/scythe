// BismIllahIRRahmaanIRRaheem
/* debug-related declarations */

#ifndef SCRTDEBUG_H
#define SCRTDEBUG_H
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <runtime/engineCore_C.h>
#ifdef SC_DEBUG
void sc_assert(const char* aExpression, const char* aFile, const char* aFunc, uint32 aLine);
void sc_log(const char* aFile, const char* aFunc, uint32 aLine, const char* aFormat, ...);
#else
SC_SPECIAL_STATIC void sc_log_disabled(const char* aFormat, ...) { (void)aFormat; }
#endif // SC_DEBUG
#ifdef SC_RELEASE
void sc_check(const char* aErrorMessage);
#endif
#ifdef SC_MSVC
#ifdef SC_DEBUG
void sc_debug_break();
#endif // SC_DEBUG
#endif // SC_MSVC

// SMART CHECK: On both debug and release
// CHECK: Release only
// ASSERT: Debug only
// LOG: Debug only
// BREAK: Debug only
// Also the assert expression will be ran whatsoever - same for the check and the smart check

#ifdef SC_DEBUG
#define SC_LOG(...) sc_log(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#else
#define SC_LOG(...) sc_log_disabled(__VA_ARGS__)
#endif // SC_DEBUG
#ifdef SC_DEBUG
#define SC_ASSERT(expr) ((expr) ? (void)0 : sc_assert(#expr, __FILE__, __FUNCTION__, __LINE__))
#else
#define SC_ASSERT(expr) expr
#endif // SC_DEBUG
#ifdef SC_RELEASE
#define SC_CHECK(expr, error) ((expr) ? (void)0 : sc_check(error))
#else
#define SC_CHECK(expr, error) expr
#endif // SC_RELEASE
#if defined(SC_DEBUG) && defined(SC_MSVC)
#define SC_BREAK() sc_debug_break()
#else
#define SC_BREAK() 
#endif // SC_DEBUG
#ifdef SC_DEBUG
#define SC_SMART_CHECK(expr, error) SC_ASSERT(expr)
#else
#define SC_SMART_CHECK(expr, error) SC_CHECK(expr, error)
#endif // SC_DEBUG

// The RET* set of macros are used to return false if the expression is not true mostly for factory functions

// to be dynamically changed
#define SC_RETURN_ON_FAILURE false

#ifdef SC_RELEASE
#define SC_RET_ASSERT(expr, error) (void)expr, (void)error
#define SC_RET_CHECK(expr, error) { if (!(expr)) { return SC_RETURN_ON_FAILURE; }}
#define SC_RET_SMART_CHECK(expr, error) { if (!(expr)) { return SC_RETURN_ON_FAILURE; }}
#else
#define SC_RET_CHECK(expr, error) (void)expr, (void)error
#define SC_RET_ASSERT(expr, error) { if (!(expr)) { SC_LOG("%s caused this: %s", #expr, error); return SC_RETURN_ON_FAILURE; }}
#define SC_RET_SMART_CHECK(expr, error) { if (!(expr)) { SC_LOG("%s caused this: %s", #expr, error); return SC_RETURN_ON_FAILURE; }}
#endif // SC_RELEASE


#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !SCRTDEBUG_H
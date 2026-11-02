// BismIllahIRRahmaanIRRaheem
/* debug functionality implementation */

#define _CRT_SECURE_NO_WARNINGS
#include <runtime/rtdebug.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#ifdef SC_MSVC
#include <intrin.h>
#endif // SC_MSVC

#define SC_EXIT_ASSERT 1
#define SC_EXIT_CHECK 2
#define SC_EXIT_FORMAT 5

#ifdef SC_DEBUG
void sc_assert(const char* aExpression, const char* aFile, const char* aFunc, uint32 aLine) {
	char m_Message[2048];
	const char* m_Title = "Assertion FAILED";
	sprintf(m_Message, "%s\n@\n%s:%s:%u", aExpression, aFile, aFunc, aLine);
#ifdef SC_WINDOWS
#ifdef SC_MSVC
	__debugbreak();
#endif // SC_MSVC
	MessageBoxA(NULL, m_Message, m_Title, MB_OK | MB_ICONERROR);
#else
	printf("%s\n%s\n", m_Title, m_Message);
#endif // SC_WINDOWS
	exit(SC_EXIT_ASSERT);
}

#ifdef SC_MSVC
void sc_debug_break() {
	__debugbreak();
}
#endif // SC_MSVC

void sc_log(const char* aFile, const char* aFunc, uint32 aLine, const char* aFormat, ...) {
	char m_Message[2048];
	size_t m_Length;
	va_list m_Args;
	if (aFormat == NULL) {
		exit(SC_EXIT_FORMAT);
	}
#ifdef SC_MSVC
	_snprintf_s(m_Message, sizeof(m_Message) - 1, _TRUNCATE, "%s:%s:%u: ", aFile, aFunc, aLine);
#else
	snprintf(m_Message, sizeof(m_Message) - 1, "%s:%s:%u: ", aFile, aFunc, aLine);
#endif // SC_MSVC
	m_Length = strlen(m_Message);
	va_start(m_Args, aFormat);
#ifdef SC_MSVC
	_vsnprintf_s(m_Message + m_Length, sizeof(m_Message) - m_Length - 1, _TRUNCATE, aFormat, m_Args);
#else
	vsnprintf(m_Message + m_Length, sizeof(m_Message) - m_Length - 1, aFormat, m_Args);
#endif // SC_MSVC
	va_end(m_Args);
	m_Length = strlen(m_Message);
	m_Message[m_Length] = '\n';
	m_Message[m_Length + 1] = '\0';
#ifdef SC_WINDOWS
	OutputDebugStringA(m_Message);
#else
	fputs(m_Message, stdout);
#endif // SC_WINDOWS
}
#else
void sc_check(const char* aErrorMessage) {
	const char* m_Title = "Fatal Error";
#ifdef SC_WINDOWS
	MessageBoxA(NULL, aErrorMessage, m_Title, MB_OK | MB_ICONERROR);
#else
	printf("%s\n%s\n", m_Title, aErrorMessage);
#endif // SC_WINDOWS
	exit(SC_EXIT_CHECK);
}
#endif // SC_DEBUG
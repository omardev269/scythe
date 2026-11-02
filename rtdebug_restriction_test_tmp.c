#include <runtime/rtdebug.h>
#include <stdlib.h>
#include <string.h>

static size_t test_length;
static const char* test_text;
static void test_output(const char* text) {
	if (test_text != NULL && strcmp(text, test_text) != 0) exit(98);
	if (strlen(text) != test_length) exit(99);
}
static int WINAPI test_message_box(HWND window, LPCSTR text, LPCSTR caption, UINT type) {
	(void)window; (void)caption; (void)type;
	test_output(text);
	return 0;
}
static void WINAPI test_debug_output(LPCSTR text) { test_output(text); }
#undef MessageBoxA
#define MessageBoxA test_message_box
#undef OutputDebugStringA
#define OutputDebugStringA test_debug_output
#include "runtime/src/rtdebug.c"

int main(int argc, char** argv) {
	char text[2042];
	if (argc != 2) return 97;
	memset(text, 'x', sizeof(text));
#ifdef SC_DEBUG
	if (strcmp(argv[1], "log") == 0) {
		test_text = "f-f:42: value %s\n";
		test_length = strlen(test_text);
		sc_log("value %s", "f", "f", 42);
		return 0;
	}
	if (strcmp(argv[1], "assert") == 0) {
		test_text = "expr\n@\nf-f:42";
		test_length = strlen(test_text);
		sc_assert("expr", "f", "f", 42);
	}
	if (strcmp(argv[1], "log-null") == 0) sc_log(NULL, "f", "f", 1);
	if (strcmp(argv[1], "assert-null") == 0) sc_assert(NULL, "f", "f", 1);
	if (strcmp(argv[1], "log-fit") == 0 || strcmp(argv[1], "log-overflow") == 0) {
		text[strcmp(argv[1], "log-fit") == 0 ? 2039 : 2040] = '\0';
		test_length = 2047;
		sc_log(text, "f", "f", 1);
		return 0;
	}
	if (strcmp(argv[1], "assert-fit") == 0 || strcmp(argv[1], "assert-overflow") == 0) {
		text[strcmp(argv[1], "assert-fit") == 0 ? 2040 : 2041] = '\0';
		test_length = 2047;
		sc_assert(text, "f", "f", 1);
	}
	if (strcmp(argv[1], "log-max-line") == 0) {
		text[2030] = '\0';
		test_length = 2047;
		sc_log(text, "f", "f", 4294967295U);
		return 0;
	}
	if (strcmp(argv[1], "assert-max-line") == 0) {
		text[2031] = '\0';
		test_length = 2047;
		sc_assert(text, "f", "f", 4294967295U);
	}
#else
	if (strcmp(argv[1], "check-null") == 0) sc_check(NULL);
	if (strcmp(argv[1], "check") == 0) {
		test_text = "error";
		test_length = strlen(test_text);
		sc_check("error");
	}
#endif
	return 96;
}

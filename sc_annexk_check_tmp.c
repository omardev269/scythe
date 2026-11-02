#include <runtime/engineCore_C.h>
#include <assert.h>
#include <string.h>

int main(void) {
	char a[64] = "abc";
	char b[64] = { 0 };
	char copy[64] = { 0 };
	char error[128];
	char tokens[] = "a,b";
	char* context = NULL;
	size_t required = 0;
	unsigned char fill[4] = { 0, 0, 0, 9 };
#if defined(SC_X64) || defined(SC_ARM64)
	assert(sizeof(largeint) == 8 && sizeof(largefloat) == 8);
#else
	assert(sizeof(largeint) == 4 && sizeof(largefloat) == 4);
#endif
	sc_memcpy_s(b, sizeof(b), a, 4);
	assert(strcmp(b, "abc") == 0);
	sc_memmove_s(b + 1, sizeof(b) - 1, b, 4);
	assert(strcmp(b, "aabc") == 0);
	sc_memset_s(fill, 3, 7, 3);
	assert(fill[0] == 7 && fill[2] == 7 && fill[3] == 9);
	assert(sc_annexk_memset(fill, 3, 5, 4) == ERANGE);
	assert(fill[0] == 5 && fill[2] == 5 && fill[3] == 9);
	assert(sc_annexk_memset(NULL, 3, 5, 1) == EINVAL);
	sc_strcpy_s(copy, sizeof(copy), "ab");
	sc_strcat_s(copy, sizeof(copy), "c");
	sc_strncat_s(copy, sizeof(copy), "de", 1);
	assert(strcmp(copy, "abcd") == 0);
	sc_strncpy_s(copy, sizeof(copy), "xyz", 2);
	assert(strcmp(copy, "xy") == 0);
	sc_strerror_s(error, sizeof(error), EINVAL);
	assert(error[0] != '\0');
	assert(strcmp(sc_strtok_s(tokens, ",", &context), "a") == 0);
	assert(strcmp(sc_strtok_s(NULL, ",", &context), "b") == 0);
	sc_sprintf_s(copy, sizeof(copy), "%s%d", "a", 12);
	assert(strcmp(copy, "a12") == 0);
	sc_snprintf_s(copy, sizeof(copy), 4, "%s", "abc");
	assert(strcmp(copy, "abc") == 0);
	sc_getenv_s(&required, copy, sizeof(copy), "SCYTHE_UNLIKELY_MISSING_ENVIRONMENT_VARIABLE");
	assert(required == 0);
	return 0;
}

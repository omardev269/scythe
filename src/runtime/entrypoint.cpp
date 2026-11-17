// BismIllahIRRahmaanIRRaheem
/* engine entry point */

#include <core/types.h>
#include <core/system_headers.h>

void scythe();

#if defined(SC_UNICODE_16_WIDE)
int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
#elif defined(SC_WINDOWS)
int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
#else
int main(int argc, char** argv) {
#endif // defined(SC_UNICODE_16_WIDE)
	scythe();
	return 0;
}
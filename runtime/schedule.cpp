// BismIllahIRRahmaanIRRaheem
/* engine runtime progression manager */

#include <runtime/engineCore.h>
#include <runtime/window.h>
#include <runtime/rtdebug.h>
#include <graphics/IDevice.h>
#include <timeint.h>
#include <stdio.h>
#ifdef SC_WINDOWS
#if defined(_UNICODE) || defined(UNICODE)
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
) {  
#else
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {
#endif // _UNICODE || UNICODE
#else
int main(
	int argc, 
	char** argv
) {
#endif // SC_WINDOWS
	sc_init_time();
	scythe::Window* p_Window = NULL;
	scythe::CreateWindowClass(p_Window, "BismIllahIRRahmaanIRRaheem", { 800, 600 });
	scythe::IGraphicsDevice* i_Device;
	scythe::CreateGraphicsDevice(i_Device, p_Window);
	while (!p_Window->ShouldClose()) {
		p_Window->PollEvents();
		i_Device->BeginFrame();
		i_Device->EndFrame();
	}
	SC_IFNOTNULLDISMISS(p_Window);
	SC_IFNOTNULLDISMISS(i_Device);
	// the time subsystem is responsible for checking its state so we don't check anything to call it
	sc_deinit_time();
	return 0;
}
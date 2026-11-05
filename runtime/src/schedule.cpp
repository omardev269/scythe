// BismIllahIRRahmaanIRRaheem
/* engine runtime progression manager */

#include <runtime/engineCore.h>
#include <runtime/window.h>
#include <runtime/rtdebug.h> 
#include <runtime/game.h>
#include <graphics/IGraphicsDevice.h>
#include <timeint.h>
#include <stdio.h>
namespace scythe {
	inline Window* g_pWindow;
	inline IGraphicsDevice* g_iGraphicsDevice;
	inline largefloat g_DeltaTime;
	static largefloat g_LastDtPoll;
}
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
	scythe::g_LastDtPoll = sc_get_time_not_since_engine_startup();
	scythe::g_DeltaTime = 0;
	scythe::g_pWindow = NULL;
	scythe::CreateWindowClass(scythe::g_pWindow, "BismIllahIRRahmaanIRRaheem", { 800, 600 });
	if (!scythe::g_pWindow) {
		sc_deinit_time();
		return 1;
	}
	scythe::g_iGraphicsDevice = NULL;
	scythe::CreateGraphicsDevice(scythe::g_iGraphicsDevice, scythe::g_pWindow);
	if (!scythe::g_iGraphicsDevice) {
		scythe::g_pWindow->Dismiss();
		sc_deinit_time();
		return 1;
	}
	start_game();
	while (true) {
		scythe::g_pWindow->PollEvents();
		scythe::g_iGraphicsDevice->BeginFrame();
		if (!update_game()) {
			break;
		}
		scythe::g_iGraphicsDevice->EndFrame();
		largefloat m_TempPoll = sc_get_time_not_since_engine_startup();
		scythe::g_DeltaTime = m_TempPoll - scythe::g_LastDtPoll;
		scythe::g_LastDtPoll = m_TempPoll;
	}
	end_game();
	SC_IFNOTNULLDISMISS(scythe::g_pWindow);
	SC_IFNOTNULLDISMISS(scythe::g_iGraphicsDevice); // many drivers need the window to be invalid before the swapchain is released
	// the time subsystem is responsible for checking its state so we don't check anything to call it
	sc_deinit_time();
	return 0;
}
// BismIllahIRRahmaanIRRaheem
/* window class declaration */

#ifndef SC_WINDOW_H
#define SC_WINDOW_H
#include <runtime/engineCore.h>
#ifdef SC_ALLOW_IMGUI
struct ImGuiContext;
#endif
namespace scythe {
	enum class Key {
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Num0,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		Up, Down, Left, Right, Space, Enter, Escape, Tab, Backspace,
		LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt
	};
	enum class MouseButton { Left, Middle, Right };
	void InitWindowSystem();
	void ShutdownWindowSystem();
	struct WindowInternal;
	// SDL window operations must run on the main thread. Readers and window
	// destruction require external coordination with concurrent writers.
	struct Window {
		WindowInternal* p_Internal;
		bool Create(const char* aTitle, uint2 aWindowSize);
		/***************************************/
		/* Polls the events for the window     */
		/* Run this before ShouldClose()       */
		/* or any other input-related function */
		/***************************************/
		void PollEvents();
		void BeginFrame();
		void EndFrame();
		bool ShouldClose();
		void GetSize(uint2& oSize);
		bool GetKey(Key aKey);
		bool GetKeyUI(Key aKey);
		bool GetMouseButton(MouseButton aButton);
		bool GetMouseButtonUI(MouseButton aButton);
		void GetMouseDelta(float2& oDelta);
		void GetMouseDeltaUI(float2& oDelta);
		void GetMousePos(float2& oPos);
		void GetMousePosUI(float2& oPos);
		void ConcentrateMouse(bool aConcentrate);
		void* GetShellHandle();
		void* GetWindowsIdentifier();
		void* GetBackendHandle();
#ifdef SC_ALLOW_IMGUI
		ImGuiContext* GetImGuiContext();
		void ImGuiImplStart();
#endif
		void Dismiss();
	};
	void CreateWindowClass(Window*& oWindow, const char* aTitle, uint2 aWindowSize);
}
#endif // !SC_WINDOW_H
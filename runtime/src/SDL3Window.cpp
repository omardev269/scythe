// BismIllahIRRahmaanIRRaheem
/* SDL3 window class implementation */

#include <runtime/window.h>
#include <SDL3/SDL.h>
#include <runtime/rtdebug.h>
#include <mutex>
#include <new>
#ifdef SC_ALLOW_IMGUI
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#endif

namespace scythe {
	struct WindowInternal {
		std::mutex m_Mutex;
		SDL_Window* p_Window;
		uint2 m_WindowSize;
		bool m_ShouldClose;
		bool m_Concentrated;
		float2 m_MouseDelta;
		float2 m_MousePos;
		float2 m_UIMousePos;
#ifdef SC_ALLOW_IMGUI
		ImGuiContext* p_ImGuiContext;
#endif
	};

	SC_SPECIAL_STATIC SDL_Scancode KeyToScancode(Key aKey) {
		if (aKey >= Key::A && aKey <= Key::Z)
			return (SDL_Scancode)(SDL_SCANCODE_A + ((int)aKey - (int)Key::A));
		if (aKey >= Key::Num1 && aKey <= Key::Num9)
			return (SDL_Scancode)(SDL_SCANCODE_1 + ((int)aKey - (int)Key::Num1));
		if (aKey >= Key::F1 && aKey <= Key::F12)
			return (SDL_Scancode)(SDL_SCANCODE_F1 + ((int)aKey - (int)Key::F1));
		switch (aKey) {
		case Key::Num0: return SDL_SCANCODE_0;
		case Key::Up: return SDL_SCANCODE_UP;
		case Key::Down: return SDL_SCANCODE_DOWN;
		case Key::Left: return SDL_SCANCODE_LEFT;
		case Key::Right: return SDL_SCANCODE_RIGHT;
		case Key::Space: return SDL_SCANCODE_SPACE;
		case Key::Enter: return SDL_SCANCODE_RETURN;
		case Key::Escape: return SDL_SCANCODE_ESCAPE;
		case Key::Tab: return SDL_SCANCODE_TAB;
		case Key::Backspace: return SDL_SCANCODE_BACKSPACE;
		case Key::LeftShift: return SDL_SCANCODE_LSHIFT;
		case Key::RightShift: return SDL_SCANCODE_RSHIFT;
		case Key::LeftControl: return SDL_SCANCODE_LCTRL;
		case Key::RightControl: return SDL_SCANCODE_RCTRL;
		case Key::LeftAlt: return SDL_SCANCODE_LALT;
		case Key::RightAlt: return SDL_SCANCODE_RALT;
		default: return SDL_SCANCODE_UNKNOWN;
		}
	}

	SC_SPECIAL_STATIC int MouseButtonToSDL(MouseButton aButton) {
		switch (aButton) {
		case MouseButton::Left: return SDL_BUTTON_LEFT;
		case MouseButton::Middle: return SDL_BUTTON_MIDDLE;
		case MouseButton::Right: return SDL_BUTTON_RIGHT;
		default: return 0;
		}
	}

	SC_SPECIAL_STATIC bool RawKey(Key aKey) {
		SDL_Scancode m_Scancode = KeyToScancode(aKey);
		if (m_Scancode == SDL_SCANCODE_UNKNOWN) return false;
		int m_Count = 0;
		const bool* m_Keys = SDL_GetKeyboardState(&m_Count);
		return m_Keys && (int)m_Scancode < m_Count && m_Keys[m_Scancode];
	}

	SC_SPECIAL_STATIC bool RawMouseButton(MouseButton aButton) {
		int m_Button = MouseButtonToSDL(aButton);
		return m_Button != 0 && (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_MASK(m_Button)) != 0;
	}

	void InitWindowSystem() {
		if (!SDL_Init(SDL_INIT_VIDEO)) SC_LOG("SDL3 failed to initialize: %s", SDL_GetError());
	}

	void ShutdownWindowSystem() {
		SDL_Quit();
	}
}

// just logs improper usage of the class
#define SC_CLASSIC_MANEUVER SC_LOG("Window class is being used improperly - p_Internal is NULL - so that means creation has failed (?)")

void scythe::Window::GetSize(uint2& oSize) {
	if (!p_Internal) {
		SC_CLASSIC_MANEUVER;
		oSize = uint2{};
		return;
	}
	oSize = p_Internal->m_WindowSize;
}

bool scythe::Window::Create(const char* aTitle, uint2 aWindowSize) {
	void* m_Storage = sc_malloc(sizeof(WindowInternal));
	if (!m_Storage) {
		p_Internal = NULL;
		return false;
	}
	WindowInternal* m_Internal = new (m_Storage) WindowInternal{};
	m_Internal->m_WindowSize = aWindowSize;
	// OpenGL context stuff takes approx 100 MBs of RAM - on windows we should disable it
#ifdef SC_WINDOWS
	m_Internal->p_Window = SDL_CreateWindow(aTitle, aWindowSize.x, aWindowSize.y, SDL_WINDOW_RESIZABLE);
#else
	m_Internal->p_Window = SDL_CreateWindow(aTitle, aWindowSize.x, aWindowSize.y, SDL_WINDOW_OPENGL);
#endif // SC_WINDOWS
	if (!m_Internal->p_Window) {
		SC_LOG("%s", SDL_GetError());
		m_Internal->~WindowInternal();
		sc_free(m_Internal);
		p_Internal = NULL;
		return false;
	}
#ifdef SC_ALLOW_IMGUI
	IMGUI_CHECKVERSION();
	m_Internal->p_ImGuiContext = ImGui::CreateContext();
	ImGui::SetCurrentContext(m_Internal->p_ImGuiContext);
	ImGui::StyleColorsClassic();
	if (!ImGui_ImplSDL3_InitForD3D(m_Internal->p_Window)) {
		ImGui::DestroyContext(m_Internal->p_ImGuiContext);
		SDL_DestroyWindow(m_Internal->p_Window);
		m_Internal->~WindowInternal();
		sc_free(m_Internal);
		p_Internal = NULL;
		return false;
	}
#endif
	if (SDL_SetWindowRelativeMouseMode(m_Internal->p_Window, true)) {
		m_Internal->m_Concentrated = true;
		SDL_HideCursor();
	}
	else {
		SC_LOG("Failed to concentrate mouse: %s", SDL_GetError());
	}
	p_Internal = m_Internal;
	SC_LOG("Window created successfully");
	return true;
}

void* scythe::Window::GetShellHandle() {
	if (!p_Internal) {
		SC_CLASSIC_MANEUVER;
		return NULL;
	}
#ifdef SC_WINDOWS
	return SDL_GetPointerProperty(SDL_GetWindowProperties(p_Internal->p_Window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
#else
	return NULL;
#endif
}

void* scythe::Window::GetWindowsIdentifier() {
	return GetShellHandle();
}

void* scythe::Window::GetBackendHandle() {
	if (!p_Internal) {
		SC_CLASSIC_MANEUVER;
		return NULL;
	}
	return p_Internal->p_Window;
}

void scythe::Window::PollEvents() {
	BeginFrame();
}

void scythe::Window::BeginFrame() {
	if (!p_Internal) {
		SC_CLASSIC_MANEUVER;
		return;
	}
	WindowInternal* m_Internal = p_Internal;
	bool m_Concentrated = m_Internal->m_Concentrated;
	bool m_Close = false;
	float2 m_Delta = {};
	SDL_Event m_Event;
	uint2 m_NewWindowSize = m_Internal->m_WindowSize;
#ifdef SC_ALLOW_IMGUI
	ImGui::SetCurrentContext(m_Internal->p_ImGuiContext);
#endif
	while (SDL_PollEvent(&m_Event)) {
#ifdef SC_ALLOW_IMGUI
		if (!m_Concentrated) ImGui_ImplSDL3_ProcessEvent(&m_Event);
#endif
		if (m_Event.type == SDL_EVENT_QUIT ||
			(m_Event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && m_Event.window.windowID == SDL_GetWindowID(m_Internal->p_Window)))
			m_Close = true;
		if (m_Event.type == SDL_EVENT_MOUSE_MOTION &&
			m_Event.motion.windowID == SDL_GetWindowID(m_Internal->p_Window)) {
			m_Delta.x = m_Event.motion.xrel;
			m_Delta.y = m_Event.motion.yrel;
		}
		if (m_Event.type == SDL_EVENT_WINDOW_RESIZED && m_Event.window.windowID == SDL_GetWindowID(m_Internal->p_Window)) {
			m_NewWindowSize.x = m_Event.window.data1;
			m_NewWindowSize.y = m_Event.window.data2;
		}
	}
	float2 m_Pos = {};
	SDL_GetMouseState(&m_Pos.x, &m_Pos.y);
	{
		std::scoped_lock<std::mutex> m_Lock(m_Internal->m_Mutex);
		if (m_Close) m_Internal->m_ShouldClose = true;
		m_Internal->m_MouseDelta = m_Delta;
		if (m_Concentrated) m_Internal->m_MousePos = m_Pos;
		else m_Internal->m_UIMousePos = m_Pos;
		m_Internal->m_WindowSize = m_NewWindowSize;
	}
}

void scythe::Window::EndFrame() {

}

bool scythe::Window::ShouldClose() {
	if (!p_Internal) {
		SC_CLASSIC_MANEUVER;
		return true;
	}
	return p_Internal->m_ShouldClose;
}

bool scythe::Window::GetKey(Key aKey) {
	return p_Internal && p_Internal->m_Concentrated && RawKey(aKey);
}

bool scythe::Window::GetKeyUI(Key aKey) {
	return p_Internal && !p_Internal->m_Concentrated && RawKey(aKey);
}

bool scythe::Window::GetMouseButton(MouseButton aButton) {
	return p_Internal && p_Internal->m_Concentrated && RawMouseButton(aButton);
}

bool scythe::Window::GetMouseButtonUI(MouseButton aButton) {
	return p_Internal && !p_Internal->m_Concentrated && RawMouseButton(aButton);
}

void scythe::Window::GetMouseDelta(float2& oDelta) {
	if (p_Internal && p_Internal->m_Concentrated) {
		oDelta = p_Internal->m_MouseDelta;
	}
	else {
		oDelta = float2{};
	}
}

void scythe::Window::GetMouseDeltaUI(float2& oDelta) {
	if (p_Internal && (!p_Internal->m_Concentrated)) {
		oDelta = p_Internal->m_MouseDelta;
	}
	else {
		oDelta = float2{};
	}
}

void scythe::Window::GetMousePos(float2& oPos) {
	oPos = p_Internal ? p_Internal->m_MousePos : float2{};
}

void scythe::Window::GetMousePosUI(float2& oPos) {
	oPos = p_Internal ? p_Internal->m_UIMousePos : float2{};
}

void scythe::Window::ConcentrateMouse(bool aConcentrate) {
	if (!p_Internal) {
		SC_CLASSIC_MANEUVER;
		return;
	}
	WindowInternal* m_Internal = p_Internal;
	if (m_Internal->m_Concentrated == aConcentrate) return;
	SDL_Window* m_Window = m_Internal->p_Window;
	bool m_Success;
	{
		std::lock_guard<std::mutex> m_Lock(m_Internal->m_Mutex);
		m_Success = SDL_SetWindowRelativeMouseMode(m_Window, aConcentrate);
		if (m_Success) {
			if (aConcentrate) SDL_HideCursor();
			else SDL_ShowCursor();
			m_Internal->m_Concentrated = aConcentrate;
			m_Internal->m_MouseDelta = {};
		}
	}
	if (!m_Success) SC_LOG("Failed to change mouse concentration: %s", SDL_GetError());
}

#ifdef SC_ALLOW_IMGUI
ImGuiContext* scythe::Window::GetImGuiContext() {
	return p_Internal ? p_Internal->p_ImGuiContext : NULL;
}

void scythe::Window::ImGuiImplStart() {
	if (!p_Internal) {
		SC_CLASSIC_MANEUVER;
		return;
	}
	ImGui::SetCurrentContext(p_Internal->p_ImGuiContext);
	ImGui_ImplSDL3_NewFrame();
}
#endif

void scythe::Window::Dismiss() {
	if (!p_Internal) {
		SC_SMART_CHECK(false, "Window class is invalid for destruction");
		return;
	}
	WindowInternal* m_Internal = p_Internal;
#ifdef SC_ALLOW_IMGUI
	ImGui::SetCurrentContext(m_Internal->p_ImGuiContext);
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext(m_Internal->p_ImGuiContext);
#endif
	SDL_DestroyWindow(m_Internal->p_Window);
	{
		std::lock_guard<std::mutex> m_Lock(m_Internal->m_Mutex);
		p_Internal = NULL;
	}
	m_Internal->~WindowInternal();
	sc_free(m_Internal);
	sc_free(this);
}

void scythe::CreateWindowClass(Window*& oWindow, const char* aTitle, uint2 aWindowSize) {
	oWindow = (Window*)sc_malloc(sizeof(Window));
	if (!oWindow) return;
	if (!oWindow->Create(aTitle, aWindowSize)) {
		sc_free(oWindow);
		oWindow = NULL;
	}
}
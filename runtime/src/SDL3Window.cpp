// BismIllahIRRahmaanIRRaheem
/* SDL3 window class implementation */

#include <runtime/window.h>
#include <SDL3/SDL.h>
#include <runtime/rtdebug.h>

namespace scythe {
	struct WindowInternal {
		SDL_Window* p_Window;
		uint2 m_WindowSize;
		bool m_ShouldClose;
	};
}

bool scythe::Window::Create(const char* aTitle, uint2 aWindowSize) {
	p_Internal = (WindowInternal*)sc_malloc(sizeof(WindowInternal));
	p_Internal->m_WindowSize = aWindowSize;
	p_Internal->m_ShouldClose = false;
	p_Internal->p_Window = NULL;
	// OpenGL context stuff takes approx 100 MBs of RAM - on windows we should disable it
#ifdef SC_WINDOWS
	p_Internal->p_Window = SDL_CreateWindow(aTitle, aWindowSize.x, aWindowSize.y, 0);
#else
	p_Internal->p_Window = SDL_CreateWindow(aTitle, aWindowSize.x, aWindowSize.y, SDL_WINDOW_OPENGL);
#endif // SC_WINDOWS
	if (!p_Internal->p_Window) {
		p_Internal = NULL;
		SC_LOG("%s", SDL_GetError());
		return false;
	}
	SC_LOG("Window created successfully");
	return true;
}

// just logs improper usage of the class
#define SC_CLASSIC_MANEUVER SC_LOG("Window class is being used improperly - p_Internal is NULL - so that means creation has failed (?)")
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

void* scythe::Window::GetBackendHandle() {
	if (!p_Internal) {
		SC_CLASSIC_MANEUVER;
		return NULL;
	}
	return p_Internal->p_Window;
}

void scythe::Window::PollEvents() {
	if (!p_Internal) {
		SC_CLASSIC_MANEUVER;
		return;
	}
	SDL_Event m_Event;
	p_Internal->m_ShouldClose = false;
	while (SDL_PollEvent(&m_Event)) {
		if (m_Event.type == SDL_EVENT_QUIT) {
			p_Internal->m_ShouldClose = true;
		}
	}
}

bool scythe::Window::ShouldClose() {
	if (!p_Internal) {
		SC_CLASSIC_MANEUVER;
		return true;
	}
	return p_Internal->m_ShouldClose;
}

void scythe::Window::Dismiss() {
	if (!p_Internal) {
		SC_SMART_CHECK(false, "Window class is invalid for destruction");
		return;
	}
	SDL_DestroyWindow(p_Internal->p_Window);
	sc_free(p_Internal);
	p_Internal = NULL;
	sc_free(this);
}

void scythe::CreateWindowClass(Window*& oWindow, const char* aTitle, uint2 aWindowSize) {
	oWindow = (Window*)sc_malloc(sizeof(Window));
	if (!oWindow->Create(aTitle, aWindowSize)) {
		sc_free(oWindow);
		oWindow = NULL;
	}
}
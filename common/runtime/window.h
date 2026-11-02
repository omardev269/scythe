// BismIllahIRRahmaanIRRaheem
/* window class declaration */

#ifndef SC_WINDOW_H
#define SC_WINDOW_H
#include <runtime/engineCore.h>
namespace scythe {
	struct WindowInternal;
	struct Window {
		WindowInternal* p_Internal;
		bool Create(const char* aTitle, uint2 aWindowSize);
		/***************************************/
		/* Polls the events for the window     */
		/* Run this before ShouldClose()       */
		/* or any other input-related function */
		/***************************************/
		void PollEvents();
		bool ShouldClose();
		void* GetShellHandle();
		void* GetBackendHandle();
		void Dismiss();
	};
	void CreateWindowClass(Window*& oWindow, const char* aTitle, uint2 aWindowSize);
}
#endif // !SC_WINDOW_H
// BismIllahIRRahmaanIRRaheem
/* header for core graphics functionality */

#ifndef GRAPHICS_CORE_H
#define GRAPHICS_CORE_H
#ifdef __cplusplus
#include <runtime/engineCore.h>
extern "C" {
#else
#include <runtime/engineCore_C.h>
#endif // __cplusplus
#ifdef SC_WINDOWS
#define SC_ALLOW_D3D11
#endif // SC_WINDOWS
enum SC_GRAPHICS_API {
	SC_GRAPHICS_API_NULL,
	SC_GRAPHICS_API_D3D11
};
// This initially checks the graphics api supported by the system and sets it to the most suitable one available
void sc_init_graphics_api_selector();
// Only for internal use
void sc_reconsider_graphics_api(enum SC_GRAPHICS_API aNewGraphicsAPI);
enum SC_GRAPHICS_API sc_get_graphics_api();
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !GRAPHICS_CORE_H
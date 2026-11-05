// BismIllahIRRahmaanIRRaheem
/* engine runtime subsystem decls */

#ifndef SC_EXPOSITION_H
#define SC_EXPOSITION_H
#include <graphics/IGraphicsDevice.h>
namespace scythe {
	inline Window* g_pWindow;
	inline IGraphicsDevice* g_iGraphicsDevice;
	inline largefloat g_DeltaTime;
}
#endif // !SC_EXPOSITION_H
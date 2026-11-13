// BismIllahIRRahmaanIRRaheem
/* graphics device interface declaration */

#ifndef IGRAPHICS_DEVICE_H
#define IGRAPHICS_DEVICE_H
#include <graphics/graphicsCore.h>
#include <runtime/IUpdateable.h>
#include <runtime/window.h>
namespace scythe {
	struct IGraphicsDevice : IUpdateable {
		// should return false if the backend is not supported
		virtual bool Create(Window* apWindow) = 0;
		virtual void Reset() = 0;
	};
	void CreateGraphicsDevice(IGraphicsDevice*& oDevice, Window* apWindow);
}
#endif // !IGRAPHICS_DEVICE_H
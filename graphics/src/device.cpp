// BismIllahIRRahmaanIRRaheem
/* device class loader */

#include <graphics/IGraphicsDevice.h>
#include <runtime/rtdebug.h>
#ifdef SC_ALLOW_D3D11
#include <d3d11device.h>
#endif // SC_ALLOW_D3D11

void scythe::CreateGraphicsDevice(IGraphicsDevice*& oDevice, Window* apWindow) {
#ifdef SC_ALLOW_D3D11 
	{
		oDevice = new D3D11GraphicsDevice;
		bool m_Result = oDevice->Create(apWindow);
		if (!m_Result) {
			SC_LOG("Failed to create D3D11 graphics device");
			oDevice = nullptr;
		}
		if (oDevice) {
			SC_LOG("D3D11 graphics device created successfully - thankfully");
		}
		if (!oDevice) {
			SC_LOG("Failed to create D3D11 graphics device");
		}
	}
#else
	oDevice = nullptr;
#endif
};
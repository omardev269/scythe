// BismIllahIRRahmaanIRRaheem
/* d3d11 device class declaration */

#ifndef SC_D3D11_DEVICE_H
#define SC_D3D11_DEVICE_H
#include <graphics/IDevice.h>
#ifdef SC_ALLOW_D3D11
#include <d3d11.h>
#include <mutex>

namespace scythe {
	struct SpecialContextLock {
		std::unique_lock<std::mutex> m_UniqueLock;
		ID3D11DeviceContext* i_DeviceContext;
	};
	class D3D11GraphicsDevice : public IGraphicsDevice {
		// --HANDLES
		HMODULE h_D3d11Dll;
		HWND h_Window;
		// --D3D INTERFACES
		ID3D11Device* i_Device;
		ID3D11DeviceContext* i_DeviceContext;
		ID3D11RenderTargetView* i_MasterRenderTargetView;
		ID3D11DepthStencilView* i_DepthStencilView;
		ID3D11DepthStencilState* i_DepthStencilStateEnabled;
		ID3D11DepthStencilState* i_DepthStencilStateDisabled;
		ID3D11RasterizerState* i_RasterizerState;
		// --DXGI INTERFACES
		IDXGIDevice* i_DxgiDevice;
		IDXGIAdapter* i_DxgiAdapter;
		IDXGIFactory* i_DxgiFactory;
		IDXGISwapChain* i_Swapchain;
		// --CONTEXT MUTEX
		std::mutex h_ContextMutex;
		// --OPERATIONAL VARIABLES
		float4 m_ClearColor;
		D3D11_VIEWPORT m_Viewport;
		RECT m_OldRect;
		// --EXTRA INFORMATION
		bool m_Warp;
		unsigned long long m_VRAMBytes; // VRAM (B) can overflow easily on 32-bit builds
		D3D_FEATURE_LEVEL m_FeatureLevel;
	public:
		bool Create(Window* apWindow) override;
		void BeginFrame() override;
		void EndFrame() override;
		void Dismiss() override;
		// --NON-VIRTUAL FUNCTIONS
		SpecialContextLock OwnContext();
		void DisownContext(SpecialContextLock& aLock);
		SC_INLINEFUNC bool IsWarp() const { return m_Warp; };
		SC_INLINEFUNC D3D_FEATURE_LEVEL GetFeatureLevel() const { return m_FeatureLevel; };
	};
}
#endif // SC_ALLOW_D3D11
#endif // !SC_D3D11_DEVICE_H 
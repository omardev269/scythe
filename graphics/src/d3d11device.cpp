// BismIllahIRRahmaanIRRaheem
/* d3d11 device implementation */

#include <d3d11device.h>
#ifdef SC_ALLOW_D3D11
#include <runtime/rtdebug.h>
#include <string>

#define SC_IFNOTNULLRELEASE(ptr) do { if ((ptr)) { (ptr)->Release(); (ptr) = NULL; } } while (0)

bool scythe::D3D11GraphicsDevice::Create(Window* apWindow) {
	{
		h_Window = NULL;
		h_D3d11Dll = NULL;
		i_Device = NULL;
		i_DeviceContext = NULL;
		i_MasterRenderTargetView = NULL;
		i_DepthStencilView = NULL;
		i_DepthStencilStateEnabled = NULL;
		i_DepthStencilStateDisabled = NULL;
		i_RasterizerState = NULL;
		i_TransparencyBlendState = NULL;
		i_DxgiAdapter = NULL;
		i_DxgiDevice = NULL;
		i_DxgiFactory = NULL;
		i_Swapchain = NULL;
		m_Warp = false;
		m_DepthEnabled = true;
		m_ClearColor = { 0.4f, 0.6f, 1.0f, 1.0f };
		SC_ZMEM(&m_Viewport, sizeof(m_Viewport));
		SC_ZMEM(&m_OldRect, sizeof(m_OldRect));
	}
	if (!apWindow) {
		SC_LOG("apWindow is NULL");
		return false;
	}
	h_Window = (HWND)apWindow->GetShellHandle();
	if (!h_Window) {
		SC_LOG("Failed to get shell handle from window");
		return false;
	}
#if defined(SC_DEBUG) && defined(SC_WINDOWS)
	system("echo Hook into RenderDoc && pause");
#endif // SC_DEBUG && SC_WINDOWS
	h_D3d11Dll = LoadLibraryA("d3d11.dll");
	if (!h_D3d11Dll) {
		SC_LOG("Failed to load d3d11.dll");
		return false;
	}
	PFN_D3D11_CREATE_DEVICE pfn_D3D11CreateDevice = (PFN_D3D11_CREATE_DEVICE)GetProcAddress(h_D3d11Dll, "D3D11CreateDevice");
	uint32 m_Flags = 0;
#ifdef SC_DEBUG
	m_Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // SC_DEBUG
	if (FAILED(pfn_D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, m_Flags, NULL, 0, D3D11_SDK_VERSION, &i_Device, &m_FeatureLevel, &i_DeviceContext))) {
		SC_LOG("Failed to create D3D11 device and context with special flags");
		if (FAILED(pfn_D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &i_Device, &m_FeatureLevel, &i_DeviceContext))) {
			SC_LOG("Hardware acceleration unavailable");
			if (FAILED(pfn_D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &i_Device, &m_FeatureLevel, &i_DeviceContext))) {
				SC_LOG("Failed to create D3D11 device and context");
				return false;
			}
		}
	}
	// using manual creation we need to get the adapter and factory for the swapchain which is pretty good as exercise
	SC_RET_SMART_CHECK(SUCCEEDED(i_Device->QueryInterface(IID_PPV_ARGS(&i_DxgiDevice))), "Failed to query for the DXGI device");
	SC_RET_SMART_CHECK(SUCCEEDED(i_DxgiDevice->GetAdapter(&i_DxgiAdapter)), "Failed to get a DXGI adapter");
	{
		DXGI_ADAPTER_DESC m_AdapterDesc;
		SC_ZMEM(&m_AdapterDesc, sizeof(DXGI_ADAPTER_DESC));
		i_DxgiAdapter->GetDesc(&m_AdapterDesc);
		SC_LOG("VRAM: %f GB (%zu KiB)", ((m_AdapterDesc.DedicatedVideoMemory / 1024.0f) / 1024.0f) / 1024.0f, (m_AdapterDesc.DedicatedVideoMemory / 1024));
		SC_LOG("Using physical device: %ls", m_AdapterDesc.Description);
		std::wstring m_DescWStr(m_AdapterDesc.Description);
		m_Warp = m_DescWStr == L"Microsoft Basic Render Driver";
	}
	SC_RET_SMART_CHECK(SUCCEEDED(i_DxgiAdapter->GetParent(IID_PPV_ARGS(&i_DxgiFactory))), "Failed to get a DXGI factory");
	{
		DXGI_SWAP_CHAIN_DESC m_SwapchainDesc;
		SC_ZMEM(&m_SwapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		// a lot of stuff in BufferDesc can be automatically filled by the system
		m_SwapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_SwapchainDesc.OutputWindow = h_Window; // fills for us the width and height of the window
		m_SwapchainDesc.SampleDesc.Count = 1;
		m_SwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_SwapchainDesc.Windowed = TRUE;
		m_SwapchainDesc.BufferCount = 1;
		SC_RET_SMART_CHECK(SUCCEEDED(i_DxgiFactory->CreateSwapChain(i_Device, &m_SwapchainDesc, &i_Swapchain)), "Failed to create a swapchain");
	}
	{
		ID3D11Texture2D* i_DxgiTexture;
		SC_RET_SMART_CHECK(SUCCEEDED(i_Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&i_DxgiTexture)), "DXGI RTO texture recieval failed");
		SC_RET_SMART_CHECK(SUCCEEDED(i_Device->CreateRenderTargetView(i_DxgiTexture, NULL, &i_MasterRenderTargetView)), "Main RTV creation failed");
		// views hold the pointer to their resource
		i_DxgiTexture->Release();
	}
	{
		RECT m_WindowRect;
		GetClientRect(h_Window, &m_WindowRect);
		m_Viewport.Width = static_cast<FLOAT>(m_WindowRect.right - m_WindowRect.left);
		m_Viewport.Height = static_cast<FLOAT>(m_WindowRect.bottom - m_WindowRect.top);
		m_Viewport.MaxDepth = 1.0f;
		D3D11_TEXTURE2D_DESC m_DepthTextureDesc;
		SC_ZMEM(&m_DepthTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		m_DepthTextureDesc.Width = (uint32)(m_WindowRect.right - m_WindowRect.left);
		m_DepthTextureDesc.Height = (uint32)(m_WindowRect.bottom - m_WindowRect.top);
		m_DepthTextureDesc.MipLevels = 1;
		m_DepthTextureDesc.ArraySize = 1;
		m_DepthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		m_DepthTextureDesc.SampleDesc.Count = 1;
		m_DepthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		ID3D11Texture2D* i_DepthTexture = NULL;
		SC_RET_SMART_CHECK(SUCCEEDED(i_Device->CreateTexture2D(&m_DepthTextureDesc, NULL, &i_DepthTexture)), "Depth texture creation failed");
		HRESULT m_DepthStencilViewResult = i_Device->CreateDepthStencilView(i_DepthTexture, NULL, &i_DepthStencilView);
		i_DepthTexture->Release();
		SC_RET_SMART_CHECK(SUCCEEDED(m_DepthStencilViewResult), "Depth stencil view creation failed");
	}
	{
		D3D11_DEPTH_STENCIL_DESC m_DssEnabledDesc;
		SC_ZMEM(&m_DssEnabledDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		m_DssEnabledDesc.DepthEnable = TRUE;
		m_DssEnabledDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		m_DssEnabledDesc.DepthFunc = D3D11_COMPARISON_LESS;
		SC_RET_SMART_CHECK(SUCCEEDED(i_Device->CreateDepthStencilState(&m_DssEnabledDesc, &i_DepthStencilStateEnabled)), "Depth stencil state creation failed");
	}
	{
		D3D11_DEPTH_STENCIL_DESC m_DssDisabledDesc;
		SC_ZMEM(&m_DssDisabledDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		SC_RET_SMART_CHECK(SUCCEEDED(i_Device->CreateDepthStencilState(&m_DssDisabledDesc, &i_DepthStencilStateDisabled)), "Depth stencil state creation failed");
	}
	{
		D3D11_RASTERIZER_DESC m_RasterizerDesc;
		SC_ZMEM(&m_RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
		m_RasterizerDesc.CullMode = D3D11_CULL_BACK;
		m_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
		m_RasterizerDesc.FrontCounterClockwise = TRUE;
		SC_RET_SMART_CHECK(SUCCEEDED(i_Device->CreateRasterizerState(&m_RasterizerDesc, &i_RasterizerState)), "Rasterizer state creation failed");
	}
	{
		D3D11_BLEND_DESC m_BlendDesc;
		SC_ZMEM(&m_BlendDesc, sizeof(m_BlendDesc));
		D3D11_RENDER_TARGET_BLEND_DESC& r_BlendTarget = m_BlendDesc.RenderTarget[0];
		r_BlendTarget.BlendEnable = TRUE;
		r_BlendTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		r_BlendTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		r_BlendTarget.BlendOp = D3D11_BLEND_OP_ADD;
		r_BlendTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
		r_BlendTarget.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		r_BlendTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		r_BlendTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		SC_RET_SMART_CHECK(SUCCEEDED(i_Device->CreateBlendState(&m_BlendDesc, &i_TransparencyBlendState)), "Transparency blend state creation failed");
	}
	return true;
}

namespace scythe {
	// context must be locked when called
	SC_SPECIAL_STATIC void ReconsiderSwapchain(IDXGISwapChain** piSwapchain, IDXGIFactory* aiDxgiFactory, ID3D11Device* aiDevice, HWND ahWindow) {
		SC_IFNOTNULLRELEASE(*piSwapchain);
		DXGI_SWAP_CHAIN_DESC m_SwapchainDesc;
		SC_ZMEM(&m_SwapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		// a lot of stuff in BufferDesc can be automatically filled by the system
		m_SwapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_SwapchainDesc.OutputWindow = ahWindow; // fills for us the width and height of the window
		m_SwapchainDesc.SampleDesc.Count = 1;
		m_SwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_SwapchainDesc.Windowed = TRUE;
		m_SwapchainDesc.BufferCount = 1;
		SC_SMART_CHECK(SUCCEEDED(aiDxgiFactory->CreateSwapChain(aiDevice, &m_SwapchainDesc, piSwapchain)), "Failed to create a swapchain");
	}
	SC_SPECIAL_STATIC void ReconsiderRenderTargetView(ID3D11RenderTargetView** piRtv, ID3D11Device* aiDevice, IDXGISwapChain* aiSwapchain) {
		SC_IFNOTNULLRELEASE(*piRtv);
		ID3D11Texture2D* i_DxgiTexture;
		SC_SMART_CHECK(SUCCEEDED(aiSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&i_DxgiTexture)), "DXGI RTO texture recieval failed");
		SC_SMART_CHECK(SUCCEEDED(aiDevice->CreateRenderTargetView(i_DxgiTexture, NULL, piRtv)), "Main RTV creation failed");
		i_DxgiTexture->Release();
	}
	SC_SPECIAL_STATIC void ReconsiderDepthStencilView(ID3D11DepthStencilView** piDsv, ID3D11Device* aiDevice, HWND ahWindow) {
		SC_IFNOTNULLRELEASE(*piDsv);
		RECT m_WindowRect;
		GetClientRect(ahWindow, &m_WindowRect);
		D3D11_TEXTURE2D_DESC m_DepthTextureDesc;
		SC_ZMEM(&m_DepthTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		m_DepthTextureDesc.Width = (uint32)(m_WindowRect.right - m_WindowRect.left);
		m_DepthTextureDesc.Height = (uint32)(m_WindowRect.bottom - m_WindowRect.top);
		m_DepthTextureDesc.MipLevels = 1;
		m_DepthTextureDesc.ArraySize = 1;
		m_DepthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		m_DepthTextureDesc.SampleDesc.Count = 1;
		m_DepthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		ID3D11Texture2D* i_DepthTexture = NULL;
		SC_SMART_CHECK(SUCCEEDED(aiDevice->CreateTexture2D(&m_DepthTextureDesc, NULL, &i_DepthTexture)), "Depth texture creation failed");
		HRESULT m_DepthStencilViewResult = aiDevice->CreateDepthStencilView(i_DepthTexture, NULL, piDsv);
		i_DepthTexture->Release();
		SC_SMART_CHECK(SUCCEEDED(m_DepthStencilViewResult), "Depth stencil view creation failed");
	}
}

scythe::SpecialContextLock scythe::D3D11GraphicsDevice::OwnContext() {
	SpecialContextLock m_Lock;
	m_Lock.m_UniqueLock = std::unique_lock<std::mutex>(h_ContextMutex);
	m_Lock.i_DeviceContext = i_DeviceContext;
	return m_Lock;
}

void scythe::D3D11GraphicsDevice::DisownContext(SpecialContextLock& aLock) {
	aLock.m_UniqueLock.unlock();
	aLock.i_DeviceContext = NULL;
}

void scythe::D3D11GraphicsDevice::BindDeviceDefaults(ID3D11DeviceContext* aiContext) {
	aiContext->OMSetRenderTargets(1, &i_MasterRenderTargetView, i_DepthStencilView);
	aiContext->RSSetState(i_RasterizerState);
	aiContext->RSSetViewports(1, &m_Viewport);
	aiContext->OMSetBlendState(i_TransparencyBlendState, NULL, 0xFFFFFFFF);
	aiContext->OMSetDepthStencilState(m_DepthEnabled ? i_DepthStencilStateEnabled : i_DepthStencilStateDisabled, 0);
}

void scythe::D3D11GraphicsDevice::SetDepth(bool aEnabled) {
	SpecialContextLock m_Lock = OwnContext();
	m_DepthEnabled = aEnabled;
	m_Lock.i_DeviceContext->OMSetDepthStencilState(m_DepthEnabled ? i_DepthStencilStateEnabled : i_DepthStencilStateDisabled, 0);
	DisownContext(m_Lock);
}

void scythe::D3D11GraphicsDevice::Reset() {
	SpecialContextLock m_Lock = OwnContext();
	m_Lock.i_DeviceContext->ClearState();
	m_DepthEnabled = true;
	BindDeviceDefaults(m_Lock.i_DeviceContext);
	DisownContext(m_Lock);
}

void scythe::D3D11GraphicsDevice::BeginFrame() {
	{
		SpecialContextLock m_Lock = OwnContext();
		// check if the window size has changed
		{
			RECT m_WindowRect;
			GetClientRect(h_Window, &m_WindowRect);
			if (memcmp(&m_WindowRect, &m_OldRect, sizeof(RECT)) != 0) {
				m_OldRect = m_WindowRect;
				SC_ZMEM(&m_Viewport, sizeof(m_Viewport));
				m_Viewport.Width = static_cast<FLOAT>(m_WindowRect.right - m_WindowRect.left);
				m_Viewport.Height = static_cast<FLOAT>(m_WindowRect.bottom - m_WindowRect.top);
				m_Viewport.MaxDepth = 1.0f;
				if (!m_Viewport.Width || !m_Viewport.Height) {
					SC_LOG("Window has zero width or height - maybe minimized?");
				}
				else {
					SC_LOG("Window size changed");
					ReconsiderSwapchain(&i_Swapchain, i_DxgiFactory, i_Device, h_Window);
					ReconsiderRenderTargetView(&i_MasterRenderTargetView, i_Device, i_Swapchain);
					ReconsiderDepthStencilView(&i_DepthStencilView, i_Device, h_Window);
				}
			}
		}
		BindDeviceDefaults(m_Lock.i_DeviceContext);
		m_Lock.i_DeviceContext->ClearRenderTargetView(i_MasterRenderTargetView, &m_ClearColor.x);
		m_Lock.i_DeviceContext->ClearDepthStencilView(i_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		DisownContext(m_Lock);
	}
}

void scythe::D3D11GraphicsDevice::EndFrame() {
	SC_SMART_CHECK(SUCCEEDED(i_Swapchain->Present(0, 0)), "Failed to present the swapchain");
}

void scythe::D3D11GraphicsDevice::Dismiss() {
	SC_IFNOTNULLRELEASE(i_TransparencyBlendState);
	SC_IFNOTNULLRELEASE(i_RasterizerState);
	SC_IFNOTNULLRELEASE(i_DepthStencilStateDisabled);
	SC_IFNOTNULLRELEASE(i_DepthStencilStateEnabled);
	SC_IFNOTNULLRELEASE(i_DepthStencilView);
	SC_IFNOTNULLRELEASE(i_MasterRenderTargetView);
	SC_IFNOTNULLRELEASE(i_DxgiFactory);
	SC_IFNOTNULLRELEASE(i_DxgiAdapter);
	SC_IFNOTNULLRELEASE(i_DxgiDevice);
	SC_IFNOTNULLRELEASE(i_Swapchain);
	SC_IFNOTNULLRELEASE(i_DeviceContext);
	SC_IFNOTNULLRELEASE(i_Device);
	if (h_D3d11Dll) {
		FreeLibrary(h_D3d11Dll);
		h_D3d11Dll = NULL;
	}
	delete this;
}
#endif // SC_ALLOW_D3D11
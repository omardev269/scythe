// BismIllahIRRahmaanIRRaheem
/* d3d11 material implementation */

#include <graphics/IMaterial.h>
#ifdef SC_ALLOW_D3D11
#include <d3d11material.h>
#include <runtime/rtdebug.h>

#define SC_ARM_D11M_DESC_LOCK() std::unique_lock h_DescLock(h_MaterialDescMutex)
#define SC_DISARM_D11M_DESC_LOCK() h_DescLock.unlock()
#define SC_REARM_D11M_DESC_LOCK() h_DescLock.lock()

#define SC_ARM_D11M_LOCK() std::unique_lock h_MaterialLock(h_MaterialMutex)
#define SC_DISARM_D11M_LOCK() h_MaterialLock.unlock()
#define SC_REARM_D11M_LOCK() h_MaterialLock.lock()

#define SC_RET_SMART_CHECK_DISMISS(expr, error) do { if (!(expr)) { Dismiss(); SC_LOG("%s", error); return false; } } while (0)
#define SC_CREATE_AND_ZERO(type, name) type name; SC_ZMEM(&name, sizeof(type))
#define SC_IFNOTNULLRELEASE(ptr) do { if ((ptr)) { (ptr)->Release(); (ptr) = NULL; } } while (0)

bool scythe::D3D11Material::Create(IGraphicsDevice* aiGraphicsDevice, MATERIAL_DESC aMaterialDesc) {
	// creation needs to be lock
	SC_ARM_D11M_LOCK();
	// checks
	{
		SC_RET_SMART_CHECK_DISMISS(aiGraphicsDevice != NULL, "Invalid graphics device");
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.TextureSize.x > 0 && aMaterialDesc.TextureSize.y > 0, "Invalid texture size");
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.PitchBytes > 0, "Invalid pitch bytes");
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.PitchBytes * aMaterialDesc.TextureSize.y > 0, "Invalid texture data size");
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.TextureSize.x <= 2048 && aMaterialDesc.TextureSize.y <= 2048, "Texture size exceeds maximum allowed dimensions");
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.pInitialData != NULL, "Initial data pointer is NULL");
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.SamplerState >= SAMPLER_STATE_NEAREST && aMaterialDesc.SamplerState <= SAMPLER_STATE_ANISOTROPIC16, "Invalid sampler state");

	}
	// initialize soft material desc
	{
		m_MaterialDesc.SamplerState = aMaterialDesc.SamplerState;
		m_MaterialDesc.TextureSize = aMaterialDesc.TextureSize;
		m_MaterialDesc.PitchBytes = aMaterialDesc.PitchBytes;
	}
	// initialize parent device
	{
		i_GraphicsDevice = static_cast<D3D11GraphicsDevice*>(aiGraphicsDevice);
	}
	// create texture
	{
		SC_CREATE_AND_ZERO(D3D11_TEXTURE2D_DESC, m_TextureDesc);
		m_TextureDesc.ArraySize = 1;
		m_TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_TextureDesc.Width = aMaterialDesc.TextureSize.x;
		m_TextureDesc.Height = aMaterialDesc.TextureSize.y;
		m_TextureDesc.MipLevels = 1;
		m_TextureDesc.SampleDesc.Count = 1;
		SC_CREATE_AND_ZERO(D3D11_SUBRESOURCE_DATA, m_InitialData);
		m_InitialData.pSysMem = aMaterialDesc.pInitialData;
		m_InitialData.SysMemPitch = aMaterialDesc.PitchBytes;
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateTexture2D(&m_TextureDesc, &m_InitialData, &i_Texture)), "Failed to create texture");
	}
	// create shader resource view
	{
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateShaderResourceView(i_Texture, NULL, &i_TextureSRV)), "Failed to create shader resource view");
	}
	// create sampler state
	{
		SC_CREATE_AND_ZERO(D3D11_SAMPLER_DESC, m_SamplerDesc);
		m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		switch (aMaterialDesc.SamplerState) {
		case SAMPLER_STATE_NEAREST:
			m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case SAMPLER_STATE_LINEAR:
			m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case SAMPLER_STATE_ANISOTROPIC2:
			m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			m_SamplerDesc.MaxAnisotropy = 2;
			break;
		case SAMPLER_STATE_ANISOTROPIC4:
			m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			if (i_GraphicsDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_0) {
				m_SamplerDesc.MaxAnisotropy = 4;
			}
			else {
				m_SamplerDesc.MaxAnisotropy = 2;
			}
			break;
		case SAMPLER_STATE_ANISOTROPIC8:
			m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			if (i_GraphicsDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_0) {
				m_SamplerDesc.MaxAnisotropy = 8;
			}
			else {
				m_SamplerDesc.MaxAnisotropy = 2;
			}
			break;
		case SAMPLER_STATE_ANISOTROPIC16:
			m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			if (i_GraphicsDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_0) {
				m_SamplerDesc.MaxAnisotropy = 16;
			}
			else {
				m_SamplerDesc.MaxAnisotropy = 2;
			}
			break;
		default:
			return false; // should never happen
		}
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateSamplerState(&m_SamplerDesc, &i_SamplerState)), "Failed to create sampler state");
	}
	return true;
}

scythe::IGraphicsDevice* scythe::D3D11Material::GetParent() {
	return i_GraphicsDevice;
}

void scythe::D3D11Material::GetDesc(MATERIAL_DESC_NOPDATA& oMaterialDesc) {
	oMaterialDesc = m_MaterialDesc;
}

void scythe::D3D11Material::UpdateData(void* apNewData) {
	SC_SMART_CHECK(apNewData != NULL, "Invalid new data pointer");
	D3D11_BOX m_Box = {};
	m_Box.left = 0;
	m_Box.top = 0;
	m_Box.front = 0;
	m_Box.right = m_MaterialDesc.TextureSize.x;
	m_Box.bottom = m_MaterialDesc.TextureSize.y;
	m_Box.back = 1;
	SC_ARM_D11M_LOCK();
	SpecialContextLock h_Lock = i_GraphicsDevice->OwnContext();
	h_Lock.i_DeviceContext->UpdateSubresource(i_Texture, 0, &m_Box, apNewData, m_MaterialDesc.PitchBytes, 0);
	// pessimized
	i_GraphicsDevice->DisownContext(h_Lock); // it is a healthy practice
}

bool scythe::D3D11Material::Recreate(MATERIAL_DESC aMaterialDesc) {
	// pessimized
	SC_ARM_D11M_LOCK();
	// releases
	{
		SC_IFNOTNULLRELEASE(i_SamplerState);
		SC_IFNOTNULLRELEASE(i_TextureSRV);
		SC_IFNOTNULLRELEASE(i_Texture); // while the RV owns a reference to the texture, we still need to release it here because we created it
	}
	// checks
	{
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.TextureSize.x > 0 && aMaterialDesc.TextureSize.y > 0, "Invalid texture size");
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.PitchBytes > 0, "Invalid pitch bytes");
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.PitchBytes * aMaterialDesc.TextureSize.y > 0, "Invalid texture data size");
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.TextureSize.x <= 2048 && aMaterialDesc.TextureSize.y <= 2048, "Texture size exceeds maximum allowed dimensions");
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.pInitialData != NULL, "Initial data pointer is NULL");
		SC_RET_SMART_CHECK_DISMISS(aMaterialDesc.SamplerState >= SAMPLER_STATE_NEAREST && aMaterialDesc.SamplerState <= SAMPLER_STATE_ANISOTROPIC16, "Invalid sampler state");

	}
	// initialize soft material desc
	{
		m_MaterialDesc.SamplerState = aMaterialDesc.SamplerState;
		m_MaterialDesc.TextureSize = aMaterialDesc.TextureSize;
		m_MaterialDesc.PitchBytes = aMaterialDesc.PitchBytes;
	}
	// create texture
	{
		SC_CREATE_AND_ZERO(D3D11_TEXTURE2D_DESC, m_TextureDesc);
		m_TextureDesc.ArraySize = 1;
		m_TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_TextureDesc.Width = aMaterialDesc.TextureSize.x;
		m_TextureDesc.Height = aMaterialDesc.TextureSize.y;
		m_TextureDesc.MipLevels = 1;
		m_TextureDesc.SampleDesc.Count = 1;
		SC_CREATE_AND_ZERO(D3D11_SUBRESOURCE_DATA, m_InitialData);
		m_InitialData.pSysMem = aMaterialDesc.pInitialData;
		m_InitialData.SysMemPitch = aMaterialDesc.PitchBytes;
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateTexture2D(&m_TextureDesc, &m_InitialData, &i_Texture)), "Failed to create texture");
	}
	// create shader resource view
	{
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateShaderResourceView(i_Texture, NULL, &i_TextureSRV)), "Failed to create shader resource view");
	}
	// create sampler state
	{
		SC_CREATE_AND_ZERO(D3D11_SAMPLER_DESC, m_SamplerDesc);
		m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		switch (aMaterialDesc.SamplerState) {
		case SAMPLER_STATE_NEAREST:
			m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case SAMPLER_STATE_LINEAR:
			m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case SAMPLER_STATE_ANISOTROPIC2:
			m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			m_SamplerDesc.MaxAnisotropy = 2;
			break;
		case SAMPLER_STATE_ANISOTROPIC4:
			m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			if (i_GraphicsDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_0) {
				m_SamplerDesc.MaxAnisotropy = 4;
			}
			else {
				m_SamplerDesc.MaxAnisotropy = 2;
			}
			break;
		case SAMPLER_STATE_ANISOTROPIC8:
			m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			if (i_GraphicsDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_0) {
				m_SamplerDesc.MaxAnisotropy = 8;
			}
			else {
				m_SamplerDesc.MaxAnisotropy = 2;
			}
			break;
		case SAMPLER_STATE_ANISOTROPIC16:
			m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			if (i_GraphicsDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_0) {
				m_SamplerDesc.MaxAnisotropy = 16;
			}
			else {
				m_SamplerDesc.MaxAnisotropy = 2;
			}
			break;
		default:
			return false; // should never happen
		}
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateSamplerState(&m_SamplerDesc, &i_SamplerState)), "Failed to create sampler state");
	}
	return true;
}

void scythe::D3D11Material::Dismiss() {
	SC_SMART_CHECK(h_MaterialMutex.try_lock(), "Thread is competing on an expired material!");
	h_MaterialMutex.unlock();
	SC_IFNOTNULLRELEASE(i_SamplerState);
	SC_IFNOTNULLRELEASE(i_TextureSRV);
	SC_IFNOTNULLRELEASE(i_Texture); // while the RV owns a reference to the texture, we still need to release it here because we created it
	delete this;
}

#endif // SC_ALLOW_D3D11

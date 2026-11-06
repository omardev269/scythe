// BismIllahIRRahmaanIRRaheem
/* d3d11 material class decl */

#ifndef SC_D3D11_MATERIAL_H
#define SC_D3D11_MATERIAL_H
#include <d3d11device.h>
#include <graphics/IMaterial.h>
#ifdef SC_ALLOW_D3D11
namespace scythe {
	class D3D11Material : public IMaterial {
		// --PARENT DEVICE
		D3D11GraphicsDevice* i_GraphicsDevice;
		// --D3D INTERFACES
		ID3D11Texture2D* i_Texture;
		ID3D11ShaderResourceView* i_TextureSRV;
		ID3D11SamplerState* i_SamplerState;
		uint32 m_PitchBytes;
		std::mutex h_MaterialMutex;
		MATERIAL_DESC_NOPDATA m_MaterialDesc;
	public:
		// zeroer (i can't use zmem because of the non-triviality of the code)
		SC_INLINEFUNC D3D11Material() {
			i_GraphicsDevice = NULL;
			i_Texture = NULL;
			i_TextureSRV = NULL;
			i_SamplerState = NULL;
			m_PitchBytes = 0;
			SC_ZMEM(&m_MaterialDesc, sizeof(MATERIAL_DESC_NOPDATA));
		}
		// --VIRTUAL FUNCTIONS
		bool Create(IGraphicsDevice* aiGraphicsDevice, MATERIAL_DESC aMaterialDesc) override;
		IGraphicsDevice* GetParent() override;
		void UpdateData(void* apNewData) override;
		bool Recreate(MATERIAL_DESC aMaterialDesc) override;
		void GetDesc(MATERIAL_DESC_NOPDATA& oMaterialDesc) override;
		void Dismiss() override;
		// --NON-VIRTUAL FUNCTIONS
		SC_INLINEFUNC ID3D11ShaderResourceView* GetSRV() {
			return i_TextureSRV;
		}
		SC_INLINEFUNC ID3D11SamplerState* GetSampler() {
			return i_SamplerState;
		}

	};
}
#endif // SC_ALLOW_D3D11
#endif // !SC_D3D11_MATERIAL_H
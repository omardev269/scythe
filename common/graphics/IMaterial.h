// BismIllahIRRahmaanIRRaheem
/* material vclass decl */

#ifndef SC_IMATERIAL_H
#define SC_IMATERIAL_H
#include <graphics/IGraphicsDevice.h>

namespace scythe
{
	enum SAMPLER_STATE {
		SAMPLER_STATE_NEAREST,
		SAMPLER_STATE_LINEAR,
		SAMPLER_STATE_ANISOTROPIC2,
		SAMPLER_STATE_ANISOTROPIC4,
		SAMPLER_STATE_ANISOTROPIC8,
		SAMPLER_STATE_ANISOTROPIC16
	};
	struct MATERIAL_DESC_NOPDATA {
		SAMPLER_STATE SamplerState;
		uint2 TextureSize; // both must be higher than 0
		uint32 PitchBytes; // PitchBytes * TextureSize.y MUST equal the size of the texture data in bytes
	};
	struct MATERIAL_DESC {
		SAMPLER_STATE SamplerState;
		uint2 TextureSize; // both must be higher than 0
		uint32 PitchBytes; // PitchBytes * TextureSize.y MUST equal the size of the texture data in bytes
		void* pInitialData; // D3D11 uses RGBA8 UNORM; UpdateData uses the same row pitch
	};
	struct IMaterial {
		virtual bool Create(IGraphicsDevice* aiGraphicsDevice, MATERIAL_DESC aMaterialDesc) = 0;
		virtual IGraphicsDevice* GetParent() = 0;
		virtual void UpdateData(void* apNewData) = 0;
		// self-destructs on failure
		virtual bool Recreate(MATERIAL_DESC aMaterialDesc) = 0;
		// note: returns a desc without pInitialData
		virtual void GetDesc(MATERIAL_DESC_NOPDATA& oMaterialDesc) = 0;
		virtual void Dismiss() = 0;
	};
	void CreateMaterial(IMaterial*& oiMaterial, IGraphicsDevice* aiGraphicsDevice, MATERIAL_DESC aMaterialDesc);
}

#endif // !SC_IMATERIAL_H
// BismIllahIRRahmaanIRRaheem
/* IMaterial factory */

#include <graphics/IMaterial.h>
#ifdef SC_ALLOW_D3D11
#include <d3d11material.h>
#endif // SC_ALLOW_D3D11

void scythe::CreateMaterial(IMaterial*& oiMaterial, IGraphicsDevice* aiGraphicsDevice, MATERIAL_DESC aMaterialDesc)
{
	oiMaterial = nullptr;
#ifdef SC_ALLOW_D3D11
	if (!aiGraphicsDevice) {
		return;
	}
	D3D11Material* iMaterial = new D3D11Material;
	if (!iMaterial->Create(aiGraphicsDevice, aMaterialDesc)) {
		delete iMaterial;
		return;
	}
	oiMaterial = iMaterial;
#endif // SC_ALLOW_D3D11
}

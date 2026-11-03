// BismIllahIRRahmaanIRRaheem
/* scene factory */

#include <graphics/IGraphicsScene.h>
#ifdef SC_ALLOW_D3D11
#include <d3d11scene.h>
#endif // SC_ALLOW_D3D11

void scythe::CreateGraphicsScene(IGraphicsScene*& oiGraphicsScene, IGraphicsDevice* aiGraphicsDevice) {
	oiGraphicsScene = nullptr;
#ifdef SC_ALLOW_D3D11
	if (!aiGraphicsDevice) {
		return;
	}
	D3D11GraphicsScene* i_Scene = new D3D11GraphicsScene;
	if (!i_Scene->Create(aiGraphicsDevice)) {
		return;
	}
	oiGraphicsScene = i_Scene;
#endif // SC_ALLOW_D3D11
}

// BismIllahIRRahmaanIRRaheem
/* pipeline factory */

#include <graphics/IGraphicsPipeline.h>
#include <runtime/rtdebug.h>
#ifdef SC_ALLOW_D3D11
#include <d3d11albedographicspipeline.h>
#include <d3d11phonggraphicspipeline.h>
#endif // SC_ALLOW_D3D11

void scythe::CreateAlbedoGraphicsPipeline(IGraphicsPipeline*& oiPipeline, IGraphicsDevice* aiRegistoire) {
	oiPipeline = nullptr;
#ifdef SC_ALLOW_D3D11
	if (!dynamic_cast<D3D11GraphicsDevice*>(aiRegistoire)) {
		SC_LOG("Invalid graphics device for D3D11 albedo pipeline");
		return;
	}
	D3D11AlbedoPipeline* i_Pipeline = new D3D11AlbedoPipeline;
	if (!i_Pipeline->Create(aiRegistoire)) {
		i_Pipeline->Dismiss();
		return;
	}
	oiPipeline = i_Pipeline;
#endif // SC_ALLOW_D3D11
}
void scythe::CreatePhongGraphicsPipeline(IGraphicsPipeline*& oiPipeline, IGraphicsDevice* aiRegistoire) {
	oiPipeline = nullptr;
#ifdef SC_ALLOW_D3D11
	if (!dynamic_cast<D3D11GraphicsDevice*>(aiRegistoire)) {
		SC_LOG("Invalid graphics device for D3D11 phong pipeline");
		return;
	}
	D3D11PhongPipeline* i_Pipeline = new D3D11PhongPipeline;
	if (!i_Pipeline->Create(aiRegistoire)) {
		i_Pipeline->Dismiss();
		return;
	}
	oiPipeline = i_Pipeline;
#endif // SC_ALLOW_D3D11
}

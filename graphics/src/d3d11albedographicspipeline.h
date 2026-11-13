// BismIllahIRRahmaanIRRaheem
/* d3d11 albedo pipeline decl */

#ifndef SC_D3D11ALBEDOPIPELINE_H
#define SC_D3D11ALBEDOPIPELINE_H
#include <ID3D11GraphicsPipeline.h>
#ifdef SC_ALLOW_D3D11
namespace scythe {
	class D3D11AlbedoPipeline : public ID3D11GraphicsPipeline {
		ID3D11VertexShader* i_VertexShader = NULL;
		ID3D11PixelShader* i_PixelShader = NULL;
		ID3D11InputLayout* i_InputLayout = NULL;
		ID3D11Buffer* i_TransformBuffer = NULL;
		ID3D11Texture2D* i_RenderTexture = NULL;
		ID3D11ShaderResourceView* i_RenderTextureView = NULL;
		ID3D11RenderTargetView* i_RenderTargetView = NULL;
		bool m_HasRendered = false;
		bool EnsureRenderTarget(ID3D11DeviceContext* aiContext, UINT aWidth, UINT aHeight, bool& aRecreated);
		void ReactionForTransformChange() override;
	public:
		bool Create(IGraphicsDevice* aiRegistoire) override;
		using ID3D11GraphicsPipeline::Render;
		void Render(std::function<void()> ahDraw, bool aDepthEnabled) override;
		void SetLights(const std::vector<LIGHT_METADATA>& aMetadata) override;
		void Update() override;
		void Dismiss() override;
	};
}
#endif // SC_ALLOW_D3D11
#endif // !SC_D3D11ALBEDOPIPELINE_H
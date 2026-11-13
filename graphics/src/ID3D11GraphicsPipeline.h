// BismIllahIRRahmaanIRRaheem
/* d3d11 pipeline vclass decl*/

#ifndef SC_D3D11COREPIPELINE_H
#define SC_D3D11COREPIPELINE_H
#include <graphics/IGraphicsPipeline.h>
#include <graphics/IGraphicsScene.h>
#include <functional>
#include <vector>
#ifdef SC_ALLOW_D3D11
#include <d3d11device.h>
namespace scythe {
	class ID3D11GraphicsPipeline : public IGraphicsPipeline {
	protected:
		D3D11GraphicsDevice* i_Device;
		ID3D11ShaderResourceView* i_Texture;
		ID3D11SamplerState* i_Sampler;
		matrix4x4 m_Transform;
		std::vector<LIGHT_METADATA> m_Lights;
		// -
		virtual void ReactionForTransformChange() = 0;
	public:
		virtual void Render(std::function<void()> ahDraw, bool aDepthEnabled) = 0;
		void Render(std::function<void()> ahDraw) { Render(ahDraw, true); }
		SC_INLINEFUNC D3D11GraphicsDevice* GetDevice() const { return i_Device; }
		SC_INLINEFUNC void SetTransform(const matrix4x4& aTransform) {
			m_Transform = aTransform;
			ReactionForTransformChange();
		}
		virtual void SetLights(const std::vector<LIGHT_METADATA>& aMetadata) = 0;
		SC_INLINEFUNC void SetRenderTexture(ID3D11ShaderResourceView* aiTexture) {
			i_Texture = aiTexture;
		}
		SC_INLINEFUNC void SetRenderSampler(ID3D11SamplerState* aiSampler) {
			i_Sampler = aiSampler;
		}
	};
}
#endif // SC_ALLOW_D3D11
#endif // !SC_D3D11COREPIPELINE_H
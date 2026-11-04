// BismIllahIRRahmaanIRRaheem
/* scene class derivative decl */

#ifndef SC_D3D11_SCENE_H
#define SC_D3D11_SCENE_H
#include <graphics/IGraphicsScene.h>
#include <d3d11device.h>
#include <vector>

#ifdef SC_ALLOW_D3D11
namespace scythe
{
	struct ClearClumpD3D11 {
		CLUMP_METADATA Metadata;
	};
	// must be allocated by new and deallocated by delete
	struct D3D11Object {
		ID3D11Buffer* i_VertexBuffer;
		ID3D11Buffer* i_IndexBuffer;
		ulargeint m_VertexBufferBytes;
		ulargeint m_IndexBufferBytes;
		uint32 m_Stride;
		std::vector<ClearClumpD3D11> m_Clumps;
	};
	class D3D11GraphicsScene : public IGraphicsScene {
		// --GRAPHICS DEVICE
		D3D11GraphicsDevice* i_GraphicsDevice;
		// --D3D INTERFACES
		ID3D11VertexShader* i_AlbedoVertexShader;
		ID3D11PixelShader* i_AlbedoPixelShader;
		ID3D11Buffer* i_AlbedoConstantBuffer;
		ID3D11InputLayout* i_InputLayout;
		ID3D11Texture2D* i_WhiteTexture;
		ID3D11ShaderResourceView* i_WhiteTextureSRV;
		ID3D11SamplerState* i_WhiteSampler;
		// --OPERATION VARIABLES
		std::mutex h_GeneralMutex;
		std::vector<D3D11Object*> m_Objects;

	public:
		bool Create(IGraphicsDevice* aiGraphicsDevice) override;
		ObjectID CreateObject(ulargeint aVertexBufferBytes, ulargeint aIndexBufferBytes, uint32 aStride) override;
		ClumpID CreateClump(const CLUMP_METADATA& aMetadata, float32* apVertexStartData, uint16* apIndexStartData) override;
		void UpdateClump(ClumpID aClump, const matrix4x4& aTransform) override;
		void UpdateClumpNumVertices(ClumpID aClump, uint16 aNumVertices) override;
		void UpdateClumpNumIndices(ClumpID aClump, uint32 aNumIndices) override;
		void UpdateClump(ClumpID aClump, float32* apVertexData, ulargeint aCurrentAttemptedAllocationSize) override;
		void UpdateClump(ClumpID aClump, uint16* apIndexData, ulargeint aCurrentAttemptedAllocationSize) override;
		void DismissClump(ClumpID& aClump) override;
		void DismissObject(ObjectID aObject) override;
		IGraphicsDevice* GetParent() override;
		void AlbedoRenderExcept(ObjectID aObject, const matrix4x4& aPV) override;
		void AlbedoRenderExcept(const matrix4x4& aPV, ObjectID aObject) override;
		void AlbedoRenderExcept(const matrix4x4& aPV, ObjectID aObject, ClumpID aClump) override;
		void AlbedoRenderExcept(const matrix4x4& aPV, ClumpID aClump) override;
		void AlbedoRender(const matrix4x4& aPV, ClumpID aClump) override;
		void AlbedoRender(const matrix4x4& aPV, ObjectID aObject) override;
		void AlbedoRender(const matrix4x4& aPV) override;
		void Dismiss() override;
	};
	struct D3D11GraphicsScene_F :  IGraphicsScene {
		ID3D11VertexShader* i_AlbedoVertexShader;
		ID3D11PixelShader* i_AlbedoPixelShader;
		ID3D11Buffer* i_AlbedoConstantBuffer;
		ID3D11InputLayout* i_InputLayout;
		D3D11GraphicsDevice* i_GraphicsDevice;
		ID3D11Device* i_Device;
		std::vector<D3D11Object*> m_Objects;
		ID3D11Texture2D* i_WhiteTexture;
		ID3D11ShaderResourceView* i_WhiteTextureSRV;
		ID3D11SamplerState* i_WhiteSampler;
		bool Create(IGraphicsDevice* aiGraphicsDevice) override;
		ObjectID CreateObject(ulargeint aVertexBufferBytes, ulargeint aIndexBufferBytes, uint32 aStride) override;
		ClumpID CreateClump(const CLUMP_METADATA& aMetadata, float32* apVertexStartData, uint16* apIndexStartData) override;
		void UpdateClump(ClumpID aClump, const matrix4x4& aTransform) override;
		void UpdateClumpNumVertices(ClumpID aClump, uint16 aNumVertices) override;
		void UpdateClumpNumIndices(ClumpID aClump, uint32 aNumIndices) override;
		void UpdateClump(ClumpID aClump, float32* apVertexData, ulargeint aCurrentAttemptedAllocationSize) override;
		void UpdateClump(ClumpID aClump, uint16* apIndexData, ulargeint aCurrentAttemptedAllocationSize) override;
		void DismissClump(ClumpID& aClump) override;
		void DismissObject(ObjectID aObject) override;
		IGraphicsDevice* GetParent() override;
		void AlbedoRenderExcept(ObjectID aObject, const matrix4x4& aPV) override;
		void AlbedoRenderExcept(const matrix4x4& aPV, ObjectID aObject) override;
		void AlbedoRenderExcept(const matrix4x4& aPV, ObjectID aObject, ClumpID aClump) override;
		void AlbedoRenderExcept(const matrix4x4& aPV, ClumpID aClump) override;
		void AlbedoRender(const matrix4x4& aPV, ClumpID aClump) override;
		void AlbedoRender(const matrix4x4& aPV, ObjectID aObject) override;
		void AlbedoRender(const matrix4x4& aPV) override;
		void Dismiss() override;
	};
}
#endif // SC_ALLOW_D3D11
#endif // !SC_D3D11_SCENE_H
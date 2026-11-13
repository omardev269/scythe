// BismIllahIRRahmaanIRRaheem
/* core graphics scene virtual class declaration */

#ifndef SC_GRAPHICS_SCENE_H
#define SC_GRAPHICS_SCENE_H
#include <graphics/IGraphicsDevice.h>
#include <graphics/IMaterial.h>

namespace scythe
{
	struct IGraphicsPipeline;
	struct ObjectID {
		ulargeint ID;
	};
	struct ClumpID {
		ulargeint FatherID;
		ulargeint ID;
	};
	struct LightID {
		ulargeint ID;
	};
	struct CLUMP_METADATA {
		ObjectID Parent;
		matrix4x4 Transform;
		ulargeint StartVertexBufferBytes;
		ulargeint StartIndexBufferBytes;
		ulargeint StartIndex;
		ulargeint NumVertexBufferBytes;
		ulargeint NumIndexBufferBytes;
		uint16 NumVertices;
		uint32 NumIndices;
		// Non-owning; NULL selects white. Materials must use the scene's device and outlive their clump references.
		// Do not recreate or dismiss a referenced material while rendering; remove references before dismissal.
		IMaterial* iMaterial;
	};
	struct LIGHT_METADATA {
		float3 Position;
		float3 Color;
		float32 Range;
	};
	struct IGraphicsScene {
		virtual bool Create(IGraphicsDevice* aiGraphicsDevice) = 0;
		virtual LightID CreateLight(const LIGHT_METADATA& aMetadata) = 0;
		virtual ObjectID CreateObject(ulargeint aVertexBufferBytes, ulargeint aIndexBufferBytes, uint32 aStride) = 0;
		virtual ClumpID CreateClump(const CLUMP_METADATA& aMetadata, float32* apVertexStartData, uint16* apIndexStartData) = 0;
		virtual void UpdateClump(ClumpID aClump, const matrix4x4& aTransform) = 0;
		virtual void UpdateClumpNumVertices(ClumpID aClump, uint16 aNumVertices) = 0;
		virtual void UpdateClumpNumIndices(ClumpID aClump, uint32 aNumIndices) = 0;
		virtual void UpdateClump(ClumpID aClump, float32* apVertexData, ulargeint aCurrentAttemptedAllocationSize) = 0;
		virtual void UpdateClump(ClumpID aClump, uint16* apIndexData, ulargeint aCurrentAttemptedAllocationSize) = 0;
		virtual void UpdateClump(ClumpID aClump, IMaterial* aiMaterial) = 0;
		virtual void UpdateLight(LightID aLight, float3 aLightPos) = 0;
		virtual IGraphicsDevice* GetParent() = 0;
		virtual void DismissClump(ClumpID& aClump) = 0;
		virtual void DismissObject(ObjectID aObject) = 0;
		virtual void DismissLight(LightID aLight) = 0;
		virtual void Render(IGraphicsPipeline* aiPipeline, const matrix4x4& aPV) = 0;
		virtual void AlbedoRender(const matrix4x4& aPV) = 0;
		// Make sure that you join all threads using the scene before dismissing this class
		virtual void Dismiss() = 0;
	};
	void CreateGraphicsScene(IGraphicsScene*& oiGraphicsScene, IGraphicsDevice* aiGraphicsDevice);
}
#endif // !SC_GRAPHICS_SCENE_H
// BismIllahIRRahmaanIRRaheem
/* core graphics scene virtual class declaration */

#ifndef SC_GRAPHICS_SCENE_H
#define SC_GRAPHICS_SCENE_H
#include <graphics/IGraphicsDevice.h>

namespace scythe
{
	struct ObjectID {
		ulargeint ID;
	};
	struct ClumpID {
		ulargeint FatherID;
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
	};
	struct IGraphicsScene {
		virtual bool Create(IGraphicsDevice* aiGraphicsDevice) = 0;
		virtual ObjectID CreateObject(ulargeint aVertexBufferBytes, ulargeint aIndexBufferBytes, uint32 aStride) = 0;
		virtual ClumpID CreateClump(const CLUMP_METADATA& aMetadata, float32* apVertexStartData, uint16* apIndexStartData) = 0;
		virtual void UpdateClump(ClumpID aClump, const matrix4x4& aTransform) = 0;
		virtual void UpdateClumpNumVertices(ClumpID aClump, uint16 aNumVertices) = 0;
		virtual void UpdateClumpNumIndices(ClumpID aClump, uint32 aNumIndices) = 0;
		virtual void UpdateClump(ClumpID aClump, float32* apVertexData, ulargeint aCurrentAttemptedAllocationSize) = 0;
		virtual void UpdateClump(ClumpID aClump, uint16* apIndexData, ulargeint aCurrentAttemptedAllocationSize) = 0;
		virtual IGraphicsDevice* GetParent() = 0;
		virtual void DismissClump(ClumpID& aClump) = 0;
		virtual void DismissObject(ObjectID aObject) = 0;
		virtual void AlbedoRender(const matrix4x4& aPV) = 0;
		// Make sure that you join all threads using the scene before dismissing this class
		virtual void Dismiss() = 0;
	};
	void CreateGraphicsScene(IGraphicsScene*& oiGraphicsScene, IGraphicsDevice* aiGraphicsDevice);
}
#endif // !SC_GRAPHICS_SCENE_H
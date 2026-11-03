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
		//IMaterial* iMaterial; // can be NULL, if NULL the object will be white, if not NULL the object will be rendered with the material
	};
	struct IGraphicsScene {
		virtual bool Create(IGraphicsDevice* aiGraphicsDevice) = 0;
		virtual ObjectID CreateObject(ulargeint aVertexBufferBytes, ulargeint aIndexBufferBytes, uint32 aStride) = 0;
		virtual ClumpID CreateClump(CLUMP_METADATA aMetadata, float32* apVertexStartData, uint16* apIndexStartData) = 0;
		virtual void UpdateClump(ClumpID aClump, matrix4x4 aTransform) = 0;
		//virtual void UpdateClump(ClumpID aClump, IMaterial* aiMaterial) = 0;
		virtual void UpdateClumpNumVertices(ClumpID aClump, uint16 aNumVertices) = 0;
		virtual void UpdateClumpNumIndices(ClumpID aClump, uint32 aNumIndices) = 0;
		virtual void UpdateClump(ClumpID aClump, float32* apVertexData, ulargeint aCurrentAttemptedAllocationSize) = 0;
		virtual void UpdateClump(ClumpID aClump, uint16* apIndexData, ulargeint aCurrentAttemptedAllocationSize) = 0;
		virtual void AlbedoRenderExcept(ObjectID aObject, matrix4x4 aPV) = 0;
		virtual IGraphicsDevice* GetParent() = 0;
		virtual void DismissClump(ClumpID& aClump) = 0;
		virtual void DismissObject(ObjectID aObject) = 0;
		virtual void AlbedoRenderExcept(matrix4x4 aPV, ObjectID aObject) = 0;
		virtual void AlbedoRenderExcept(matrix4x4 aPV, ObjectID aObject, ClumpID aClump) = 0;
		virtual void AlbedoRenderExcept(matrix4x4 aPV, ClumpID aClump) = 0;
		virtual void AlbedoRender(matrix4x4 aPV, ClumpID aClump) = 0;
		virtual void AlbedoRender(matrix4x4 aPV, ObjectID aObject) = 0;
		virtual void AlbedoRender(matrix4x4 aPV) = 0;
		// TODO
		//virtual void EnumerateObjects(std::vector<ObjectID>& oObjects) = 0;
		//virtual void EnumerateClumps(ObjectID aObject, std::vector<ClumpID>& oClumps) = 0;
		virtual void Dismiss() = 0;
	};
	void CreateGraphicsScene(IGraphicsScene*& oiGraphicsScene, IGraphicsDevice* aiGraphicsDevice);
}
#endif // !SC_GRAPHICS_SCENE_H
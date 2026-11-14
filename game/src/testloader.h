// BismIllahIRRahmaanIRRaheem
/* experimental model loader for tests */

#ifndef SC_TESTLOADER_H
#define SC_TESTLOADER_H
#include <graphics/IGraphicsScene.h>
#include <vector>

struct StructuredMesh {
	scythe::ObjectID ObjID;
	std::vector<scythe::ClumpID> Clumps;
	std::vector<float32> Vertices; // 32-byte stride: position, UV, normal
	std::vector<uint16> Indices;
	// Owned by the caller; dismiss the object before dismissing these materials.
	std::vector<scythe::IMaterial*> Materials;
};

// Requires assimp-vc145-mt.dll for the process architecture on the DLL search path.
// Windows x64 builds copy it from runtime when available; x86 needs a matching DLL.
// Textures may be embedded or external PNG/JPEG, up to 2048 pixels per dimension.
// On failure ObjID is zero and all resources created by the loader are dismissed.
StructuredMesh LoadMeshIntoSceneAndRetrieveInfo(const char* aFilePath, scythe::IGraphicsScene* aiScene);
#endif // SC_TESTLOADER_H

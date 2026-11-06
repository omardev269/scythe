// BismIllahIRRahmaanIRRaheem
/* gltf loader for tests */

#include <runtime/game.h>
#include <runtime/exposition.h>
#include <runtime/rtdebug.h>
#include <graphics/IGraphicsScene.h>
#include <graphics/freeflycam.h>
#include <gb_math.h>
#include <vector>

struct StructuredMesh {
	scythe::ObjectID ObjID;
	std::vector<scythe::ClumpID> Clumps;
	std::vector<float32> Vertices; // stride must be 20B
	std::vector<uint16> Indices;
};

#if 0
// only for internal tests - not optimized
StructuredMesh LoadMeshIntoSceneAndRetrieveInfo(const char* aFilePath, scythe::IGraphicsScene* aiScene) {
	
}
#endif // 0
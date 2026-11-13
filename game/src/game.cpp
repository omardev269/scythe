// BismIllahIRRahmaanIRRaheem
/* test / sample game */

#include <runtime/game.h>
#include <runtime/exposition.h>
#include <runtime/rtdebug.h>
#include <graphics/IGraphicsScene.h>
#include <graphics/IGraphicsPipeline.h>
#include <graphics/freeflycam.h>
#include <gb_math.h>
#include <vector>

static scythe::FreeflyCamera g_Camera;
static scythe::IGraphicsScene* g_iScene = NULL;
static scythe::IGraphicsPipeline* g_iPipeline = NULL;
static scythe::IMaterial* g_iMaterial = NULL;
static scythe::ObjectID g_ObjectID = { 0 };
static scythe::ClumpID g_ClumpID = { 0, 0 };
static scythe::LightID g_LightID = { 0 };

struct StructuredMesh {
	scythe::ObjectID ObjID;
	std::vector<scythe::ClumpID> Clumps;
	std::vector<float32> Vertices; // stride must be 20B
	std::vector<uint16> Indices;
};

StructuredMesh LoadMeshIntoSceneAndRetrieveInfo(const char* aFilePath, scythe::IGraphicsScene* aiScene);

void start_game() {
	scythe::CreatePhongGraphicsPipeline(g_iPipeline, scythe::g_iGraphicsDevice);
	if (!g_iPipeline) {
		SC_LOG("Failed to create phong graphics pipeline");
		return;
	}
	scythe::MATERIAL_DESC m_MaterialDesc = {};
	m_MaterialDesc.SamplerState = scythe::SAMPLER_STATE_NEAREST;
	m_MaterialDesc.TextureSize = { 2, 2 };
	m_MaterialDesc.PitchBytes = 8;
	uint8  m_TextureData[16] = {
		255, 0, 0, 255,
		0, 255, 50, 255,
		0, 255, 50, 255,
		255, 0, 0, 255
	};
	m_MaterialDesc.pInitialData = m_TextureData;
	scythe::CreateMaterial(g_iMaterial, scythe::g_iGraphicsDevice, m_MaterialDesc);
	scythe::CreateGraphicsScene(g_iScene, scythe::g_iGraphicsDevice);
	SC_SMART_CHECK(g_iScene, "Failed to create graphics scene");
	g_ObjectID = g_iScene->CreateObject(96, 6, 32);
	scythe::CLUMP_METADATA m_Metadata = {};
	m_Metadata.NumIndexBufferBytes = 6;
	m_Metadata.NumVertexBufferBytes = 96;
	m_Metadata.NumIndices = 3;
	m_Metadata.NumVertices = 3;
	gb_mat4_identity((gbMat4*)&m_Metadata.Transform);
	m_Metadata.Parent = g_ObjectID;
	m_Metadata.StartIndex = 0;
	m_Metadata.StartVertexBufferBytes = 0;
	m_Metadata.StartIndexBufferBytes = 0;
	m_Metadata.iMaterial = g_iMaterial;
	float32 m_Vertices[24] = {
		0.0f, 0.5f, 0.0f,	1.0f, 0.0f,		0, 0, 1,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,		0, 0, 1,
		0.5f, -0.5f, 0.0f,	1.0f, 1.0f,		0, 0, 1
	};
	uint16 m_Indices[3] = { 0, 1, 2 };
	g_ClumpID = g_iScene->CreateClump(m_Metadata, m_Vertices, m_Indices);
	g_Camera.Reset();
	g_Camera.SetSpeed(1.0f);
	scythe::LIGHT_METADATA m_LightMetadata = {};
	m_LightMetadata.Color = { 1.0f, 1.0f, 1.0f };
	m_LightMetadata.Range = 3.0f;
	m_LightMetadata.Position = { 0.0f, 0.0f, 1.0f };
	g_LightID = g_iScene->CreateLight(m_LightMetadata);
}


static bool g_MouseToggle = true;
static bool g_EscPressed = false;
static matrix4x4 g_PVMatrix;

bool update_game() {
	// frame state start
	{
		if (scythe::g_pWindow->ShouldClose() || !g_iScene || !g_iPipeline) {
			return false;
		}
		if (!g_EscPressed && (scythe::g_pWindow->GetKey(scythe::Key::Escape) || scythe::g_pWindow->GetKeyUI(scythe::Key::Escape))) {
			SC_TOGGLE(g_MouseToggle);
		}
		scythe::g_pWindow->ConcentrateMouse(g_MouseToggle);
		g_EscPressed = (scythe::g_pWindow->GetKey(scythe::Key::Escape) || scythe::g_pWindow->GetKeyUI(scythe::Key::Escape));
	}
	// camera logic
	{
		float3 m_CamRotation;
		g_Camera.GetRotation(m_CamRotation);
		const float32 m_YawRad = gb_to_radians(m_CamRotation.y);
		const float32 m_Cos = cosf(m_YawRad);
		const float32 m_Sin = sinf(m_YawRad);
		if (scythe::g_pWindow->GetKey(scythe::Key::W)) {
			g_Camera.Move({ m_Sin, 0, -m_Cos });
		}
		if (scythe::g_pWindow->GetKey(scythe::Key::S)) {
			g_Camera.Move({ -m_Sin, 0, m_Cos });
		}
		if (scythe::g_pWindow->GetKey(scythe::Key::A)) {
			g_Camera.Move({ -m_Cos, 0, -m_Sin });
		}
		if (scythe::g_pWindow->GetKey(scythe::Key::D)) {
			g_Camera.Move({ m_Cos, 0, m_Sin });
		}
		if (scythe::g_pWindow->GetKey(scythe::Key::Space)) {
			g_Camera.Move({ 0, 1, 0 });
		}
		if (scythe::g_pWindow->GetKey(scythe::Key::LeftShift)) {
			g_Camera.Move({ 0, -1, 0 });
		}
		// view rotation (skipped on the first frame)
		static bool s_FirstLookFrame = true;
		float2 m_MouseDelta;
		scythe::g_pWindow->GetMouseDelta(m_MouseDelta);
		if (s_FirstLookFrame) {
			s_FirstLookFrame = false;
		}
		else {
			g_Camera.Rotate({ m_MouseDelta.y, m_MouseDelta.x, 0 });
		}
	}
	// render
	{
		uint2 m_WindowSize;
		scythe::g_pWindow->GetSize(m_WindowSize);
		g_Camera.Update(scythe::g_DeltaTime, 60.0f, (float32)m_WindowSize.x / m_WindowSize.y);
		g_Camera.GetMatrix(g_PVMatrix);
		float3 m_CameraPosition;
		g_Camera.GetPosition(m_CameraPosition);
		g_iScene->Render(g_iPipeline, g_PVMatrix, m_CameraPosition);
		g_iPipeline->Update();
	}
	return true;
}

void end_game() {
	if (g_iScene) {
		g_iScene->DismissLight(g_LightID);
		g_iScene->DismissClump(g_ClumpID);
		g_iScene->DismissObject(g_ObjectID);
		g_iScene->Dismiss();
	}
	if (g_iMaterial) g_iMaterial->Dismiss();
	if (g_iPipeline) g_iPipeline->Dismiss();
}
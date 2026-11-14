// BismIllahIRRahmaanIRRaheem
/* test / sample game */

#include <runtime/game.h>
#include <runtime/exposition.h>
#include <runtime/rtdebug.h>
#include <graphics/IGraphicsScene.h>
#include <graphics/IGraphicsPipeline.h>
#include <graphics/freeflycam.h>
#include <gb_math.h>
#include "testloader.h"

static scythe::FreeflyCamera g_Camera;
static scythe::IGraphicsScene* g_iScene = NULL;
static scythe::IGraphicsPipeline* g_iPipeline = NULL;
static scythe::IMaterial* g_iMaterial = NULL;
static StructuredMesh g_Model = {};
static scythe::ObjectID g_LightMarkerObjectID = { 0 };
static scythe::ClumpID g_LightMarkerClumpID = { 0, 0 };
static scythe::LightID g_LightID = { 0 };
static float3 g_LightPosition;
static float3 g_LightMarkerPosition;

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
	if (!g_iScene) {
		SC_LOG("Failed to create graphics scene");
		return;
	}
	g_Model = LoadMeshIntoSceneAndRetrieveInfo("test.glb", g_iScene);
	if (!g_Model.ObjID.ID) SC_LOG("Failed to load test.glb from the working directory");
	float32 m_Vertices[24] = {
		0.0f, 0.5f, 0.0f,	1.0f, 0.0f,		0, 0, 1,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,		0, 0, 1,
		0.5f, -0.5f, 0.0f,	1.0f, 1.0f,		0, 0, 1
	};
	g_LightMarkerObjectID = g_iScene->CreateObject(96, 12, 32);
	scythe::CLUMP_METADATA m_LightMarkerMetadata = {};
	m_LightMarkerMetadata.Parent = g_LightMarkerObjectID;
	m_LightMarkerMetadata.NumVertexBufferBytes = 96;
	m_LightMarkerMetadata.NumVertices = 3;
	m_LightMarkerMetadata.iMaterial = g_iMaterial;
	m_LightMarkerMetadata.NumIndexBufferBytes = 12;
	m_LightMarkerMetadata.NumIndices = 6;
	gbVec3 m_LightMarkerTranslation = { 0.0f, 0.0f, 1.0f };
	gb_mat4_translate((gbMat4*)&m_LightMarkerMetadata.Transform, m_LightMarkerTranslation);
	uint16 m_LightMarkerIndices[6] = { 0, 2, 1, 0, 1, 2 };
	g_LightMarkerClumpID = g_iScene->CreateClump(m_LightMarkerMetadata, m_Vertices, m_LightMarkerIndices);
	g_LightMarkerPosition = { 0.0f, 0.0f, 1.0f };
	g_Camera.Reset();
	g_Camera.SetSpeed(1.0f);
	scythe::LIGHT_METADATA m_LightMetadata = {};
	m_LightMetadata.Color = { 1.0f, 1.0f, 1.0f };
	m_LightMetadata.Range = 100.0f;
	m_LightMetadata.Position = { 0.0f, 0.0f, 1.0f };
	g_LightPosition = m_LightMetadata.Position;
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
		if (!scythe::g_pWindow->GetKey(scythe::Key::LeftControl)) {
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
		else {
			if (scythe::g_pWindow->GetKey(scythe::Key::W)) {
				g_LightPosition.z -= scythe::g_DeltaTime;
			}
			if (scythe::g_pWindow->GetKey(scythe::Key::S)) {
				g_LightPosition.z += scythe::g_DeltaTime;
			}
			if (scythe::g_pWindow->GetKey(scythe::Key::A)) {
				g_LightPosition.x -= scythe::g_DeltaTime;
			}
			if (scythe::g_pWindow->GetKey(scythe::Key::D)) {
				g_LightPosition.x += scythe::g_DeltaTime;
			}
			if (scythe::g_pWindow->GetKey(scythe::Key::Space)) {
				g_LightPosition.y += scythe::g_DeltaTime;
			}
			if (scythe::g_pWindow->GetKey(scythe::Key::LeftShift)) {
				g_LightPosition.y -= scythe::g_DeltaTime;
			}
			if (g_LightPosition.x != g_LightMarkerPosition.x ||
				g_LightPosition.y != g_LightMarkerPosition.y ||
				g_LightPosition.z != g_LightMarkerPosition.z) {
				g_iScene->UpdateLight(g_LightID, g_LightPosition);
				gbVec3 m_LightMarkerTranslation = { g_LightPosition.x, g_LightPosition.y, g_LightPosition.z };
				matrix4x4 m_LightMarkerTransform;
				gb_mat4_translate((gbMat4*)&m_LightMarkerTransform, m_LightMarkerTranslation);
				g_iScene->UpdateClump(g_LightMarkerClumpID, m_LightMarkerTransform);
				g_LightMarkerPosition = g_LightPosition;
			}
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
		g_iScene->DismissClump(g_LightMarkerClumpID);
		g_iScene->DismissObject(g_LightMarkerObjectID);
		if (g_Model.ObjID.ID) g_iScene->DismissObject(g_Model.ObjID);
		g_iScene->Dismiss();
	}
	for (scythe::IMaterial* i_Material : g_Model.Materials) i_Material->Dismiss();
	g_Model = {};
	if (g_iMaterial) g_iMaterial->Dismiss();
	if (g_iPipeline) g_iPipeline->Dismiss();
}
// BismIllahIRRahmaanIRRaheem
/* test / sample game */

#include <runtime/game.h>
#include <runtime/exposition.h>
#include <runtime/rtdebug.h>
#include <graphics/IGraphicsScene.h>
#include <graphics/freeflycam.h>
#include <gb_math.h>
#include <SDL3/SDL.h>

static scythe::FreeflyCamera g_Camera;
static scythe::IGraphicsScene* g_iScene = NULL;
static scythe::ObjectID g_ObjectID = { 0 };
static scythe::ClumpID g_ClumpID = { 0, 0 };

void start_game() {
	g_Camera.Reset();
	scythe::CreateGraphicsScene(g_iScene, scythe::g_iGraphicsDevice);
	SC_SMART_CHECK(g_iScene, "Failed to create graphics scene");
	g_ObjectID = g_iScene->CreateObject(60, 6, 20);
	scythe::CLUMP_METADATA m_Metadata = {};
	m_Metadata.NumIndexBufferBytes = 6;
	m_Metadata.NumVertexBufferBytes = 60;
	m_Metadata.NumIndices = 3;
	m_Metadata.NumVertices = 3;
	gb_mat4_identity((gbMat4*)&m_Metadata.Transform);
	m_Metadata.Parent = g_ObjectID;
	m_Metadata.StartIndex = 0;
	m_Metadata.StartVertexBufferBytes = 0;
	m_Metadata.StartIndexBufferBytes = 0;
	float32 m_Vertices[15] = {
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f
	};
	uint16 m_Indices[3] = { 0, 1, 2 };
	g_ClumpID = g_iScene->CreateClump(m_Metadata, m_Vertices, m_Indices);
}

bool update_game() {
	if (scythe::g_pWindow->ShouldClose() || !g_iScene) {
		return false;
	}

	int m_Width = 0;
	int m_Height = 0;
	SDL_Window* p_Window = static_cast<SDL_Window*>(scythe::g_pWindow->GetBackendHandle());
	if (p_Window && SDL_GetWindowSizeInPixels(p_Window, &m_Width, &m_Height) && m_Width > 0 && m_Height > 0) {
		g_Camera.Move({ 0.0f, 0.0f, -1.0f });
		g_Camera.Update(static_cast<float32>(scythe::g_DeltaTime), 60, static_cast<float32>(m_Width) / m_Height);
		matrix4x4 m_ProjectionView;
		g_Camera.GetMatrix(m_ProjectionView);
		g_iScene->AlbedoRender(m_ProjectionView);
	}
	return true;
}

void end_game() {
	g_iScene->DismissClump(g_ClumpID);
	g_iScene->DismissObject(g_ObjectID);
	g_iScene->Dismiss();
}
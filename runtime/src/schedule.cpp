// BismIllahIRRahmaanIRRaheem
/* engine runtime progression manager */

#include <runtime/engineCore.h>
#include <runtime/window.h>
#include <runtime/rtdebug.h>
#include <graphics/IGraphicsDevice.h>
#include <graphics/IGraphicsScene.h>
#include <timeint.h>
#include <stdio.h>
#ifdef SC_WINDOWS
#if defined(_UNICODE) || defined(UNICODE)
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
) {  
#else
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {
#endif // _UNICODE || UNICODE
#else
int main(
	int argc, 
	char** argv
) {
#endif // SC_WINDOWS
	sc_init_time();
	scythe::Window* p_Window = NULL;
	scythe::CreateWindowClass(p_Window, "BismIllahIRRahmaanIRRaheem", { 800, 600 });
	if (!p_Window) {
		sc_deinit_time();
		return 1;
	}
	scythe::IGraphicsDevice* i_Device = NULL;
	scythe::CreateGraphicsDevice(i_Device, p_Window);
	if (!i_Device) {
		p_Window->Dismiss();
		sc_deinit_time();
		return 1;
	}
	scythe::IGraphicsScene* i_Scene = NULL;
	scythe::CreateGraphicsScene(i_Scene, i_Device);
	if (!i_Scene) {
		i_Device->Dismiss();
		p_Window->Dismiss();
		sc_deinit_time();
		return 1;
	}
	float32 a_Vertices[] = {
		 0.0f,  0.5f, 0.5f, 0.5f, 0.0f,
		 0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f
	};
	uint16 a_Indices[] = { 0, 1, 2 };
	matrix4x4 m_Identity = {{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	}};
	scythe::ObjectID m_Object = i_Scene->CreateObject(sizeof(a_Vertices), sizeof(a_Indices), 5 * sizeof(float32));
	scythe::CLUMP_METADATA m_Metadata = {};
	m_Metadata.Parent = m_Object;
	m_Metadata.Transform = m_Identity;
	m_Metadata.NumVertexBufferBytes = sizeof(a_Vertices);
	m_Metadata.NumIndexBufferBytes = sizeof(a_Indices);
	m_Metadata.NumVertices = 3;
	m_Metadata.NumIndices = 3;
	scythe::ClumpID m_Clump = i_Scene->CreateClump(m_Metadata, a_Vertices, a_Indices);
	while (!p_Window->ShouldClose()) {
		p_Window->PollEvents();
		i_Device->BeginFrame();
		i_Scene->AlbedoRender(m_Identity, m_Clump);
		i_Device->EndFrame();
	}
	i_Scene->DismissClump(m_Clump);
	i_Scene->DismissObject(m_Object);
	SC_IFNOTNULLDISMISS(i_Scene);
	SC_IFNOTNULLDISMISS(p_Window);
	SC_IFNOTNULLDISMISS(i_Device); // many drivers need the window to be invalid before the swapchain is released
	// the time subsystem is responsible for checking its state so we don't check anything to call it
	sc_deinit_time();
	return 0;
}
// BismIllahIRRahmaanIRRaheem
/* experimental model loader for tests */

#include "testloader.h"
#include <runtime/rtdebug.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <gb_math.h>
#include <SDL3/SDL.h>
#include <limits.h>
#include <filesystem>
#include <string>
#ifdef SC_X64
#include <emmintrin.h>
#endif // SC_X64

// Keep SDL's bundled PNG/JPEG decoder private to this translation unit.
#define STB_IMAGE_STATIC
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_NO_STDIO
#define STBI_NO_THREAD_LOCALS
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#ifndef SC_X64
#define STBI_NO_SIMD
#endif
#include "..\..\SDL3-3.4.16\src\video\stb_image.h"

namespace scythe {
	struct TestImportAPI {
		HMODULE Module;
		decltype(&aiImportFile) Import;
		decltype(&aiReleaseImport) Release;
		decltype(&aiGetErrorString) Error;
		decltype(&aiGetMaterialTexture) Texture;
		decltype(&aiGetMaterialColor) Color;
		decltype(&aiGetEmbeddedTexture) Embedded;
	};

	struct TestBrush {
		std::vector<uint8> Pixels;
		uint32 Width;
		uint32 Height;
		bool Opaque;
		IMaterial* Material;
	};

	SC_SPECIAL_STATIC bool OpenTestImportAPI(TestImportAPI& oAPI) {
		oAPI.Module = LoadLibraryA("assimp-vc145-mt.dll");
		if (!oAPI.Module) {
			SC_LOG("Could not load assimp-vc145-mt.dll; deploy a DLL matching the process architecture");
			return false;
		}
		oAPI.Import = reinterpret_cast<decltype(oAPI.Import)>(GetProcAddress(oAPI.Module, "aiImportFile"));
		oAPI.Release = reinterpret_cast<decltype(oAPI.Release)>(GetProcAddress(oAPI.Module, "aiReleaseImport"));
		oAPI.Error = reinterpret_cast<decltype(oAPI.Error)>(GetProcAddress(oAPI.Module, "aiGetErrorString"));
		oAPI.Texture = reinterpret_cast<decltype(oAPI.Texture)>(GetProcAddress(oAPI.Module, "aiGetMaterialTexture"));
		oAPI.Color = reinterpret_cast<decltype(oAPI.Color)>(GetProcAddress(oAPI.Module, "aiGetMaterialColor"));
		oAPI.Embedded = reinterpret_cast<decltype(oAPI.Embedded)>(GetProcAddress(oAPI.Module, "aiGetEmbeddedTexture"));
		if (!oAPI.Import || !oAPI.Release || !oAPI.Error || !oAPI.Texture || !oAPI.Color || !oAPI.Embedded) {
			SC_LOG("Assimp DLL is missing required exports");
			FreeLibrary(oAPI.Module);
			oAPI.Module = NULL;
			return false;
		}
		return true;
	}

	SC_SPECIAL_STATIC bool TestPixelsOpaque(const std::vector<uint8>& aPixels) {
		size_t i = 0;
#ifdef SC_X64
		const __m128i m_ForceRGB = _mm_set1_epi32(0x00ffffff);
		for (; i + 16 <= aPixels.size(); i += 16) {
			const __m128i m_Pixels = _mm_loadu_si128(reinterpret_cast<const __m128i*>(aPixels.data() + i));
			const __m128i m_Equal = _mm_cmpeq_epi8(_mm_or_si128(m_Pixels, m_ForceRGB), _mm_set1_epi32(-1));
			if (_mm_movemask_epi8(m_Equal) != 0xffff) return false;
		}
#endif // SC_X64
		for (; i < aPixels.size(); i += 4) {
			if (aPixels[i + 3] != 255) return false;
		}
		return true;
	}

	SC_SPECIAL_STATIC uint8 TestColorByte(float32 aValue) {
		if (!(aValue > 0.0f)) return 0;
		if (aValue >= 1.0f) return 255;
		return static_cast<uint8>(aValue * 255.0f);
	}

	SC_SPECIAL_STATIC bool LoadTestBrush(const TestImportAPI& aAPI, const aiScene* apScene,
		const aiMaterial* apMaterial, const char* aPath, TestBrush& oBrush) {
		aiString m_Path;
		bool m_HasTexture = aAPI.Texture(apMaterial, aiTextureType_BASE_COLOR, 0, &m_Path, NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS;
		if (!m_HasTexture) m_HasTexture = aAPI.Texture(apMaterial, aiTextureType_DIFFUSE, 0, &m_Path, NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS;
		if (m_HasTexture) {
			const aiTexture* p_Texture = aAPI.Embedded(apScene, m_Path.C_Str());
			int m_Width = 0, m_Height = 0, m_Components = 0;
			uint8* p_Pixels = NULL;
			if (p_Texture && p_Texture->mHeight != 0) {
				if (!p_Texture->pcData || p_Texture->mWidth == 0 || p_Texture->mWidth > 2048 || p_Texture->mHeight > 2048) {
					SC_LOG("Unsupported embedded texture dimensions");
					return false;
				}
				oBrush.Width = p_Texture->mWidth;
				oBrush.Height = p_Texture->mHeight;
				oBrush.Pixels.resize(static_cast<size_t>(oBrush.Width) * oBrush.Height * 4);
				for (size_t i = 0; i < oBrush.Pixels.size() / 4; ++i) {
					oBrush.Pixels[i * 4] = p_Texture->pcData[i].r;
					oBrush.Pixels[i * 4 + 1] = p_Texture->pcData[i].g;
					oBrush.Pixels[i * 4 + 2] = p_Texture->pcData[i].b;
					oBrush.Pixels[i * 4 + 3] = p_Texture->pcData[i].a;
				}
			}
			else {
				if (p_Texture) {
					if (!p_Texture->pcData || p_Texture->mWidth > INT_MAX) {
						SC_LOG("Invalid compressed embedded texture");
						return false;
					}
					p_Pixels = stbi_load_from_memory(reinterpret_cast<const uint8*>(p_Texture->pcData), static_cast<int>(p_Texture->mWidth), &m_Width, &m_Height, &m_Components, 4);
				}
				else if (m_Path.C_Str()[0] != '*') {
					std::filesystem::path m_TexturePath(m_Path.C_Str());
					if (!m_TexturePath.is_absolute()) m_TexturePath = std::filesystem::path(aPath).parent_path() / m_TexturePath;
					std::string m_FullPath = m_TexturePath.string();
					size_t m_Size = 0;
					void* p_Data = SDL_LoadFile(m_FullPath.c_str(), &m_Size);
					if (p_Data && m_Size <= INT_MAX) p_Pixels = stbi_load_from_memory(static_cast<const uint8*>(p_Data), static_cast<int>(m_Size), &m_Width, &m_Height, &m_Components, 4);
					SDL_free(p_Data);
				}
				if (!p_Pixels || m_Width <= 0 || m_Height <= 0 || m_Width > 2048 || m_Height > 2048) {
					if (p_Pixels) stbi_image_free(p_Pixels);
					SC_LOG("Could not load texture or texture exceeds 2048 pixels: %s", m_Path.C_Str());
					return false;
				}
				oBrush.Width = static_cast<uint32>(m_Width);
				oBrush.Height = static_cast<uint32>(m_Height);
				oBrush.Pixels.assign(p_Pixels, p_Pixels + static_cast<size_t>(m_Width) * m_Height * 4);
				stbi_image_free(p_Pixels);
			}
		}
		else {
			aiColor4D m_Color;
			if (aAPI.Color(apMaterial, AI_MATKEY_BASE_COLOR, &m_Color) != AI_SUCCESS &&
				aAPI.Color(apMaterial, AI_MATKEY_COLOR_DIFFUSE, &m_Color) != AI_SUCCESS) {
				m_Color.r = m_Color.g = m_Color.b = 0.5f;
				m_Color.a = 1.0f;
			}
			oBrush.Width = oBrush.Height = 1;
			oBrush.Pixels = { TestColorByte(m_Color.r), TestColorByte(m_Color.g), TestColorByte(m_Color.b), TestColorByte(m_Color.a) };
		}
		oBrush.Opaque = TestPixelsOpaque(oBrush.Pixels);
		return true;
	}

	SC_SPECIAL_STATIC void DismissTestMesh(IGraphicsScene* aiScene, StructuredMesh& aMesh) {
		if (aMesh.ObjID.ID) aiScene->DismissObject(aMesh.ObjID);
		for (IMaterial* i_Material : aMesh.Materials) i_Material->Dismiss();
		aMesh = {};
	}

	SC_SPECIAL_STATIC bool BuildTestMesh(const TestImportAPI& aAPI, const aiScene* apImport,
		const char* aPath, IGraphicsScene* aiScene, StructuredMesh& oMesh) {
		if (!apImport->HasMeshes() || !apImport->HasMaterials() || !aiScene->GetParent()) {
			SC_LOG("Model has no meshes, materials, or scene device");
			return false;
		}
		std::vector<TestBrush> m_Brushes(apImport->mNumMaterials);
		std::vector<bool> m_Used(apImport->mNumMaterials, false);
		size_t m_VertexCount = 0, m_IndexCount = 0;
		for (uint32 c = 0; c < apImport->mNumMeshes; ++c) {
			const aiMesh* p_Mesh = apImport->mMeshes[c];
			if (!p_Mesh || !p_Mesh->HasPositions() || !p_Mesh->HasNormals() || !p_Mesh->mNumVertices ||
				p_Mesh->mNumVertices > 65535 - m_VertexCount || p_Mesh->mMaterialIndex >= apImport->mNumMaterials ||
				!apImport->mMaterials[p_Mesh->mMaterialIndex] || !p_Mesh->mNumFaces || !p_Mesh->mFaces) {
				SC_LOG("Unsupported or invalid mesh in model");
				return false;
			}
			m_VertexCount += p_Mesh->mNumVertices;
			for (uint32 f = 0; f < p_Mesh->mNumFaces; ++f) {
				const aiFace& r_Face = p_Mesh->mFaces[f];
				if (r_Face.mNumIndices != 3 || !r_Face.mIndices || m_IndexCount > UINT_MAX / sizeof(uint16) - 3) {
					SC_LOG("Model contains unsupported faces or too many indices");
					return false;
				}
				for (uint32 i = 0; i < 3; ++i) {
					if (r_Face.mIndices[i] >= p_Mesh->mNumVertices) {
						SC_LOG("Model contains an out-of-range index");
						return false;
					}
				}
				m_IndexCount += 3;
			}
			m_Used[p_Mesh->mMaterialIndex] = true;
		}
		for (uint32 b = 0; b < apImport->mNumMaterials; ++b) {
			if (!m_Used[b]) continue;
			TestBrush& r_Brush = m_Brushes[b];
			if (!LoadTestBrush(aAPI, apImport, apImport->mMaterials[b], aPath, r_Brush)) return false;
			MATERIAL_DESC m_Desc = {};
			m_Desc.SamplerState = SAMPLER_STATE_LINEAR;
			m_Desc.TextureSize = { r_Brush.Width, r_Brush.Height };
			m_Desc.PitchBytes = r_Brush.Width * 4;
			m_Desc.pInitialData = r_Brush.Pixels.data();
			CreateMaterial(r_Brush.Material, aiScene->GetParent(), m_Desc);
			if (!r_Brush.Material) {
				SC_LOG("Failed to create model material");
				return false;
			}
			oMesh.Materials.push_back(r_Brush.Material);
		}
		oMesh.Vertices.reserve(m_VertexCount * 8);
		oMesh.Indices.reserve(m_IndexCount);
		oMesh.Clumps.reserve(apImport->mNumMeshes);
		std::vector<CLUMP_METADATA> m_Metadata;
		m_Metadata.reserve(apImport->mNumMeshes);
		for (int m_Pass = 0; m_Pass < 2; ++m_Pass) {
			for (uint32 c = 0; c < apImport->mNumMeshes; ++c) {
				const aiMesh* p_Mesh = apImport->mMeshes[c];
				const TestBrush& r_Brush = m_Brushes[p_Mesh->mMaterialIndex];
				if (r_Brush.Opaque != (m_Pass == 0)) continue;
				CLUMP_METADATA m_Meta = {};
				gb_mat4_identity(reinterpret_cast<gbMat4*>(&m_Meta.Transform));
				m_Meta.StartVertexBufferBytes = oMesh.Vertices.size() * sizeof(float32);
				m_Meta.StartIndexBufferBytes = oMesh.Indices.size() * sizeof(uint16);
				m_Meta.StartIndex = oMesh.Indices.size();
				m_Meta.NumVertices = static_cast<uint16>(p_Mesh->mNumVertices);
				m_Meta.NumIndices = p_Mesh->mNumFaces * 3;
				m_Meta.NumVertexBufferBytes = static_cast<ulargeint>(m_Meta.NumVertices) * 8 * sizeof(float32);
				m_Meta.NumIndexBufferBytes = static_cast<ulargeint>(m_Meta.NumIndices) * sizeof(uint16);
				m_Meta.iMaterial = r_Brush.Material;
				const uint32 m_Base = static_cast<uint32>(oMesh.Vertices.size() / 8);
				for (uint32 v = 0; v < p_Mesh->mNumVertices; ++v) {
					const aiVector3D& r_Pos = p_Mesh->mVertices[v];
					const aiVector3D& r_Normal = p_Mesh->mNormals[v];
					oMesh.Vertices.insert(oMesh.Vertices.end(), { r_Pos.x, r_Pos.y, r_Pos.z,
						p_Mesh->HasTextureCoords(0) ? p_Mesh->mTextureCoords[0][v].x : 0.0f,
						p_Mesh->HasTextureCoords(0) ? p_Mesh->mTextureCoords[0][v].y : 0.0f,
						r_Normal.x, r_Normal.y, r_Normal.z });
				}
				for (uint32 f = 0; f < p_Mesh->mNumFaces; ++f) {
					for (uint32 i = 0; i < 3; ++i) oMesh.Indices.push_back(static_cast<uint16>(m_Base + p_Mesh->mFaces[f].mIndices[i]));
				}
				m_Metadata.push_back(m_Meta);
			}
		}
		oMesh.ObjID = aiScene->CreateObject(oMesh.Vertices.size() * sizeof(float32), oMesh.Indices.size() * sizeof(uint16), 8 * sizeof(float32));
		if (!oMesh.ObjID.ID) {
			SC_LOG("Failed to create model object");
			return false;
		}
		for (CLUMP_METADATA& r_Meta : m_Metadata) {
			r_Meta.Parent = oMesh.ObjID;
			ClumpID m_Clump = aiScene->CreateClump(r_Meta, oMesh.Vertices.data() + r_Meta.StartVertexBufferBytes / sizeof(float32), oMesh.Indices.data() + r_Meta.StartIndex);
			if (!m_Clump.ID || m_Clump.FatherID != oMesh.ObjID.ID) {
				SC_LOG("Failed to create model clump");
				return false;
			}
			oMesh.Clumps.push_back(m_Clump);
		}
		return true;
	}
}

StructuredMesh LoadMeshIntoSceneAndRetrieveInfo(const char* aFilePath, scythe::IGraphicsScene* aiScene) {
	StructuredMesh m_Result = {};
	if (!aFilePath || !aFilePath[0] || !aiScene) {
		SC_LOG("Invalid model path or scene");
		return m_Result;
	}
	scythe::TestImportAPI m_API = {};
	if (!scythe::OpenTestImportAPI(m_API)) return m_Result;
	const struct aiScene* p_Import = m_API.Import(aFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PreTransformVertices | aiProcess_GenNormals);
	if (!p_Import) {
		SC_LOG("Failed to import model %s: %s", aFilePath, m_API.Error());
		FreeLibrary(m_API.Module);
		return m_Result;
	}
	const bool m_Success = scythe::BuildTestMesh(m_API, p_Import, aFilePath, aiScene, m_Result);
	m_API.Release(p_Import);
	FreeLibrary(m_API.Module);
	if (!m_Success) scythe::DismissTestMesh(aiScene, m_Result);
	return m_Result;
}
// BismIllahIRRahmaanIRRaheem
/* d3d11 scene system impl */

#include <d3d11scene.h>
#ifdef SC_ALLOW_D3D11
#include <d3d11material.h>
#include <runtime/rtdebug.h>
#include <gb_math.h>

#define SC_IFNOTNULLRELEASE(ptr) do { if ((ptr)) { (ptr)->Release(); (ptr) = NULL; } } while (0)

static const uint32 g_VertexShaderBytecodeAlbedo[] = { 1128421444u, 2402879077u, 4141149369u, 3236055827u, 3179830628u, 1u, 1820u, 6u, 56u, 584u,
1316u, 1440u, 1648u, 1732u, 963538753u, 520u, 520u, 4294836736u, 468u, 52u,
2359297u, 3145728u, 3145728u, 2359296u, 3145729u, 0u, 65540u, 0u, 0u, 4294836736u,
83886161u, 2685337605u, 1056964608u, 1065353216u, 0u, 0u, 33554463u, 2147483653u, 2416902144u, 33554463u,
2147549189u, 2416902145u, 33554433u, 2147549184u, 2684354565u, 50331653u, 2147876864u, 2147483648u, 2700607489u, 50331650u,
2147549185u, 2158624768u, 2153054208u, 50331653u, 2147876864u, 2147483648u, 2700607490u, 50331650u, 2147614721u, 2158624768u,
2153054208u, 50331653u, 2147876864u, 2147483648u, 2700607491u, 50331650u, 2147745793u, 2158624768u, 2153054208u, 50331653u,
2147680256u, 2147483648u, 2699952132u, 50331650u, 2148007937u, 2153054208u, 2147483648u, 67108868u, 2148466688u, 2418278400u,
2694119429u, 2691301381u, 50331657u, 3221487616u, 2162425857u, 2162425856u, 33554433u, 2147549185u, 2684354561u, 33554433u,
2147614721u, 2684354562u, 33554433u, 2147745793u, 2684354563u, 33554433u, 2148007937u, 2684354564u, 50331657u, 2147549185u,
2162425857u, 2162425856u, 33554433u, 2147549186u, 2689925121u, 33554433u, 2147614722u, 2689925122u, 33554433u, 2147745794u,
2689925123u, 33554433u, 2148007938u, 2689925124u, 50331657u, 2147614721u, 2162425858u, 2162425856u, 33554433u, 2147549186u,
2701066241u, 33554433u, 2147614722u, 2701066242u, 33554433u, 2147745794u, 2701066243u, 33554433u, 2148007938u, 2701066244u,
50331657u, 2147549184u, 2162425858u, 2162425856u, 67108868u, 3221422080u, 2147483648u, 2699296768u, 2162425857u, 33554433u,
3221749760u, 2147483648u, 33554433u, 3758292992u, 2430861313u, 65535u, 1380206675u, 724u, 65600u, 181u,
67108953u, 2133574u, 0u, 4u, 50331743u, 1052786u, 0u, 50331743u, 1052722u, 1u,
67108967u, 1057010u, 0u, 1u, 50331749u, 1056818u, 1u, 33554536u, 2u, 100663350u,
1048594u, 0u, 2129930u, 0u, 0u, 100663350u, 1048610u, 0u, 2129930u, 0u,
1u, 100663350u, 1048642u, 0u, 2129930u, 0u, 2u, 100663350u, 1048706u, 0u,
2129930u, 0u, 3u, 83886134u, 1048690u, 1u, 1053254u, 0u, 83886134u, 1048706u,
1u, 16385u, 1065353216u, 117440529u, 1056786u, 0u, 1052230u, 0u, 1052230u, 1u,
100663350u, 1048594u, 0u, 2129946u, 0u, 0u, 100663350u, 1048610u, 0u, 2129946u,
0u, 1u, 100663350u, 1048642u, 0u, 2129946u, 0u, 2u, 100663350u, 1048706u,
0u, 2129946u, 0u, 3u, 117440529u, 1056802u, 0u, 1052230u, 0u, 1052230u,
1u, 184549391u, 1048594u, 0u, 16386u, 1056964608u, 1056964608u, 0u, 0u, 2132710u,
0u, 0u, 184549391u, 1048610u, 0u, 16386u, 1056964608u, 1056964608u, 0u, 0u,
2132710u, 0u, 1u, 184549391u, 1048642u, 0u, 16386u, 1056964608u, 1056964608u, 0u,
0u, 2132710u, 0u, 2u, 184549391u, 1048706u, 0u, 16386u, 1056964608u, 1056964608u,
0u, 0u, 2132710u, 0u, 3u, 117440529u, 1056834u, 0u, 1052230u, 0u,
1052230u, 1u, 100663350u, 1048594u, 0u, 2129978u, 0u, 0u, 100663350u, 1048610u,
0u, 2129978u, 0u, 1u, 100663350u, 1048642u, 0u, 2129978u, 0u, 2u,
100663350u, 1048706u, 0u, 2129978u, 0u, 3u, 117440529u, 1056898u, 0u, 1052230u,
0u, 1052230u, 1u, 83886134u, 1056818u, 1u, 1052742u, 1u, 16777278u, 1413567571u,
116u, 24u, 2u, 0u, 4u, 8u, 0u, 0u, 1u, 0u,
0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
15u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
1178944594u, 200u, 1u, 72u, 1u, 28u, 4294837248u, 2304u, 160u, 60u,
0u, 0u, 0u, 0u, 0u, 1u, 1u, 1702129225u, 1801539698u, 2880154368u,
60u, 1u, 96u, 64u, 0u, 0u, 120u, 0u, 64u, 2u,
144u, 0u, 1851880020u, 1919903347u, 1769234797u, 1632464495u, 2020176500u, 2880154368u, 196611u, 262148u,
0u, 0u, 1919117645u, 1718580079u, 1378361460u, 1279795241u, 1394625619u, 1701077352u, 1866670194u, 1818849389u,
824210021u, 3223088u, 1313297225u, 76u, 2u, 8u, 56u, 0u, 0u, 3u,
0u, 1799u, 65u, 0u, 0u, 3u, 1u, 771u, 1230196560u, 1313818964u,
1480938496u, 1380929347u, 2880110660u, 1313297231u, 80u, 2u, 8u, 56u, 0u, 1u,
3u, 0u, 15u, 68u, 0u, 0u, 3u, 1u, 3075u, 1348425299u,
1414091599u, 5132105u, 1129858388u, 1146244943u, 2880154368u, };

static const uint32 g_PixelShaderBytecodeAlbedo[] = { 1128421444u, 1375268566u, 1200379889u, 3503995554u, 238599292u, 1u, 696u, 6u, 56u, 164u,
272u, 396u, 556u, 644u, 963538753u, 100u, 100u, 4294902272u, 60u, 40u,
2621440u, 2621440u, 2621440u, 2359297u, 2621440u, 0u, 4294902272u, 33554463u, 2147483648u, 2952986624u,
33554463u, 2415919104u, 2685339648u, 50331714u, 2148466688u, 2967732224u, 2699298816u, 33554433u, 2148468736u, 2162425856u,
65535u, 1380206675u, 100u, 64u, 25u, 50331738u, 1073152u, 0u, 67115096u, 1077248u,
0u, 21845u, 50335842u, 1052722u, 1u, 50331749u, 1057010u, 0u, 150995013u, 1057010u,
0u, 1052742u, 1u, 1080902u, 0u, 1073152u, 0u, 16777278u, 1413567571u, 116u,
2u, 0u, 0u, 2u, 0u, 0u, 0u, 1u, 0u, 0u,
0u, 0u, 0u, 0u, 1u, 0u, 0u, 0u, 0u, 0u,
0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 1178944594u,
152u, 0u, 0u, 2u, 28u, 4294902784u, 2304u, 112u, 92u, 3u,
0u, 0u, 0u, 0u, 1u, 1u, 102u, 2u, 5u, 4u,
4294967295u, 0u, 1u, 13u, 1632853863u, 1701605485u, 1600585842u, 1954047316u, 6648437u, 1919117645u,
1718580079u, 1378361460u, 1279795241u, 1394625619u, 1701077352u, 1866670194u, 1818849389u, 824210021u, 3223088u, 1313297225u,
80u, 2u, 8u, 56u, 0u, 1u, 3u, 0u, 15u, 68u,
0u, 0u, 3u, 1u, 771u, 1348425299u, 1414091599u, 5132105u, 1129858388u, 1146244943u,
2880154368u, 1313297231u, 44u, 1u, 8u, 32u, 0u, 0u, 3u, 0u,
15u, 1415534163u, 1162302017u, 2880110676u, };

#define SC_RET_SMART_CHECK_DISMISS(expr, error) do { if (!(expr)) { Dismiss(); SC_LOG("%s", error); return false; } } while (0)
#define SC_CREATE_AND_ZERO(type, name) type name; SC_ZMEM(&name, sizeof(type))

// call this only when modifying class state
#define SC_ARM_D11GS_LOCK() std::unique_lock h_Lock(h_GeneralMutex)

bool scythe::D3D11GraphicsScene::Create(IGraphicsDevice* aiGraphicsDevice) {
	{
		SC_ARM_D11GS_LOCK();
		i_GraphicsDevice = static_cast<D3D11GraphicsDevice*>(aiGraphicsDevice);
		i_AlbedoConstantBuffer = NULL;
		i_AlbedoPixelShader = NULL;
		i_AlbedoVertexShader = NULL;
		i_InputLayout = NULL;
		i_WhiteSampler = NULL;
		i_WhiteTexture = NULL;
		i_WhiteTextureSRV = NULL;
	}
	// albedo constant buffer creation
	{
		SC_CREATE_AND_ZERO(D3D11_BUFFER_DESC, m_ConstantBufferDesc);
		m_ConstantBufferDesc.ByteWidth = sizeof(matrix4x4);
		m_ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		m_ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ID3D11Buffer* i_NewBuffer = NULL;
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateBuffer(&m_ConstantBufferDesc, NULL, &i_NewBuffer)), "Failed to create albedo constant buffer");
		SC_ARM_D11GS_LOCK();
		i_AlbedoConstantBuffer = i_NewBuffer;
	}
	// albedo shader creation
	{
		ID3D11VertexShader* i_NewShader = NULL;
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateVertexShader(g_VertexShaderBytecodeAlbedo, sizeof(g_VertexShaderBytecodeAlbedo), NULL, &i_NewShader)), "Failed to create albedo vertex shader");
		SC_ARM_D11GS_LOCK();
		i_AlbedoVertexShader = i_NewShader;
	}
	{
		ID3D11PixelShader* i_NewShader = NULL;
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreatePixelShader(g_PixelShaderBytecodeAlbedo, sizeof(g_PixelShaderBytecodeAlbedo), NULL, &i_NewShader)), "Failed to create albedo pixel shader");
		SC_ARM_D11GS_LOCK();
		i_AlbedoPixelShader = i_NewShader;
	}
	// albedo input layout creation
	{
		D3D11_INPUT_ELEMENT_DESC m_InputElementDesc[2];
		SC_ZMEM(&m_InputElementDesc, sizeof(m_InputElementDesc));
		m_InputElementDesc[0].SemanticName = "POSITION";
		m_InputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		m_InputElementDesc[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		m_InputElementDesc[1].SemanticName = "TEXCOORD";
		m_InputElementDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		m_InputElementDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		ID3D11InputLayout* i_NewLayout = NULL;
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateInputLayout(m_InputElementDesc, 2, g_VertexShaderBytecodeAlbedo, sizeof(g_VertexShaderBytecodeAlbedo), &i_NewLayout)), "Failed to create albedo input layout");
		SC_ARM_D11GS_LOCK();
		i_InputLayout = i_NewLayout;
	}
	// white texture creation
	{
		SC_CREATE_AND_ZERO(D3D11_TEXTURE2D_DESC, m_WhiteTextureDesc);
		m_WhiteTextureDesc.Width = 1;
		m_WhiteTextureDesc.Height = 1;
		m_WhiteTextureDesc.MipLevels = 1;
		m_WhiteTextureDesc.ArraySize = 1;
		m_WhiteTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_WhiteTextureDesc.SampleDesc.Count = 1;
		m_WhiteTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		uint32 m_WhitePixelData = 0xFFFFFFFF; // R255	G255	B255	A255
		D3D11_SUBRESOURCE_DATA m_WhiteSubresourceData;
		SC_ZMEM(&m_WhiteSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_WhiteSubresourceData.pSysMem = &m_WhitePixelData;
		m_WhiteSubresourceData.SysMemPitch = sizeof(uint32);
		ID3D11Texture2D* i_NewTexture = NULL;
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateTexture2D(&m_WhiteTextureDesc, &m_WhiteSubresourceData, &i_NewTexture)), "Failed to create white texture");
		SC_ARM_D11GS_LOCK();
		i_WhiteTexture = i_NewTexture;
	}
	// white texture shader resource view creation
	{
		ID3D11ShaderResourceView* i_NewSRV = NULL;
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateShaderResourceView(i_WhiteTexture, NULL, &i_NewSRV)), "Failed to create white texture SRV");
		SC_ARM_D11GS_LOCK();
		i_WhiteTextureSRV = i_NewSRV;
	}
	// white sampler creation
	{
		SC_CREATE_AND_ZERO(D3D11_SAMPLER_DESC, m_WhiteSamplerDesc);
		m_WhiteSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		m_WhiteSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		m_WhiteSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		m_WhiteSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_WhiteSamplerDesc.MinLOD = 0;
		m_WhiteSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		ID3D11SamplerState* i_NewSampler = NULL;
		SC_RET_SMART_CHECK_DISMISS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateSamplerState(&m_WhiteSamplerDesc, &i_NewSampler)), "Failed to create white sampler");
		SC_ARM_D11GS_LOCK();
		i_WhiteSampler = i_NewSampler;
	}
	return true;
}

#undef SC_RETURN_ON_FAILURE
#define SC_RETURN_ON_FAILURE { 0 }

scythe::ObjectID scythe::D3D11GraphicsScene::CreateObject(ulargeint aVertexBufferBytes, ulargeint aIndexBufferBytes, uint32 aStride) {
	ID3D11Buffer *i_VertexBuffer, *i_IndexBuffer;
	// vbuf
	{
		SC_CREATE_AND_ZERO(D3D11_BUFFER_DESC, m_BufferDesc);
		m_BufferDesc.ByteWidth = aVertexBufferBytes;
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		SC_RET_SMART_CHECK(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateBuffer(&m_BufferDesc, NULL, &i_VertexBuffer)), "Vertex buffer creation failed");
	}
	// ibuf
	{
		SC_CREATE_AND_ZERO(D3D11_BUFFER_DESC, m_BufferDesc);
		m_BufferDesc.ByteWidth = aIndexBufferBytes;
		m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		SC_RET_SMART_CHECK(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateBuffer(&m_BufferDesc, NULL, &i_IndexBuffer)), "Index buffer creation failed");
	}
	D3D11Object* p_D3D11Object = new D3D11Object;
	p_D3D11Object->m_Stride = aStride; // we need to know this when setting the vertex buffer
	p_D3D11Object->m_VertexBufferBytes = aVertexBufferBytes; // -
	p_D3D11Object->m_IndexBufferBytes = aIndexBufferBytes; // for updating with a higher size and stuff
	p_D3D11Object->i_IndexBuffer = i_IndexBuffer;
	p_D3D11Object->i_VertexBuffer = i_VertexBuffer;
	SC_ARM_D11GS_LOCK();
	m_Objects.push_back(p_D3D11Object);
	return { m_Objects.size() }; // 0 should mean an issue
}
#undef SC_RETURN_ON_FAILURE
#define SC_RETURN_ON_FAILURE false
namespace scythe {
	SC_SPECIAL_STATIC bool CheckObjectStuff(std::vector<D3D11Object*>& aObjectVector, ObjectID aObjectID) {
		SC_RET_SMART_CHECK(aObjectID.ID != 0, "ObjectID is 0");
		SC_RET_SMART_CHECK(aObjectVector.size() >= aObjectID.ID, "ObjectID is invalid - higher than the size of the object vector");
		SC_RET_SMART_CHECK(aObjectVector[aObjectID.ID - 1] != NULL, "ObjectID is invalid (NULL case)");
		return true;
	}
	SC_SPECIAL_STATIC bool CheckClumpStuff(std::vector<ClearClumpD3D11>& aClumpVector, ClumpID aClumpID) {
		SC_RET_SMART_CHECK(aClumpID.FatherID != 0, "Clump father ID is 0");
		SC_RET_SMART_CHECK(aClumpID.ID != 0, "Clump ID is 0");
		SC_RET_SMART_CHECK(aClumpVector.size() >= aClumpID.ID, "Clump ID is invalid - higher than the size of the clump vector");
		SC_RET_SMART_CHECK(!aClumpVector[aClumpID.ID - 1].Expired, "Clump ID is invalid (expired case)");
		return true;
	}
}
#define SC_DUMB_PLAIN_RETURN_ON_ERROR(expr) if (!(expr)) { return SC_RETURN_ON_FAILURE; }
#undef SC_RETURN_ON_FAILURE
#define SC_RETURN_ON_FAILURE { 0, 0 }
scythe::ClumpID scythe::D3D11GraphicsScene::CreateClump(const scythe::CLUMP_METADATA& aMetadata, float32* apVertexStartData, uint16* apIndexStartData) {
	D3D11Object* p_Object;
	// checks
	{
		SC_ARM_D11GS_LOCK();
		SC_DUMB_PLAIN_RETURN_ON_ERROR(CheckObjectStuff(m_Objects, aMetadata.Parent));
		p_Object = m_Objects[aMetadata.Parent.ID - 1];
	}
	// update buffers
	{
		// the context is locked because in D3D11 you can't use 2 contexts in parallel
		SpecialContextLock h_SpecialLock = i_GraphicsDevice->OwnContext();
		// update vbuf
		{
			SC_CREATE_AND_ZERO(D3D11_BOX, m_Box);
			m_Box.left = aMetadata.StartVertexBufferBytes;
			m_Box.right = aMetadata.StartVertexBufferBytes + aMetadata.NumVertexBufferBytes;
			if (m_Box.right < p_Object->m_VertexBufferBytes) {
				i_GraphicsDevice->DisownContext(h_SpecialLock);
				SC_RET_SMART_CHECK(m_Box.right >= p_Object->m_VertexBufferBytes, "Vertex buffer overflow");
			}
			m_Box.bottom = 1;
			m_Box.back = 1;
			h_SpecialLock.i_DeviceContext->UpdateSubresource(p_Object->i_VertexBuffer, 0, &m_Box, apVertexStartData, 0, 0);
		}
		// update ibuf
		{
			SC_CREATE_AND_ZERO(D3D11_BOX, m_Box);
			m_Box.left = aMetadata.StartIndexBufferBytes;
			m_Box.right = aMetadata.StartIndexBufferBytes + aMetadata.NumIndexBufferBytes;
			if (m_Box.right < p_Object->m_IndexBufferBytes) {
				i_GraphicsDevice->DisownContext(h_SpecialLock);
				SC_RET_SMART_CHECK(m_Box.right >= p_Object->m_IndexBufferBytes, "Index buffer overflow");
			}
			m_Box.bottom = 1;
			m_Box.back = 1;
			h_SpecialLock.i_DeviceContext->UpdateSubresource(p_Object->i_IndexBuffer, 0, &m_Box, apIndexStartData, 0, 0);
		}
		i_GraphicsDevice->DisownContext(h_SpecialLock);
	}
	ClearClumpD3D11 m_CClump = { aMetadata, false };
	SC_ARM_D11GS_LOCK();
	p_Object->m_Clumps.push_back(m_CClump);
	return { aMetadata.Parent.ID, p_Object->m_Clumps.size()};
}
#undef SC_RETURN_ON_FAILURE
#define SC_RETURN_ON_FAILURE false
void scythe::D3D11GraphicsScene::DismissClump(ClumpID& aClump) {
	ObjectID m_ObjectID = { aClump.FatherID };
	if (!CheckObjectStuff(m_Objects, m_ObjectID)) return;
	D3D11Object* p_Object = m_Objects[aClump.FatherID - 1];
	if (!CheckClumpStuff(p_Object->m_Clumps, aClump)) return;
	ClearClumpD3D11* p_Clump = &p_Object->m_Clumps[aClump.ID - 1];
	{
		SC_ARM_D11GS_LOCK();
		p_Clump->Expired = true;
	}
	aClump.ID = 0;
	aClump.FatherID = 0;
}
void scythe::D3D11GraphicsScene::Dismiss() {
	SC_SMART_CHECK(h_GeneralMutex.try_lock(), "Thread is competing on an expired gscene!");
	SC_IFNOTNULLRELEASE(i_AlbedoConstantBuffer);
	SC_IFNOTNULLRELEASE(i_AlbedoPixelShader);
	SC_IFNOTNULLRELEASE(i_AlbedoVertexShader);
	SC_IFNOTNULLRELEASE(i_InputLayout);
	SC_IFNOTNULLRELEASE(i_WhiteSampler);
	SC_IFNOTNULLRELEASE(i_WhiteTexture);
	SC_IFNOTNULLRELEASE(i_WhiteTextureSRV);
	h_GeneralMutex.unlock();
	delete this;
}

// High confidence: clump lookup and updates. Scene state must already be locked.
namespace scythe {
	SC_SPECIAL_STATIC ClearClumpD3D11* FindLiveClump(std::vector<D3D11Object*>& aObjects, ClumpID aClump) {
		ObjectID m_Parent = { aClump.FatherID };
		if (!CheckObjectStuff(aObjects, m_Parent)) return NULL;
		D3D11Object* p_Object = aObjects[aClump.FatherID - 1];
		if (!CheckClumpStuff(p_Object->m_Clumps, aClump)) return NULL;
		return &p_Object->m_Clumps[aClump.ID - 1];
	}

	SC_SPECIAL_STATIC bool UploadClumpBytes(D3D11GraphicsDevice* aiDevice, ID3D11Buffer* aiBuffer,
		ulargeint aStart, ulargeint aAllocation, ulargeint aBytes, const void* apData) {
		SC_RET_SMART_CHECK(aBytes <= aAllocation, "Clump update exceeds its allocation");
		if (aBytes == 0) return true;
		SC_RET_SMART_CHECK(apData != NULL, "Invalid clump update data");
		D3D11_BUFFER_DESC m_Desc;
		aiBuffer->GetDesc(&m_Desc);
		SC_RET_SMART_CHECK(aStart <= m_Desc.ByteWidth && aAllocation <= m_Desc.ByteWidth - aStart,
			"Clump allocation exceeds its buffer");
		D3D11_BOX m_Box = {};
		m_Box.left = static_cast<UINT>(aStart);
		m_Box.right = static_cast<UINT>(aStart + aBytes);
		m_Box.bottom = 1;
		m_Box.back = 1;
		SpecialContextLock h_Context = aiDevice->OwnContext();
		h_Context.i_DeviceContext->UpdateSubresource(aiBuffer, 0, &m_Box, apData, 0, 0);
		aiDevice->DisownContext(h_Context);
		return true;
	}
}

void scythe::D3D11GraphicsScene::UpdateClump(ClumpID aClump, const matrix4x4& aTransform) {
	ClearClumpD3D11* p_Clump = FindLiveClump(m_Objects, aClump);
	if (!p_Clump) return;
	SC_ARM_D11GS_LOCK();
	p_Clump->Metadata.Transform = aTransform;
}

void scythe::D3D11GraphicsScene::UpdateClumpNumVertices(ClumpID aClump, uint16 aNumVertices) {
	ClearClumpD3D11* p_Clump = FindLiveClump(m_Objects, aClump);
	if (!p_Clump) return;
	SC_ARM_D11GS_LOCK();
	p_Clump->Metadata.NumVertices = aNumVertices;
}

void scythe::D3D11GraphicsScene::UpdateClumpNumIndices(ClumpID aClump, uint32 aNumIndices) {
	ClearClumpD3D11* p_Clump = FindLiveClump(m_Objects, aClump);
	if (!p_Clump) return;
	SC_ARM_D11GS_LOCK();
	p_Clump->Metadata.NumIndices = aNumIndices;
}

void scythe::D3D11GraphicsScene::UpdateClump(ClumpID aClump, float32* apVertexData, ulargeint aCurrentAttemptedAllocationSize) {
	ClearClumpD3D11* p_Clump = FindLiveClump(m_Objects, aClump);
	if (!p_Clump) return;
	UploadClumpBytes(i_GraphicsDevice, m_Objects[aClump.FatherID - 1]->i_VertexBuffer,
		p_Clump->Metadata.StartVertexBufferBytes, p_Clump->Metadata.NumVertexBufferBytes,
		aCurrentAttemptedAllocationSize, apVertexData);
}

void scythe::D3D11GraphicsScene::UpdateClump(ClumpID aClump, uint16* apIndexData, ulargeint aCurrentAttemptedAllocationSize) {
	ClearClumpD3D11* p_Clump = FindLiveClump(m_Objects, aClump);
	if (!p_Clump) return;
	UploadClumpBytes(i_GraphicsDevice, m_Objects[aClump.FatherID - 1]->i_IndexBuffer,
		p_Clump->Metadata.StartIndexBufferBytes, p_Clump->Metadata.NumIndexBufferBytes,
		aCurrentAttemptedAllocationSize, apIndexData);
}

void scythe::D3D11GraphicsScene::UpdateClump(ClumpID aClump, IMaterial* aiMaterial) {
	SC_ARM_D11GS_LOCK();
	ClearClumpD3D11* p_Clump = FindLiveClump(m_Objects, aClump);
	if (!p_Clump) return;
	if (aiMaterial && aiMaterial->GetParent() != i_GraphicsDevice) {
		SC_LOG("Clump material belongs to a different graphics device");
		return;
	}
	p_Clump->Metadata.iMaterial = aiMaterial;
}
// End high confidence: clump lookup and updates.

// High confidence: object disposal and parent access.
void scythe::D3D11GraphicsScene::DismissObject(ObjectID aObject) {
	if (!CheckObjectStuff(m_Objects, aObject)) return;
	D3D11Object* p_Object = m_Objects[aObject.ID - 1];
	{
		SC_ARM_D11GS_LOCK();
		m_Objects[aObject.ID - 1] = NULL;
	}
	SC_IFNOTNULLRELEASE(p_Object->i_IndexBuffer);
	SC_IFNOTNULLRELEASE(p_Object->i_VertexBuffer);
	delete p_Object;
}

scythe::LightID scythe::D3D11GraphicsScene::CreateLight(float3 aLightPos, float32 aLightRange, float32 aEarlyIntensity) {
	return { 0 };
}

void scythe::D3D11GraphicsScene::UpdateLight(LightID aLight, float3 aLightPos) {
	
}

void scythe::D3D11GraphicsScene::DismissLight(LightID aLight){

}

scythe::IGraphicsDevice* scythe::D3D11GraphicsScene::GetParent() {
	return i_GraphicsDevice;
}
// End high confidence: object disposal and parent access.

// Medium confidence: albedo rendering assumes gb_math column-major transforms.
void scythe::D3D11GraphicsScene::AlbedoRender(const matrix4x4& aPV) {
	SC_ARM_D11GS_LOCK();
	SpecialContextLock h_Context = i_GraphicsDevice->OwnContext();
	ID3D11DeviceContext* i_Context = h_Context.i_DeviceContext;
	i_Context->PSSetShader(i_AlbedoPixelShader, NULL, 0);
	i_Context->VSSetShader(i_AlbedoVertexShader, NULL, 0);
	i_Context->VSSetConstantBuffers(0, 1, &i_AlbedoConstantBuffer);
	i_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	i_Context->IASetInputLayout(i_InputLayout);
	for (D3D11Object* p_Object : m_Objects) {
		if (!p_Object) continue;
		D3D11_BUFFER_DESC m_VertexDesc, m_IndexDesc;
		p_Object->i_VertexBuffer->GetDesc(&m_VertexDesc);
		p_Object->i_IndexBuffer->GetDesc(&m_IndexDesc);
		if (p_Object->m_Stride < 5 * sizeof(float32)) {
			SC_LOG("Object vertex stride is too small for the albedo input layout");
			continue;
		}
		UINT m_Stride = p_Object->m_Stride;
		UINT m_Offset = 0;
		i_Context->IASetVertexBuffers(0, 1, &p_Object->i_VertexBuffer, &m_Stride, &m_Offset);
		i_Context->IASetIndexBuffer(p_Object->i_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		for (const ClearClumpD3D11& r_Clump : p_Object->m_Clumps) {
			if (r_Clump.Expired || r_Clump.Metadata.NumIndices == 0) continue;
			const CLUMP_METADATA& r_Metadata = r_Clump.Metadata;
			ID3D11ShaderResourceView* i_SRV = i_WhiteTextureSRV;
			ID3D11SamplerState* i_Sampler = i_WhiteSampler;
			if (r_Metadata.iMaterial) {
				if (r_Metadata.iMaterial->GetParent() != i_GraphicsDevice) {
					SC_LOG("Clump material belongs to a different graphics device");
					continue;
				}
				D3D11Material* p_Material = static_cast<D3D11Material*>(r_Metadata.iMaterial);
				i_SRV = p_Material->GetSRV();
				i_Sampler = p_Material->GetSampler();
			}
			if (r_Metadata.StartVertexBufferBytes > m_VertexDesc.ByteWidth ||
				r_Metadata.NumVertexBufferBytes > m_VertexDesc.ByteWidth - r_Metadata.StartVertexBufferBytes ||
				r_Metadata.StartIndexBufferBytes > m_IndexDesc.ByteWidth ||
				r_Metadata.NumIndexBufferBytes > m_IndexDesc.ByteWidth - r_Metadata.StartIndexBufferBytes ||
				r_Metadata.StartIndexBufferBytes % sizeof(uint16) != 0 ||
				r_Metadata.StartIndex < r_Metadata.StartIndexBufferBytes / sizeof(uint16) ||
				r_Metadata.StartIndex - r_Metadata.StartIndexBufferBytes / sizeof(uint16) > r_Metadata.NumIndexBufferBytes / sizeof(uint16) ||
				r_Metadata.NumIndices > r_Metadata.NumIndexBufferBytes / sizeof(uint16) -
					(r_Metadata.StartIndex - r_Metadata.StartIndexBufferBytes / sizeof(uint16))) {
				SC_LOG("Clump draw exceeds its buffer allocation");
				continue;
			}
			D3D11_MAPPED_SUBRESOURCE m_Mapped = {};
			if (FAILED(i_Context->Map(i_AlbedoConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_Mapped))) {
				SC_LOG("Failed to map albedo constant buffer");
				continue;
			}
			gb_mat4_mul((gbMat4*)m_Mapped.pData, (gbMat4*)&aPV, (gbMat4*)&r_Metadata.Transform);
			i_Context->Unmap(i_AlbedoConstantBuffer, 0);
			i_Context->PSSetShaderResources(0, 1, &i_SRV);
			i_Context->PSSetSamplers(0, 1, &i_Sampler);
			i_Context->DrawIndexed(r_Metadata.NumIndices, static_cast<UINT>(r_Metadata.StartIndex), 0);
		}
	}
	i_GraphicsDevice->DisownContext(h_Context);
}
// End medium confidence: albedo rendering.
#endif // SC_ALLOW_D3D11

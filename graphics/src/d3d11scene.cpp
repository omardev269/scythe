// BismIllahIRRahmaanIRRaheem
/* d3d11 scene system impl */

#include <d3d11scene.h>
#ifdef SC_ALLOW_D3D11
#include <runtime/rtdebug.h>
#include <gb_math.h>

#define SC_IFNOTNULLRELEASE(ptr) { if ((ptr)) { (ptr)->Release(); (ptr) = NULL; } }

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

#define SC_RET_SMART_CHECK_DISMISS(expr, error) { if (!(expr)) { Dismiss(); SC_LOG("%s", error); return false; } }
#define SC_RET_SMART_CHECK_DISMISS_ND11GS(expr, error) { if (!(expr)) { h_Lock.unlock(); Dismiss(); SC_LOG("%s", error); return false; } }
#define SC_CREATE_AND_ZERO(type, name) type name; SC_ZMEM(&name, sizeof(type));

// call this only when modifying class state
#define SC_ARM_D11GS_LOCK() std::unique_lock h_Lock(h_GeneralMutex)

bool scythe::D3D11GraphicsScene::Create(IGraphicsDevice* aiGraphicsDevice) {
	SC_ARM_D11GS_LOCK();
	i_GraphicsDevice = static_cast<D3D11GraphicsDevice*>(aiGraphicsDevice);
	i_AlbedoConstantBuffer = NULL;
	i_AlbedoPixelShader = NULL;
	i_AlbedoVertexShader = NULL;
	i_InputLayout = NULL;
	i_WhiteSampler = NULL;
	i_WhiteTexture = NULL;
	i_WhiteTextureSRV = NULL;
	// albedo constant buffer creation
	{
		SC_CREATE_AND_ZERO(D3D11_BUFFER_DESC, m_ConstantBufferDesc);
		m_ConstantBufferDesc.ByteWidth = sizeof(matrix4x4);
		m_ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		m_ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		SC_RET_SMART_CHECK_DISMISS_ND11GS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateBuffer(&m_ConstantBufferDesc, NULL, &i_AlbedoConstantBuffer)), "Failed to create albedo constant buffer");
	}
	// albedo shader creation
	SC_RET_SMART_CHECK_DISMISS_ND11GS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateVertexShader(g_VertexShaderBytecodeAlbedo, sizeof(g_VertexShaderBytecodeAlbedo), NULL, &i_AlbedoVertexShader)), "Failed to create albedo vertex shader");
	SC_RET_SMART_CHECK_DISMISS_ND11GS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreatePixelShader(g_PixelShaderBytecodeAlbedo, sizeof(g_PixelShaderBytecodeAlbedo), NULL, &i_AlbedoPixelShader)), "Failed to create albedo pixel shader");
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
		SC_RET_SMART_CHECK_DISMISS_ND11GS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateInputLayout(m_InputElementDesc, 2, g_VertexShaderBytecodeAlbedo, sizeof(g_VertexShaderBytecodeAlbedo), &i_InputLayout)), "Failed to create albedo input layout");
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
		SC_RET_SMART_CHECK_DISMISS_ND11GS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateTexture2D(&m_WhiteTextureDesc, &m_WhiteSubresourceData, &i_WhiteTexture)), "Failed to create white texture");
	}
	// white texture shader resource view creation
	SC_RET_SMART_CHECK_DISMISS_ND11GS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateShaderResourceView(i_WhiteTexture, NULL, &i_WhiteTextureSRV)), "Failed to create white texture SRV");
	// white sampler creation
	{
		SC_CREATE_AND_ZERO(D3D11_SAMPLER_DESC, m_WhiteSamplerDesc);
		m_WhiteSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		m_WhiteSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		m_WhiteSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		m_WhiteSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_WhiteSamplerDesc.MinLOD = 0;
		m_WhiteSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		SC_RET_SMART_CHECK_DISMISS_ND11GS(SUCCEEDED(i_GraphicsDevice->GetDevice()->CreateSamplerState(&m_WhiteSamplerDesc, &i_WhiteSampler)), "Failed to create white sampler");
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
#define SC_RETURN_ON_FAILURE { 0, 0 }
scythe::ClumpID scythe::D3D11GraphicsScene::CreateClump(const scythe::CLUMP_METADATA& aMetadata, float32* apVertexStartData, uint16* apIndexStartData) {
	SC_RET_SMART_CHECK(aMetadata.Parent.ID != 0, "Clump parent ID is 0");
	D3D11Object* p_Object;
	{
		SC_ARM_D11GS_LOCK();
		SC_RET_SMART_CHECK(m_Objects.size() >= aMetadata.Parent.ID, "Clump parent ID is invalid - higher than the size of the object vector");
		p_Object = m_Objects[aMetadata.Parent.ID - 1];
		SC_RET_SMART_CHECK(p_Object != NULL, "Clump parent ID is invalid (NULL case)")
	}

}
#undef SC_RETURN_ON_FAILURE
#define SC_RETURN_ON_FAILURE false
void scythe::D3D11GraphicsScene::Dismiss() {
	SC_SMART_CHECK(h_GeneralMutex.try_lock(), "Thread is competing on an expired gscene!");
	h_GeneralMutex.unlock();
	SC_IFNOTNULLRELEASE(i_AlbedoConstantBuffer);
	SC_IFNOTNULLRELEASE(i_AlbedoPixelShader);
	SC_IFNOTNULLRELEASE(i_AlbedoVertexShader);
	SC_IFNOTNULLRELEASE(i_InputLayout);
	SC_IFNOTNULLRELEASE(i_WhiteSampler);
	SC_IFNOTNULLRELEASE(i_WhiteTexture);
	SC_IFNOTNULLRELEASE(i_WhiteTextureSRV);
	delete this;
}
#endif // SC_ALLOW_D3D11

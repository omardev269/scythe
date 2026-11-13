// BismIllahIRRahmaanIRRaheem
/* graphics pipeline impl */

#include <d3d11phonggraphicspipeline.h>

#ifdef SC_ALLOW_D3D11
#include <runtime/rtdebug.h>
static uint32 g_VertexShaderBytecodePhong[] = { 1128421444u, 417819479u, 3919603940u, 2361773739u, 2697243283u, 1u, 1896u, 6u, 56u, 596u,
1360u, 1484u, 1692u, 1776u, 963538753u, 532u, 532u, 4294836736u, 480u, 52u,
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
3221749760u, 2147483648u, 33554433u, 3758292992u, 2430861313u, 33554433u, 3758555137u, 2430861312u, 65535u, 1380206675u,
756u, 65600u, 189u, 67108953u, 2133574u, 0u, 4u, 50331743u, 1052786u, 0u,
50331743u, 1052722u, 1u, 67108967u, 1057010u, 0u, 1u, 50331749u, 1056818u, 1u,
50331749u, 1056882u, 2u, 33554536u, 2u, 100663350u, 1048594u, 0u, 2129930u, 0u,
0u, 100663350u, 1048610u, 0u, 2129930u, 0u, 1u, 100663350u, 1048642u, 0u,
2129930u, 0u, 2u, 100663350u, 1048706u, 0u, 2129930u, 0u, 3u, 83886134u,
1048690u, 1u, 1053254u, 0u, 83886134u, 1048706u, 1u, 16385u, 1065353216u, 117440529u,
1056786u, 0u, 1052230u, 0u, 1052230u, 1u, 100663350u, 1048594u, 0u, 2129946u,
0u, 0u, 100663350u, 1048610u, 0u, 2129946u, 0u, 1u, 100663350u, 1048642u,
0u, 2129946u, 0u, 2u, 100663350u, 1048706u, 0u, 2129946u, 0u, 3u,
117440529u, 1056802u, 0u, 1052230u, 0u, 1052230u, 1u, 184549391u, 1048594u, 0u,
16386u, 1056964608u, 1056964608u, 0u, 0u, 2132710u, 0u, 0u, 184549391u, 1048610u,
0u, 16386u, 1056964608u, 1056964608u, 0u, 0u, 2132710u, 0u, 1u, 184549391u,
1048642u, 0u, 16386u, 1056964608u, 1056964608u, 0u, 0u, 2132710u, 0u, 2u,
184549391u, 1048706u, 0u, 16386u, 1056964608u, 1056964608u, 0u, 0u, 2132710u, 0u,
3u, 117440529u, 1056834u, 0u, 1052230u, 0u, 1052230u, 1u, 100663350u, 1048594u,
0u, 2129978u, 0u, 0u, 100663350u, 1048610u, 0u, 2129978u, 0u, 1u,
100663350u, 1048642u, 0u, 2129978u, 0u, 2u, 100663350u, 1048706u, 0u, 2129978u,
0u, 3u, 117440529u, 1056898u, 0u, 1052230u, 0u, 1052230u, 1u, 83886134u,
1056818u, 1u, 1052742u, 1u, 83886134u, 1056882u, 2u, 1053254u, 0u, 16777278u,
1413567571u, 116u, 25u, 2u, 0u, 5u, 8u, 0u, 0u, 1u,
0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
0u, 16u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
0u, 1178944594u, 200u, 1u, 72u, 1u, 28u, 4294837248u, 2304u, 160u,
60u, 0u, 0u, 0u, 0u, 0u, 1u, 1u, 1702129225u, 1801539698u,
2880154368u, 60u, 1u, 96u, 64u, 0u, 0u, 120u, 0u, 64u,
2u, 144u, 0u, 1851880020u, 1919903347u, 1769234797u, 1632464495u, 2020176500u, 2880154368u, 196611u,
262148u, 0u, 0u, 1919117645u, 1718580079u, 1378361460u, 1279795241u, 1394625619u, 1701077352u, 1866670194u,
1818849389u, 824210021u, 3223088u, 1313297225u, 76u, 2u, 8u, 56u, 0u, 0u,
3u, 0u, 1799u, 65u, 0u, 0u, 3u, 1u, 771u, 1230196560u,
1313818964u, 1480938496u, 1380929347u, 2880110660u, 1313297231u, 112u, 3u, 8u, 80u, 0u,
1u, 3u, 0u, 15u, 92u, 0u, 0u, 3u, 1u, 3075u,
101u, 0u, 0u, 3u, 2u, 2055u, 1348425299u, 1414091599u, 5132105u, 1129858388u,
1146244943u, 1397706752u, 1330205769u, 2880110670u, };

static uint32 g_PixelShaderBytecodePhong[] = { 1128421444u, 50953844u, 3549288092u, 1631190310u, 349447435u, 1u, 728u, 6u, 56u, 164u,
272u, 396u, 556u, 676u, 963538753u, 100u, 100u, 4294902272u, 60u, 40u,
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
112u, 3u, 8u, 80u, 0u, 1u, 3u, 0u, 15u, 92u,
0u, 0u, 3u, 1u, 771u, 101u, 0u, 0u, 3u, 2u,
7u, 1348425299u, 1414091599u, 5132105u, 1129858388u, 1146244943u, 1397706752u, 1330205769u, 2880110670u, 1313297231u,
44u, 1u, 8u, 32u, 0u, 0u, 3u, 0u, 15u, 1415534163u,
1162302017u, 2880110676u, };

namespace scythe {
	bool D3D11PhongPipeline::Create(IGraphicsDevice* aiRegistoire)
	{
		SC_RET_SMART_CHECK(aiRegistoire != NULL, "Invalid graphics device fed into the phong pipeline implementation");
		{
			i_Sampler = NULL;
			i_Texture = NULL;
			i_VertexShader = NULL;
			i_PixelShader = NULL;
			i_InputLayout = NULL;
			i_TransformBuffer = NULL;
		}
		i_Device = (D3D11GraphicsDevice*)aiRegistoire;
		ID3D11Device* i_D3DDevice = i_Device->GetDevice();
		SC_RET_SMART_CHECK(SUCCEEDED(i_D3DDevice->CreateVertexShader(g_VertexShaderBytecodePhong, sizeof(g_VertexShaderBytecodePhong), NULL, &i_VertexShader)), "Phong vertex shader creation failed");
		SC_RET_SMART_CHECK(SUCCEEDED(i_D3DDevice->CreatePixelShader(g_PixelShaderBytecodePhong, sizeof(g_PixelShaderBytecodePhong), NULL, &i_PixelShader)), "Phong pixel shader creation failed");
		{
			D3D11_INPUT_ELEMENT_DESC m_InputLayoutDesc[2];
			SC_ZMEM(&m_InputLayoutDesc, sizeof(m_InputLayoutDesc));
			m_InputLayoutDesc[0].SemanticName = "POSITION";
			m_InputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			m_InputLayoutDesc[1].SemanticName = "TEXCOORD";
			m_InputLayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
			m_InputLayoutDesc[1].AlignedByteOffset = 12;
			SC_RET_SMART_CHECK(SUCCEEDED(i_D3DDevice->CreateInputLayout(m_InputLayoutDesc, 2, g_VertexShaderBytecodePhong, sizeof(g_VertexShaderBytecodePhong), &i_InputLayout)), "Phong input layout creation failed");
		}
		{
			D3D11_BUFFER_DESC m_ConstantBufferDesc;
			SC_ZMEM(&m_ConstantBufferDesc, sizeof(m_ConstantBufferDesc));
			m_ConstantBufferDesc.ByteWidth = (UINT)sizeof(matrix4x4);
			m_ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			SC_RET_SMART_CHECK(SUCCEEDED(i_D3DDevice->CreateBuffer(&m_ConstantBufferDesc, NULL, &i_TransformBuffer)), "Phong constant buffer creation failed");
		}
		{
			D3D11_TEXTURE2D_DESC m_TextureDesc;
			SC_ZMEM(&m_TextureDesc, sizeof(m_TextureDesc));
			m_TextureDesc.Width = static_cast<UINT>(i_Device->GetViewport().Width);
			m_TextureDesc.Height = static_cast<UINT>(i_Device->GetViewport().Height);
			m_TextureDesc.MipLevels = 1;
			m_TextureDesc.ArraySize = 1;
			m_TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			m_TextureDesc.SampleDesc.Count = 1;
			m_TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			SC_RET_SMART_CHECK(SUCCEEDED(i_D3DDevice->CreateTexture2D(&m_TextureDesc, NULL, &i_RenderTexture)), "Phong GBuffer creation failed");
		}
		{
			SC_RET_SMART_CHECK(SUCCEEDED(i_D3DDevice->CreateShaderResourceView(i_RenderTexture, NULL, &i_RenderTextureView)), "Phong GBuffer SRV creation failed");
		}
		{
			SC_RET_SMART_CHECK(SUCCEEDED(i_D3DDevice->CreateRenderTargetView(i_RenderTexture, NULL, &i_RenderTargetView)), "Phong GBuffer RTV creation failed");
		}
		return true;
	}
	bool D3D11PhongPipeline::EnsureRenderTarget(ID3D11DeviceContext* aiContext, UINT aWidth, UINT aHeight, bool& aRecreated)
	{
		aRecreated = false;
		if (!aWidth || !aHeight) return false;
		D3D11_TEXTURE2D_DESC m_Desc;
		i_RenderTexture->GetDesc(&m_Desc);
		if (m_Desc.Width == aWidth && m_Desc.Height == aHeight) return true;

		m_Desc.Width = aWidth;
		m_Desc.Height = aHeight;
		ID3D11Texture2D* i_NewTexture = NULL;
		ID3D11ShaderResourceView* i_NewTextureView = NULL;
		ID3D11RenderTargetView* i_NewTargetView = NULL;
		ID3D11Device* i_D3DDevice = i_Device->GetDevice();
		HRESULT m_Result = i_D3DDevice->CreateTexture2D(&m_Desc, NULL, &i_NewTexture);
		if (SUCCEEDED(m_Result)) m_Result = i_D3DDevice->CreateShaderResourceView(i_NewTexture, NULL, &i_NewTextureView);
		if (SUCCEEDED(m_Result)) m_Result = i_D3DDevice->CreateRenderTargetView(i_NewTexture, NULL, &i_NewTargetView);
		if (FAILED(m_Result)) {
			if (i_NewTargetView) i_NewTargetView->Release();
			if (i_NewTextureView) i_NewTextureView->Release();
			if (i_NewTexture) i_NewTexture->Release();
			SC_LOG("Failed to resize albedo render target");
			return false;
		}
		const FLOAT m_ClearColor[4] = { 0, 0, 0, 0 };
		aiContext->ClearRenderTargetView(i_NewTargetView, m_ClearColor);
		i_RenderTargetView->Release();
		i_RenderTextureView->Release();
		i_RenderTexture->Release();
		i_RenderTexture = i_NewTexture;
		i_RenderTextureView = i_NewTextureView;
		i_RenderTargetView = i_NewTargetView;
		aRecreated = true;
		return true;
	}
	void D3D11PhongPipeline::ReactionForTransformChange()
	{
		SpecialContextLock h_Lock = i_Device->OwnContext();
		h_Lock.i_DeviceContext->UpdateSubresource(i_TransformBuffer, 0, NULL, &m_Transform, 0, 0);
		i_Device->DisownContext(h_Lock);
	}
	void D3D11PhongPipeline::SetLights(const std::vector<LIGHT_METADATA>& aMetadata)
	{
		m_Lights = aMetadata;
	}
	void D3D11PhongPipeline::Render(std::function<void()> ahDraw, bool aDepthEnabled)
	{
		i_Device->SetDepth(aDepthEnabled);
		{
			SpecialContextLock h_Lock = i_Device->OwnContext();
			ID3D11DeviceContext* i_D3DContext = h_Lock.i_DeviceContext;
			const D3D11_VIEWPORT& r_Viewport = i_Device->GetViewport();
			bool m_Recreated;
			if (!EnsureRenderTarget(i_D3DContext, static_cast<UINT>(r_Viewport.Width), static_cast<UINT>(r_Viewport.Height), m_Recreated)) {
				i_Device->DisownContext(h_Lock);
				return;
			}
			if (!m_HasRendered || m_Recreated) {
				ID3D11Resource* i_MasterRenderTexture = NULL;
				i_Device->GetMasterRenderTargetView()->GetResource(&i_MasterRenderTexture);
				i_D3DContext->CopyResource(i_RenderTexture, i_MasterRenderTexture);
				i_MasterRenderTexture->Release();
			}
			m_HasRendered = true;
			i_D3DContext->OMSetRenderTargets(1, &i_RenderTargetView, i_Device->GetDepthStencilView());
			i_D3DContext->IASetInputLayout(i_InputLayout);
			i_D3DContext->VSSetShader(i_VertexShader, NULL, 0);
			i_D3DContext->PSSetShader(i_PixelShader, NULL, 0);
			i_D3DContext->VSSetConstantBuffers(0, 1, &i_TransformBuffer);
			i_D3DContext->PSSetShaderResources(0, 1, &i_Texture);
			i_D3DContext->PSSetSamplers(0, 1, &i_Sampler);
			i_Device->DisownContext(h_Lock);
		}
		ahDraw();
	}
	void D3D11PhongPipeline::Update()
	{
		i_Device->Reset();
		SpecialContextLock h_Lock = i_Device->OwnContext();
		if (!m_HasRendered) {
			i_Device->DisownContext(h_Lock);
			return;
		}
		m_HasRendered = false;
		UINT m_ViewportCount = 1;
		D3D11_VIEWPORT m_Viewport;
		h_Lock.i_DeviceContext->RSGetViewports(&m_ViewportCount, &m_Viewport);
		bool m_Recreated;
		if (!m_ViewportCount || !EnsureRenderTarget(h_Lock.i_DeviceContext, static_cast<UINT>(m_Viewport.Width), static_cast<UINT>(m_Viewport.Height), m_Recreated) || m_Recreated) {
			i_Device->DisownContext(h_Lock);
			return;
		}
		ID3D11Resource* i_MasterRenderTexture = NULL;
		i_Device->GetMasterRenderTargetView()->GetResource(&i_MasterRenderTexture);
		h_Lock.i_DeviceContext->CopyResource(i_MasterRenderTexture, i_RenderTexture);
		i_MasterRenderTexture->Release();
		i_Device->DisownContext(h_Lock);
	}
	void D3D11PhongPipeline::Dismiss()
	{
		if (i_RenderTargetView) i_RenderTargetView->Release();
		if (i_RenderTextureView) i_RenderTextureView->Release();
		if (i_RenderTexture) i_RenderTexture->Release();
		if (i_TransformBuffer) i_TransformBuffer->Release();
		if (i_InputLayout) i_InputLayout->Release();
		if (i_PixelShader) i_PixelShader->Release();
		if (i_VertexShader) i_VertexShader->Release();
		delete this;
	}
}
#endif // SC_ALLOW_D3D11

#include "main.h"
#include "renderer.h"
#include "splash.h"
#include "selectCursor.h"
#include <random>

#define ParticleAmount 100000

ID3D11ShaderResourceView* Splash::m_ParticleSRV = {};
ID3D11ShaderResourceView* Splash::m_PositionSRV = {};
ID3D11ShaderResourceView* Splash::m_ColorSRV = {};

ID3D11UnorderedAccessView* Splash::m_ResultUAV = {};

Splash::Splash()
{
	m_Component.clear();
	Splash::Init();
}

Splash::Splash(Splash& copy)
{
	m_Component.clear();
	Splash::Init();
}

Splash::~Splash()
{
	Splash::Uninit();
}

void Splash::Init()
{
	VERTEX_3D_PARTICLE vertex[4];
	float size = 2.0f;
	D3DXVECTOR3 center = { 0.0f, 10.0f, 0.0f };

	vertex[0].Position = D3DXVECTOR3(center.x - size, center.y - size, center.z - size);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	vertex[1].Position = D3DXVECTOR3(center.x - size, center.y + size, center.z - size);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
	vertex[2].Position = D3DXVECTOR3(center.x + size, center.y - size, center.z - size);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
	vertex[3].Position = D3DXVECTOR3(center.x + size, center.y + size, center.z - size);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//パーティクル情報生成
	m_Particle = new ParticleCompute[ParticleAmount];

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<>	 distW(-150, 150);
	std::uniform_int_distribution<>	 distH(-150, 150);
	std::uniform_int_distribution<>	 distZ(-25, 0);
	std::uniform_int_distribution<>	 distS1(1, 5);
	std::uniform_int_distribution<>	 distS2(-10, 10);
	std::uniform_int_distribution<>	 distCR(10, 100);
	std::uniform_int_distribution<>	 distCG(10, 100);
	std::uniform_int_distribution<>	 distCB(10, 100);

	for (int i = 0; i < ParticleAmount; i++){
		float rw = distW(mt);
		float rh = distH(mt);
		float rz = distZ(mt);
		float rsX = distS2(mt);
		float rsY = distS1(mt);
		float rsZ = distS2(mt);
		float rcR = distCR(mt) * 0.01f;
		float rcG = distCG(mt) * 0.01f;
		float rcB = distCB(mt) * 0.01f;

		m_Particle[i].pos = D3DXVECTOR3(rw, rh, rz);
		m_Particle[i].life = 60.0f * distS1(mt);
		m_Particle[i].vel = D3DXVECTOR3(rsX, rsY, rsZ);
		m_Particle[i].color = D3DXCOLOR(rcR, rcG, rcB, 1.0f);
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D_PARTICLE) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	Renderer::CreateStructuredBuffer(sizeof(ParticleCompute), (UINT)ParticleAmount, &m_ParticleBuffer);
	Renderer::CreateStructuredBuffer(sizeof(D3DXVECTOR3), (UINT)ParticleAmount, &m_PositionBuffer);
	Renderer::CreateStructuredBuffer(sizeof(D3DXCOLOR), (UINT)ParticleAmount, &m_ColorBuffer);

	Renderer::CreateShaderResourceView(m_ParticleBuffer, sizeof(ParticleCompute), (UINT)ParticleAmount, &m_ParticleSRV);
	Renderer::CreateShaderResourceView(m_PositionBuffer, sizeof(D3DXVECTOR3), (UINT)ParticleAmount, &m_PositionSRV);
	Renderer::CreateShaderResourceView(m_ColorBuffer, sizeof(D3DXCOLOR), (UINT)ParticleAmount, &m_ColorSRV);


	Renderer::CreateUnorderedAccessBuffer(sizeof(ParticleCompute), (UINT)ParticleAmount, &m_ResultBuffer);
	Renderer::CreateUnorderedAccessView(m_ResultBuffer, (UINT)ParticleAmount, &m_ResultUAV);

	D3D11_MAPPED_SUBRESOURCE subres;
	ZeroMemory(&subres, sizeof(subres));
	auto hr = Renderer::GetDeviceContext()->Map(m_ColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subres);

	auto pColBuf = (D3DXCOLOR*)(subres.pData);
	for (int v = 0; v < ParticleAmount; v++){
		pColBuf[v] = m_Particle[v].color;
	}

	Renderer::GetDeviceContext()->Unmap(m_ColorBuffer, 0);

	Renderer::CreateComputeShader(&m_ComputeShader, "shader\\particleCS.cso");
	Renderer::CreateVertexShader(&m_VertexShader, &m_InputLayout, "shader\\particleVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\particlePS.cso");

}

void Splash::Uninit()
{
	m_PixelShader->Release();
	m_InputLayout->Release();
	m_VertexShader->Release();

	m_ResultUAV->Release();
	m_ResultBuffer->Release();

	m_PositionSRV->Release();
	m_ParticleSRV->Release();

	m_PositionBuffer->Release();
	m_ParticleBuffer->Release();

	m_VertexBuffer->Release();
}

void Splash::Update()
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE subres{};
	D3DXVECTOR3* pPosBuf;
	ID3D11Buffer* pBufDbg{};

	//現時点のパーティクル情報(m_Particle)をバッファ(m_ParticleBuffer)に渡す
	pBufDbg = {};
	hr = Renderer::GetDeviceContext()->Map(m_ParticleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subres);
	ParticleCompute* pBufType = (ParticleCompute*)subres.pData;
	memcpy(subres.pData, m_Particle, sizeof(ParticleCompute) * ParticleAmount);
	Renderer::GetDeviceContext()->Unmap(m_ParticleBuffer, 0);

	//ComputeShaderにパーティクル情報のバッファ(m_ParticleBuffer)のSRVを設定して実行する
	ID3D11ShaderResourceView* pSRVs[1] = { m_ParticleSRV };
	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, pSRVs);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ResultUAV, 0);
	Renderer::GetDeviceContext()->Dispatch(100000, 1, 1);

	ID3D11UnorderedAccessView* nullUAV{};
	ID3D11ShaderResourceView* nullSRV{};

	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &nullUAV, 0);
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &nullSRV);
	Renderer::GetDeviceContext()->CSSetShader(nullptr, nullptr, 0);

	//CSの計算結果(m_ResultBuffer)を別のBufferに移してそれを参照しながらパーティクル情報を更新する
	ZeroMemory(&subres, sizeof(subres));
	pBufDbg = {};
	Renderer::CreateAndCopyBuffer((UINT)sizeof(ParticleCompute), ParticleAmount, m_ResultBuffer, &pBufDbg);
	Renderer::GetDeviceContext()->Map(pBufDbg, 0, D3D11_MAP_READ, 0, &subres);
	pBufType = {};
	pBufType = (ParticleCompute*)subres.pData;

	memcpy((m_Particle), (subres.pData), sizeof(ParticleCompute) * ParticleAmount);
	Renderer::GetDeviceContext()->Unmap(pBufDbg, 0);

	//更新済みのパーティクル情報を基にして描画用の頂点情報(m_PositionBuffer)に設定する
	ZeroMemory(&subres, sizeof(subres));
	hr = Renderer::GetDeviceContext()->Map(m_PositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subres);

	pPosBuf = (D3DXVECTOR3*)(subres.pData);
	for (int v = 0; v < ParticleAmount; v++){
		pPosBuf[v] = m_Particle[v].pos;
	}

	Renderer::GetDeviceContext()->Unmap(m_PositionBuffer, 0);

	pBufDbg->Release();

}

void Splash::Draw()
{	
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	Renderer::GetDeviceContext()->IASetInputLayout(m_InputLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	UINT stride = sizeof(VERTEX_3D_PARTICLE);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_PositionSRV);
	Renderer::GetDeviceContext()->VSSetShaderResources(3, 1, &m_ColorSRV);

	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::GetDeviceContext()->DrawInstanced(4, ParticleAmount, 0, 0);

}

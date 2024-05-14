#include "main.h"
#include "waveface.h"
#include "selectCursor.h"

//ポリゴン生成
int polygonNumX = WAVE_COUNT;
int polygonNumZ = WAVE_COUNT;
int height = 0;
int sizeX = 1;
int sizeZ = 1;

Waveface::Waveface()
{
	m_Component.clear();
	Waveface::Init();
}

Waveface::Waveface(Waveface& copy)
{
	m_Component.clear();
	Waveface::Init();
}

Waveface::~Waveface()
{
	Waveface::Uninit();
}

void Waveface::Init()
{
	m_ObjectName = "WaveFace";

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	m_numVertex = (polygonNumX + 1) * (polygonNumX + 1);
	m_numIndex = (polygonNumX + 1) * 2 * polygonNumZ + (polygonNumZ - 1) * 2;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * m_numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	auto hr = Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	//インデックスバッファ用意
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * m_numIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_IndexBuffer);

	//頂点バッファの中身を埋める
	{

		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		// 頂点バッファ生成
		{
			for (int x = 0; x < polygonNumX + 1; x++)
			{
				for (int z = 0; z < polygonNumZ + 1; z++)
				{
					int i = x * (polygonNumX + 1) + z;

					pVtx[i].Position = D3DXVECTOR3(-(polygonNumX / 2.0f) * sizeX + z * sizeX, (float)height, (polygonNumZ / 2.0f) * sizeZ - x * sizeZ);
					pVtx[i].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//法線ベクトル
					pVtx[i].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[i].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
				}
			}
		}

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	//インデックスバッファの中身を埋める
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		// インデックスバッファのポインタを取得
		Renderer::GetDeviceContext()->Map(m_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short* pIdx = (unsigned short*)msr.pData;

		int idx = 0;
		int i = 0;
		for (int x = 0; x < polygonNumX; x++)
		{
			for (int z = 0; z < polygonNumZ + 1; z++)
			{
				pIdx[i] = (polygonNumZ + 1) + z + (polygonNumZ + 1) * x;
				i++;

				pIdx[i] = x * (polygonNumZ + 1) + z;
				i++;
			}

			if (x < (polygonNumX - 1))
			{
				pIdx[i] = polygonNumZ + (polygonNumZ + 1) * x;
				i++;

				pIdx[i] = (polygonNumZ + 1) * 2 + (polygonNumZ + 1) * x;
				i++;
			}
		}

		Renderer::GetDeviceContext()->Unmap(m_IndexBuffer, 0);

		//インデックスバッファ用意
		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\deferredGeometryVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader, "shader\\deferredGeometryPS.cso");
	}
}

void Waveface::Uninit()
{
}

void Waveface::Update()
{
	for(int i = 0; i < polygonNumX + 1; i++)
	{
		for(int j = 0; j < polygonNumZ + 1; j++)
		{
			int n = i * (polygonNumX + 1) + j;
			switch(m_WavePoints[n].status)
			{
				case 0:
					m_WavePoints[n].position = D3DXVECTOR3(-(polygonNumX / 2.0f) * sizeX + j * sizeX, (float)height, (polygonNumZ / 2.0f) * sizeZ - i * sizeZ);
								 	
					m_WavePoints[n].amplitude = WAVE_AMPLITUDE;	// 振幅
					m_WavePoints[n].time = 0.0f;
					m_WavePoints[n].status = 1;

				case 1:
					// 振動原点からの距離を求める
					float valx = 0.0f - m_WavePoints[n].position.x;
					float valz = 0.0f - m_WavePoints[n].position.z;
					float length = sqrtf(valx * valx + valz * valz);

					// 距離を元に振動の式から変位（波高）を求める
					m_WavePoints[n].position.y = m_WavePoints[n].amplitude * sinf(2 * 3.14f * (length / WAVE_LENGTH - m_WavePoints[n].time / WAVE_CYCLE));


					// 時間を進める
					m_WavePoints[n].time += 0.1f;

					break;
			}
		}
	}

	D3D11_MAPPED_SUBRESOURCE msr;
	ZeroMemory(&msr, sizeof(msr));
	auto hr = Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* posbuf = static_cast<VERTEX_3D*>(msr.pData);
	for(int i = 0; i < polygonNumX + 1; i++)
	{
		for(int j = 0; j < polygonNumZ + 1; j++)
		{
			int n = i * (polygonNumX + 1) + j;
			(posbuf + n)->Position.x = m_WavePoints[n].position.x;
			(posbuf + n)->Position.y = m_WavePoints[n].position.y;
			(posbuf + n)->Position.z = m_WavePoints[n].position.z;
			(posbuf + n)->Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			(posbuf + n)->Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			(posbuf + n)->TexCoord = D3DXVECTOR2(i * 0.5f, j * 0.5f);
		}
	}

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

}

void Waveface::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//　マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = false;
	Renderer::SetMaterial(material);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed(m_numIndex, 0, 0);

}

#include "main.h"
#include "renderer.h"
#include "Polygon3D.h"
#include "manager.h"
#include "CtrlCamera.h"

Polygon3D::Polygon3D()
{
	m_Component.clear();
	Polygon3D::Init();
}

Polygon3D::Polygon3D(Polygon3D& copy)
{
	m_Component.clear();
	Polygon3D::Init();
}

Polygon3D::~Polygon3D()
{
	Polygon3D::Uninit();
}

void Polygon3D::Init()
{
// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_Vertexbuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * 8;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_Indexbuffer);


	{
		{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
			D3D11_MAPPED_SUBRESOURCE msr;
			Renderer::GetDeviceContext()->Map(m_Vertexbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
			VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

			{
				pVtx[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
				pVtx[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				pVtx[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
				pVtx[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				pVtx[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
				pVtx[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				pVtx[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
				pVtx[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			}

			Renderer::GetDeviceContext()->Unmap(m_Vertexbuffer, 0);
		}

		{//インデックスバッファの中身を埋める

			// インデックスバッファのポインタを取得
			D3D11_MAPPED_SUBRESOURCE msr;
			Renderer::GetDeviceContext()->Map(m_Indexbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			unsigned short* pIdx = (unsigned short*)msr.pData;

			int idx = 0;

			{
				pIdx[0] = 2;
				pIdx[1] = 0;
				pIdx[2] = 3;
				pIdx[3] = 1;
				pIdx[4] = 3;
				pIdx[5] = 1;
				pIdx[6] = 2;
				pIdx[7] = 0;
			}

			Renderer::GetDeviceContext()->Unmap(m_Indexbuffer, 0);
		}
	}


	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
										   "asset\\texture\\MYWORLDCREATOR.png",
										   NULL, NULL,
										   &m_Texture,
										   NULL);

	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VSBuffer, &m_InputLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PSBuffer, "shader\\unlitTexturePS.cso");

}

void Polygon3D::Uninit()
{
}

void Polygon3D::Update()
{
}

void Polygon3D::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(m_InputLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VSBuffer, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PSBuffer, NULL, 0);

		
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_Vertexbuffer, &stride, &offset);

	Renderer::GetDeviceContext()->IASetIndexBuffer(m_Indexbuffer, DXGI_FORMAT_R16_UINT, 0);

	//　マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	Renderer::SetATCEnable(true);


	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed(8, 0, 0);
}

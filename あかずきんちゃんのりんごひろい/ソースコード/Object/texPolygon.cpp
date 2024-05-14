#include"texPolygon.h"

/********************************************************************************************/
//class PTEX
/********************************************************************************************/

PTEX::PTEX() {
	g_VertexBuffer = NULL;
	texData = NULL;

	texset.pos = { 0.0f, 0.0f };
	texset.wh = { 0.0f, 0.0f };
	texset.u = {0.0f, 0.0f, 0.0f};
	texset.v = {0.0f, 0.0f, 0.0f};

}

PTEX::~PTEX() {

}

void PTEX::initData(char* tp, Float2 p, Float2 w, Float3 u, Float3 v ) {
	texPass = tp;
	texset.pos = p;
	texset.wh = w;
	texset.u = u;
	texset.v = v;

	ID3D11Device* pDevice = GetDevice();
	D3DX11CreateShaderResourceViewFromFile(
		pDevice,
		texPass,
		NULL,
		NULL,
		&texData,
		NULL);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	SetVertex();
}

void PTEX::updateData()
{
}

void PTEX::drawData() {
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &texData);

	SetVertex();

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

void PTEX::SetVertex() {

	//OpenGLからの座標系変換
	//{(SCREEN_WIDTH / 2 - p.x), (SCREEN_HEIGHT / 2 - (-1 * p.y))}
	Float2 fromNSPosition = { (SCREEN_WIDTH / 2 + texset.pos.x) , (SCREEN_HEIGHT / 2 - (-1 * texset.pos.y)) };

	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(fromNSPosition.x - texset.wh.x / 2, fromNSPosition.y - texset.wh.y / 2, 0.0f);
	vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(texset.u.x, texset.v.x);

	vertex[1].Position = D3DXVECTOR3(fromNSPosition.x + texset.wh.x / 2, fromNSPosition.y - texset.wh.y / 2, 0.0f);
	vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(texset.u.y, texset.v.x);

	vertex[2].Position = D3DXVECTOR3(fromNSPosition.x - texset.wh.x / 2, fromNSPosition.y + texset.wh.y / 2, 0.0f);
	vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(texset.u.x, texset.v.y);

	vertex[3].Position = D3DXVECTOR3(fromNSPosition.x + texset.wh.x / 2, fromNSPosition.y + texset.wh.y / 2, 0.0f);
	vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(texset.u.y, texset.v.y);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}


void PTEX::unInitData() {
	if (texData) {
		texData->Release();
		texData = NULL;
	}

	if (g_VertexBuffer) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

bool PTEX::checkCollision(PTEX target)
{
	bool colFlag = false;
	Float3 thisCircle = { texset.pos.x, texset.pos.y, texset.wh.x / 2 };
	Float3 targetCircle = { target.texset.pos.x, target.texset.pos.y, target.texset.wh.x / 2 };

	if (sqrtf((thisCircle.x - targetCircle.x) * (thisCircle.x - targetCircle.x)
		+ (thisCircle.y - targetCircle.y) * (thisCircle.y - targetCircle.y))
		<= (thisCircle.z + targetCircle.z) - 30) {
		colFlag = true;
	}

	return colFlag;
}
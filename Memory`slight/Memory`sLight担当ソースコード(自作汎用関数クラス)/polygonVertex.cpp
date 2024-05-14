#include<cmath>
#include "polygonVertex.h"
#include "renderer.h"

//描画用頂点バッファへverData[4]を移動
void POLYVERTEX::SetVertex()
{
	//頂点バッファ(vertexVuffer)のセット
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//バッファへの頂点情報のセット
	vertex[0].Position = verData[0].position;
	vertex[0].Diffuse = verData[0].diffuse;
	vertex[0].TexCoord = verData[0].texcoord;

	vertex[1].Position = verData[1].position;
	vertex[1].Diffuse = verData[1].diffuse;
	vertex[1].TexCoord = verData[1].texcoord;
	
	vertex[2].Position = verData[2].position;
	vertex[2].Diffuse = verData[2].diffuse;
	vertex[2].TexCoord = verData[2].texcoord;
	
	vertex[3].Position = verData[3].position;
	vertex[3].Diffuse = verData[3].diffuse;
	vertex[3].TexCoord = verData[3].texcoord;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}

//ポリゴンの中心を原点に移動
void POLYVERTEX::setOrigin()
{
	verData[0].position = D3DXVECTOR3(-polyData.polyWh.x / 2, -polyData.polyWh.y / 2, 0.0f);

	verData[1].position = D3DXVECTOR3(polyData.polyWh.x / 2, -polyData.polyWh.y / 2, 0.0f);

	verData[2].position = D3DXVECTOR3(-polyData.polyWh.x / 2, polyData.polyWh.y / 2, 0.0f);

	verData[3].position = D3DXVECTOR3(polyData.polyWh.x / 2, polyData.polyWh.y / 2, 0.0f);
}

//ポリゴンの中心を"polyData.polyPos"に移動
void POLYVERTEX::resetPosition()
{
	verData[0].position.x += polyData.polyPos.x;
	verData[0].position.y += polyData.polyPos.y;

	verData[1].position.x += polyData.polyPos.x;
	verData[1].position.y += polyData.polyPos.y;

	verData[2].position.x += polyData.polyPos.x;
	verData[2].position.y += polyData.polyPos.y;

	verData[3].position.x += polyData.polyPos.x;
	verData[3].position.y += polyData.polyPos.y;
}

//テクスチャアニメーションのU値処理{ アニメーションの更新フレーム数 }
void POLYVERTEX::texAnimetionU(int updateTime)
{

	if (fCount % updateTime == 0) {
		polyData.texU.x = polyData.texU.y;
		polyData.texU.y += polyData.texU.z;
		if (polyData.texU.x == 1.0f) {
			polyData.texU.x = 0.0f;
			polyData.texU.y = polyData.texU.z;
		}
	}

	verData[0].texcoord.x = polyData.texU.x;

	verData[1].texcoord.x = polyData.texU.y;

	verData[2].texcoord.x = polyData.texU.x;

	verData[3].texcoord.x = polyData.texU.y;
}

//テクスチャアニメーションのV値処理{ テクスチャの上からいくつめのアニメーションか }
void POLYVERTEX::texAnimetionV(int uNum)
{
	polyData.texV.x = polyData.texV.z * (uNum - 1);
	polyData.texV.y = polyData.texV.z * uNum;

	verData[0].texcoord.y = polyData.texV.x;

	verData[1].texcoord.y = polyData.texV.x;

	verData[2].texcoord.y = polyData.texV.y;

	verData[3].texcoord.y = polyData.texV.y;

}

int POLYVERTEX::SetTexture(char* tpc, Float3 u, Float3 v)
{

	ID3D11Device* pDevice = GetDevice();
	D3DX11CreateShaderResourceViewFromFile(
		pDevice,
		tpc,
		NULL,
		NULL,
		&textures[texNum].texData,
		NULL);

	textures[texNum].texU = u;
	textures[texNum].texV = v;

	texNum++;
	return texNum - 1;
}

void POLYVERTEX::ChangeTexture(int n)
{
	if (n >= texNum) {
		return;
	}

	nowTexNum = n;
	polyData.texU = textures[nowTexNum].texU;
	polyData.texV = textures[nowTexNum].texV;

	verData[0].texcoord = D3DXVECTOR2(polyData.texU.x, polyData.texV.x);
	verData[1].texcoord = D3DXVECTOR2(polyData.texU.y, polyData.texV.x);
	verData[2].texcoord = D3DXVECTOR2(polyData.texU.x, polyData.texV.y);
	verData[3].texcoord = D3DXVECTOR2(polyData.texU.y, polyData.texV.y);

}

void POLYVERTEX::initData(std::string* t, const char* tp, Float2 p, Float2 w, Float3 u, Float3 v)
{
	tag = t;
	polyData.polyPos = p;
	polyData.polyWh = w;
	polyData.texU = u;
	polyData.texV = v;

	resetVerData();


	D3DX11CreateShaderResourceViewFromFile(
		GetDevice(),
		tp,
		NULL,
		NULL,
		&textures[0].texData,
		NULL);
	assert(textures[0].texData);
	texNum++;
	textures[0].texU = u;
	textures[0].texV = v;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	SetVertex();
}

void POLYVERTEX::updateData()
{
}

void POLYVERTEX::drawData()
{
	fCount++;//フレームカウント

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();//座標の2D変換

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &textures[nowTexNum].texData);

	SetVertex();

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}

void POLYVERTEX::unInitData()
{
	for (auto t : textures) {
		if (t.texData) {
			t.texData->Release();
			t.texU = {};
			t.texV = {};
		}
	}

	if (g_VertexBuffer) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

bool POLYVERTEX::polyCollisionCheck(POLYVERTEX& target)
{
	float xResult = (abs(this->polyData.polyPos.x - target.polyData.polyPos.x));
	float yResult = (abs(this->polyData.polyPos.y - target.polyData.polyPos.y));
	float xWhResult = (this->polyData.polyWh.x / 2 + target.polyData.polyWh.x / 2);
	float yWhResult = (this->polyData.polyWh.y / 2 + target.polyData.polyWh.y / 2);

	bool xCollision = (xResult <= xWhResult);
	bool yCollision= (yResult <= yWhResult);

	if (xCollision && yCollision) {
		return true;
	}
	
	return false;
}

DIRECTION POLYVERTEX::polyDirectionCheck(POLYVERTEX& target)
{
	D3DXVECTOR3 bwVec = { target.polyData.polyPos.x - polyData.polyPos.x, target.polyData.polyPos.y - polyData.polyPos.y, 0.0f };
	D3DXVECTOR3 tgVec = { target.polyData.polyPos.x , 0.0f, 0.0f };

	D3DXVec3Normalize(&bwVec, &bwVec);
	D3DXVec3Normalize(&tgVec, &tgVec);

	float dot = bwVec.x * tgVec.x + bwVec.y * tgVec.y;
	float cross = bwVec.x * tgVec.y - bwVec.y * tgVec.x;

	bool isRight = (dot < 0);
	bool isUp = (cross < 0);

	if (dot == 0 && cross == 0) {
		return DIRECTION::MATCH;
	}

	if (cross < 0.5f && cross > -0.5f) {
		if (isRight) return DIRECTION::RIGHT;
		else return DIRECTION::LEFT;
	}
	else {
		if (isUp) return DIRECTION::UP;
		else return DIRECTION::DOWN;
	}

	return DIRECTION::FAILED;
}

COLLISIONDATA POLYVERTEX::polyCollisionDataCheck(POLYVERTEX& target)
{
	COLLISIONDATA result = { DIRECTION::FAILED, (std::string*)"NODATA" };
	if (!polyCollisionCheck(target)) {
		return result;
	}
	D3DXVECTOR2 baseVec(-target.polyData.polyWh.x, -target.polyData.polyWh.y );
	D3DXVec2Normalize(&baseVec, &baseVec);
	D3DXVECTOR2 yVec(0.0f, -1.0f);
	float baseAngle = baseVec.x * yVec.x + baseVec.y * yVec.y;

	D3DXVECTOR2 distVec(target.polyData.polyPos.x - polyData.polyPos.x, target.polyData.polyPos.y - polyData.polyPos.y);
	D3DXVec2Normalize(&distVec, &distVec);
	float down = fabs(distVec.x * yVec.x + distVec.y * yVec.y);

	if (baseAngle > down) {
		/*横向きに衝突*/
		D3DXVECTOR3 bwVec = { target.polyData.polyPos.x - polyData.polyPos.x, target.polyData.polyPos.y - polyData.polyPos.y, 0.0f };
		D3DXVECTOR3 tgVec = { -(target.polyData.polyWh.x / 2) , 0.0f, 0.0f };

		float dot = bwVec.x * tgVec.x + bwVec.y * tgVec.y;

		if (dot < 0) {
			result.direction = DIRECTION::LEFT;
			result.tag = target.tag;
		}
		else {
			result.direction = DIRECTION::RIGHT;
			result.tag = target.tag;
		}
	}
	else {
		/*縦向きに衝突*/
		D3DXVECTOR3 bwVec = { target.polyData.polyPos.x - polyData.polyPos.x, target.polyData.polyPos.y - polyData.polyPos.y, 0.0f };
		D3DXVECTOR3 tgVec = { 0.0f, -(target.polyData.polyWh.y / 2), 0.0f };

		float dot = bwVec.x * tgVec.x + bwVec.y * tgVec.y;

		if (dot < 0) {
			result.direction = DIRECTION::UP;
			result.tag = target.tag;
		}
		else{
			result.direction = DIRECTION::DOWN;
			result.tag = target.tag;
		}
	}

	return result;
}

//ポリゴンの平行移動{xの増分, yの増分}
void POLYVERTEX::polyTranslation(Float2 incremental)
{
	polyData.polyPos.x += incremental.x;
	polyData.polyPos.y += incremental.y;

	verData[0].position.x += incremental.x;
	verData[0].position.y += incremental.y;

	verData[1].position.x += incremental.x;
	verData[1].position.y += incremental.y;

	verData[2].position.x += incremental.x;
	verData[2].position.y += incremental.y;

	verData[3].position.x += incremental.x;
	verData[3].position.y += incremental.y;
}

void POLYVERTEX::polyTransPoint(Float2 Point)
{
	Float2 inc;
	inc.x = Point.x - polyData.polyPos.x;
	inc.y = Point.y - polyData.polyPos.y;

	polyTranslation(inc);
}

void POLYVERTEX::polyTransXPoint(float Point)
{
	Float2 inc;
	inc.x = Point - polyData.polyPos.x;
	inc.y = 0.0f;

	polyTranslation(inc);
}

void POLYVERTEX::polyTransYPoint(float Point)
{
	Float2 inc;
	inc.x = 0.0f;
	inc.y = Point - polyData.polyPos.y;

	polyTranslation(inc);
}

//ポリゴンの拡大縮小{倍率}
void POLYVERTEX::polyScalling(float magnification)
{
	setOrigin();

	polyData.polyWh.x *= magnification;
	polyData.polyWh.y *= magnification;

	verData[0].position.x *= magnification;
	verData[0].position.y *= magnification;

	verData[1].position.x *= magnification;
	verData[1].position.y *= magnification;

	verData[2].position.x *= magnification;
	verData[2].position.y *= magnification;

	verData[3].position.x *= magnification;
	verData[3].position.y *= magnification;

	resetPosition();
}

//ポリゴンの回転{回転角度(度)}
void POLYVERTEX::polyRotation(float degree)
{
	float radian = degree / 100.0f;

	float hw, hh;

	hw = polyData.polyWh.x * 0.5f;
	hh = polyData.polyWh.y * 0.5f;

	float BaseAngle = atan2f(hh, hw);		// 中心点から頂点に対する角度
	D3DXVECTOR2 temp = D3DXVECTOR2(hw, hh);
	float Radius = D3DXVec2Length(&temp);	// 中心点から頂点に対する距離

	float x = polyData.polyPos.x - cosf(BaseAngle + radian) * Radius;
	float y = polyData.polyPos.y - sinf(BaseAngle + radian) * Radius;
	verData[0].position.x = x;
	verData[0].position.y = y;

	x = polyData.polyPos.x + cosf(BaseAngle - radian) * Radius;
	y = polyData.polyPos.y - sinf(BaseAngle - radian) * Radius;
	verData[1].position.x = x;
	verData[1].position.y = y;

	x = polyData.polyPos.x - cosf(BaseAngle - radian) * Radius;
	y = polyData.polyPos.y + sinf(BaseAngle - radian) * Radius;
	verData[2].position.x = x;
	verData[2].position.y = y;

	x = polyData.polyPos.x + cosf(BaseAngle + radian) * Radius;
	y = polyData.polyPos.y + sinf(BaseAngle + radian) * Radius;
	verData[3].position.x = x;
	verData[3].position.y = y;

}

//タグの取得
std::string POLYVERTEX::getTag()
{
	return *tag;
}

//頂点情報を生成時のデータで更新
void POLYVERTEX::resetVerData()
{
	verData[0].position = D3DXVECTOR3(polyData.polyPos.x - polyData.polyWh.x / 2, polyData.polyPos.y - polyData.polyWh.y / 2, 0.0f);
	verData[0].texcoord = D3DXVECTOR2(polyData.texU.x, polyData.texV.x);

	verData[1].position = D3DXVECTOR3(polyData.polyPos.x + polyData.polyWh.x / 2, polyData.polyPos.y - polyData.polyWh.y / 2, 0.0f);
	verData[1].texcoord = D3DXVECTOR2(polyData.texU.y, polyData.texV.x);

	verData[2].position = D3DXVECTOR3(polyData.polyPos.x - polyData.polyWh.x / 2, polyData.polyPos.y + polyData.polyWh.y / 2, 0.0f);
	verData[2].texcoord = D3DXVECTOR2(polyData.texU.x, polyData.texV.y);

	verData[3].position = D3DXVECTOR3(polyData.polyPos.x + polyData.polyWh.x / 2, polyData.polyPos.y + polyData.polyWh.y / 2, 0.0f);
	verData[3].texcoord = D3DXVECTOR2(polyData.texU.y, polyData.texV.y);
}

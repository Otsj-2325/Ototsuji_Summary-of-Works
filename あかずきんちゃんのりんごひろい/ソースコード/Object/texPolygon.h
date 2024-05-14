#pragma once

#include<cmath>
#include"main.h"
#include"renderer.h"
#include"input.h"
#include"randam.h"
#include"texture.h"

/********************************************************/
//TexturePolygon用抽象クラス
/********************************************************/
class PTEX {
private:

protected:
typedef struct {
	Float2 pos;
	Float2 wh;
	Float3 u;
	Float3 v;

}TEXSET;
	ID3D11Buffer* g_VertexBuffer = 0;//polygon各点の座標バッファ
	ID3D11ShaderResourceView* texData = 0;
	char* texPass = 0;//TextureDirectoryPass
	TEXSET texset = { {0.0f, 0.0f},{0.0f, 0.0f},
			{0.0f, 0.0f},{0.0f, 0.0f} };//texset構造体

	virtual void SetVertex();
public:
	PTEX();
	~PTEX();

	virtual void initData(char* tp, Float2 p, Float2 w, Float3 u, Float3 v );
	virtual void updateData();
	virtual void drawData();
	virtual void unInitData();
	bool checkCollision(PTEX target);
	Float2 getPos() { return texset.pos; }
	Float2 getWh() { return texset.wh; }

};


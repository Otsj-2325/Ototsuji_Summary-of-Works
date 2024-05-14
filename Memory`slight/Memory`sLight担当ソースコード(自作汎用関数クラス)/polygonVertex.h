/*==============================================================================

   [PolygonDrawer.h] 
   
	 Author :H.Ototsuji
 	 Date   :2022/10/28
==============================================================================*/

#pragma once

#include<cmath>
#include<string>
#include"main.h"

//位置関係を表すための列挙体
enum class DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	MATCH,
	FAILED,
};

//ポリゴンのデータ保持用構造体
struct POLYGONDATA {
	Float2 polyPos = { 0.0f };//ポリゴンの中心座標{X値, V値}
	Float2 polyWh = { 0.0f };//ポリゴンの幅高さ{幅, 高さ}
	Float3 texU = { 0.0f };//ポリゴンのU値{始点値, 終点値, 増分値}
	Float3 texV = { 0.0f };//ポリゴンのV値{始点値, 終点値, 増分値}
};

struct COLLISIONDATA {
	DIRECTION direction;
	std::string* tag;
};

struct TEXTUREDATA {
	ID3D11ShaderResourceView* texData;
	Float3 texU;
	Float3 texV;
};

//頂点のデータ保持用構造体
struct POLYGONVERTEXDATA {
	D3DXVECTOR3 position = { 0.0f, 0.0f, 0.0f };//頂点の座標{X値, Y値, Z値(0.0f)}
	D3DXCOLOR diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };//頂点カラー{R値, G値, B値, A値}
	D3DXVECTOR2 texcoord = { 0.0f, 0.0f };//頂点UV値{U値, V値}
};

class POLYVERTEX {
protected:
	POLYGONDATA polyData;//ポリゴンのデータ
	POLYGONVERTEXDATA verData[4];//各頂点のデータ(0:左上, 1:右上, 2:左下, 3:右下)
	std::string* tag;//タグデータ(未実装)

private:
	ID3D11Buffer* g_VertexBuffer = 0;//polygon各点の座標バッファ
	TEXTUREDATA textures[10];//読み込んだテクスチャデータのポインタ
	int texNum = 0;
	int nowTexNum = 0;
	int fCount = 0;//生成後フレーム数カウント(drawで加算)

	void SetVertex();//描画用頂点バッファへverData[4]を移動
	void setOrigin();//ポリゴンの中心を原点に移動
	void resetPosition();//ポリゴンの中心を"polyData.polyPos"に移動

protected:
	virtual void initData(std::string* t, const char* tp, Float2 p, Float2 w, Float3 u, Float3 v);
	virtual void unInitData();
	virtual void updateData();
	virtual void drawData();

public:
	void polyTranslation(Float2 incremental);//ポリゴンの平行移動{xの増分, yの増分}
	void polyTransPoint(Float2 Point);//ポリゴンの平行移動{移動先の座標(Float2)}
	void polyTransXPoint(float Point);//ポリゴンのX軸平行移動{移動先のX座標(float)}
	void polyTransYPoint(float Point);//ポリゴンのY軸平行移動{移動先のY座標(float)}
	void polyScalling(float magnification);//ポリゴンの拡大縮小{倍率}
	void polyRotation(float degree);//ポリゴンの回転{回転角度(度)}
	
	void texAnimetionU(int updateTime);//テクスチャアニメーションのU値処理{アニメーションの更新フレーム数}
	void texAnimetionV(int uNum);//{テクスチャアニメーションのV値処理{テクスチャの上からいくつめのアニメーションか}

	int SetTexture(char* tpc, Float3 u, Float3 v);

	void ChangeTexture(int n);

	std::string getTag();

	void resetVerData();//頂点情報を生成時のデータで更新

	bool polyCollisionCheck(POLYVERTEX& target);//ポリゴン同士の衝突判定{ターゲットポリゴン}
	DIRECTION polyDirectionCheck(POLYVERTEX& target);//ポリゴン同士の位置関係{ターゲットポリゴン}
	COLLISIONDATA polyCollisionDataCheck(POLYVERTEX& target);


	const POLYGONDATA getPolyData() { return polyData; };
};

class SEAL :public POLYVERTEX {
public:
	void updateData()override {};
};
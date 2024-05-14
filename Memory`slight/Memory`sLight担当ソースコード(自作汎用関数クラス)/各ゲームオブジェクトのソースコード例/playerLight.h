#pragma once

#include "polygonVertex.h"
#include "sceneBlock.h"
#include "Camera.h"

class LIGHTPLAYER : public POLYVERTEX
{
private:
	float speed = 10.0f;							// 移動速度
	bool m_isPlayerMove = false;					// 移動してるかの判定
	DIRECTION m_eFace = DIRECTION::RIGHT;			// プレイヤーの向き
	int m_eBlock = 0;								// プレイヤーがいるブロック
	int m_PlayerBlock = 0;							// 
	int m_lightBlock = 0;							// 
	int m_oldLightBlock = -1;						// ライトしているブロックを記録する関数
	int m_blockDiffer = 0;
	int m_nextBlock = 0;
	Float2 m_nextPos = { 0.0f,0.0f };
	bool m_stop = false;


public:
	LIGHTPLAYER() { POLYVERTEX::polyActivate(); }

	void updateData(void) override;					// アップデート
	void PlayerDirection(void);					//プレイヤーが左右どちらにいるか
	void PlayerScroll(void);							// 動き
	void PlayerAnime(void);							// アニメーション

	void setLightBlock(int lb) { m_lightBlock = lb; };
	void setPlayerBlock(int pb) { m_PlayerBlock = pb; };

	DIRECTION getPlayereFace() { return m_eFace; }
	int getPlayerBlock() { return m_eBlock; }			// ブロックをもらう

	void initData(char* t, char* tp);

	void PlayerBlock();
	void PlayerGoRight();
	void PlayerGoLeft();
	void PlayerChangeBlock();

	DIRECTION getPlayerDire() { return m_eFace; }

	bool getStopflag() { return m_stop; }

};
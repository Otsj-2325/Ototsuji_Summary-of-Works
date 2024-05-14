#pragma once

#include "polygonVertex.h"
#include "sceneBlock.h"
#include"sleepyEnemy.h"
#include"sound.h"

class ENEMY : public POLYVERTEX
{
private:
	Float2 m_vel = { 3.0f, 5.0f };	// 移動速度
	STATUS m_status = STAY;		// 状態
	Float2 m_basePos = { 0.0f, 0.0f };	// 最初の位置

	DIRECTION m_face = DIRECTION::LEFT;	// エネミーの向き
	int m_block = 1;	// エネミーをいるブロック

	int m_lightBlock = 0;	// ライトしているブロック
	int m_oldLightBlock = -1;	// ライトしているブロックを記録する関数
	DIRECTION m_lightBlockFace = DIRECTION::LEFT;	// ライトしているブロックの向き
	
	ENEMYEFFECT effect;

	int g_BGMNo = 0;
	
public:
	void initData(char* t, char* tp, Float2 p, Float2 w, Float3 u, Float3 v);
	void updateData(void) override;	// アップデート（ライトのオンオフに関わらない）
	void LightUpdateData(void);		// アップデート（ライトオン）
	void noLightUpdateData(void);	// アップデート（ライトオフ）
	void drawData();
	void unInitData();

	void EnemyMove(void);	// 動き
	void EnemyAnime(void);	// アニメーション
	void EnemyBlock(void);	// ブロック判定

	void isLightBlockNext(void);	// ライトしているブロックは隣にいるかの判定
	void setLightBlock(int n) { m_lightBlock = n; }	// ライトしているブロックを読み込む
	DIRECTION getEnemyFace() { return m_face; }	// 向きをもらう
	int getEnemyBlock() { return m_block; }	// ブロックをもらう

	void targetChase(POLYVERTEX& target);	// ターゲットを追いかける
	void setEnemyBasePos(Float2 basePos) { m_basePos = basePos; };	// 基本位置を設定
	void returnBasePosY(void);	// 基本高さを戻る
	void setStatusReturn(void);	// 状態はRETURNを設定する
	void SetFace(DIRECTION face) {
		m_face = face;
	}
};
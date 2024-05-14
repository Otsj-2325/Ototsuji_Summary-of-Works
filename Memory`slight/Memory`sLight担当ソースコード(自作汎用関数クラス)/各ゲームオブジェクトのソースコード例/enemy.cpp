#include "enemy.h"

void ENEMY::initData(char* t, char* tp, Float2 p, Float2 w, Float3 u, Float3 v)
{
	setEnemyBasePos(p);
	POLYVERTEX::initData(t, tp, p, w, u, v);

	effect.initData((char*)"enemyEffect", (char*)"data/CharactorTextures/enemyEffect_01.png", p, w, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	effect.setColour({ 1.0f, 1.0f, 1.0f, 0.3f });

	g_BGMNo = LoadSound((char*)"data/BGM/packun_eating.wav");

}

void ENEMY::updateData(void)
{
	EnemyBlock();

	switch (m_status)
	{
	case SLEEP:
		m_status = STAY;
		break;
	case STAY:
		EnemyAnime();
		break;
	case MOVE:
		EnemyAnime();
		EnemyMove();
		break;
	case RETURN:
		EnemyAnime();
		returnBasePosY();
		break;
	}

	effect.polyTransPoint(polyData.polyPos);
}

void ENEMY::LightUpdateData(void)
{
	isLightBlockNext();
}

void ENEMY::noLightUpdateData() {
}

void ENEMY::drawData()
{
	POLYVERTEX::drawData();
	effect.drawData();
}

void ENEMY::unInitData()
{
	POLYVERTEX::unInitData();
	effect.unInitData();
}

void ENEMY::isLightBlockNext(void)
{
	int blockDiffer = (m_block - m_lightBlock);	// ブロック差

	if (blockDiffer == 1 || blockDiffer == -1) {	// ライトしたブロックの左右判定
		if (blockDiffer == 1)
			m_lightBlockFace = DIRECTION::LEFT;
		else if (blockDiffer == -1)
			m_lightBlockFace = DIRECTION::RIGHT;

		m_oldLightBlock = m_lightBlock;
		m_status = MOVE;
	}
}

void ENEMY::targetChase(POLYVERTEX & target)
{
	m_status = CHASE;
	polyTargetChase(target, m_vel);
	PlaySound(g_BGMNo, -1);
}

void ENEMY::returnBasePosY(void)
{
	float blockCenter = (m_block + 1)* SBLOCK_CENTER_X * 2 - SBLOCK_CENTER_X;	// ブロックの真ん中

	if (polyData.polyPos.x >= blockCenter + 3.0f || polyData.polyPos.x <= blockCenter - 3.0f) {
		if (polyData.polyPos.x > blockCenter) {	// ブロックの真ん中を戻る
			polyTranslation({ -m_vel.x, 0.0f });
			m_face = DIRECTION::LEFT;
		}
		if (polyData.polyPos.x < blockCenter) {
			polyTranslation({ m_vel.x, 0.0f });
			m_face = DIRECTION::RIGHT;
		}
	}

	if (polyData.polyPos.y >= m_basePos.y) {	// 基本高さを戻る
		polyTranslation({ 0.0f, -m_vel.y });
	}

	if (polyData.polyPos.y <= m_basePos.y && polyData.polyPos.x < blockCenter + 4.0f && polyData.polyPos.x > blockCenter - 4.0f) {
		m_status = STAY;
	}
}

void ENEMY::setStatusReturn(void)
{
	if (m_status == CHASE) {
		m_status = RETURN;
	}
}


void ENEMY::EnemyMove(void)
{
	float lightblockCenter = (m_oldLightBlock + 1)* SBLOCK_CENTER_X * 2 - SBLOCK_CENTER_X;

	if (m_lightBlockFace == DIRECTION::LEFT) {	// エネミー左移動
		if (polyData.polyPos.x > lightblockCenter) {	// 左ブロックの真ん中まで移動
			polyTranslation({ -m_vel.x, 0.0f });
		}
		else {
			m_status = STAY;	// 移動終了
		}
		m_face = DIRECTION::LEFT;	// エネミー左向き
	}
	if (m_lightBlockFace == DIRECTION::RIGHT) {	// エネミー右移動
		if (polyData.polyPos.x <= lightblockCenter) {	// 右ブロックの真ん中まで移動
			polyTranslation({ m_vel.x, 0.0f });
		}
		else {
			m_status = STAY;	// 移動終了
		}
		m_face = DIRECTION::RIGHT;	// エネミー右向き
	}
}

void ENEMY::EnemyAnime(void)
{
	Float3 pls = polyData.texU;

	if (m_face == DIRECTION::RIGHT) {
		if (polyData.texU.x < polyData.texU.y) {
			polyData.texU = { pls.y, pls.x,pls.z };
		}
		texAnimetionU(12, 0);
	}
	else if (m_face == DIRECTION::LEFT) {
		if (polyData.texU.x > polyData.texU.y) {
			polyData.texU = { pls.y, pls.x,pls.z };
		}
		texAnimetionU(12, 1);
	}
}

void ENEMY::EnemyBlock(void)
{
	for (int i = 0; i < SBLOCK_NUM; i++) {
		if (polyData.polyPos.x > (SBLOCK_CENTER_X * 2 * i) && polyData.polyPos.x < (SBLOCK_CENTER_X * 2 * (i + 1))) {
			m_block = i;

		}
	}
}
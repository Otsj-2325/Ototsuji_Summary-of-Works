#include "playerLight.h"

#define SPEED 2.2f

//初期化処理
//scene0Master.cppのほうで、いちいち引数を書かなくていいようにしたよ
void LIGHTPLAYER::initData(char* t, char* tp)
{
	Float2 p = { 10.0f, SCREEN_HEIGHT - 120.0f };
	Float2 w = { SCREEN_WIDTH * 0.05f, SCREEN_HEIGHT * 0.15f };
	Float3 u = { 0.0f, (1.0f / 4), (1.0f / 4) };
	Float3 v = { 0.0f, 1.0f, 1.0f };

	POLYVERTEX::initData(t, tp, p, w, u, v);
	texAnimetionV(1);
}

//下のプレイヤーのアップデート
void LIGHTPLAYER::updateData(void)
{
	PlayerBlock();
	PlayerDirection();
	PlayerScroll();
	PlayerChangeBlock();
	PlayerAnime();
}

void LIGHTPLAYER::PlayerBlock(void)
{
	for (int i = 0; i < SBLOCK_NUM; i++) {
		if (polyData.polyPos.x > (SBLOCK_CENTER_X * 2 * i) && polyData.polyPos.x < (SBLOCK_CENTER_X * 2 * (i + 1))) {
			m_eBlock = i;
		}
	}
}

//画面スクロール時の移動処理とか
void LIGHTPLAYER::PlayerScroll(void)
{
	//Cameraのエリアチェンジフラグがtrueの時だけ進みます
	if (AreaChangeFlag()) {

		//動いてるよフラグ
		m_isPlayerMove = false;

		//スクロールのときは前だけを見て進みます
		m_eFace = DIRECTION::LEFT;

	}
	else {//フラグが折れたら止まります
		m_isPlayerMove = false;
	}
}


void LIGHTPLAYER::PlayerGoRight()
{
	if (polyData.polyPos.x < m_nextPos.x - 5.0f || polyData.polyPos.x > m_nextPos.x + 5.0f) {
		m_stop = false;
		m_isPlayerMove = true;
		float vx = 0.01f * (m_nextPos.x - polyData.polyPos.x);
		float plus = 0.0f;
		if (vx < 0.0f) {
			m_isPlayerMove = false;
			vx = 0.0f;
			plus = 0.0f;
		}
		else if (vx > 0.0f && vx < SPEED) {
			plus = SPEED;
		}
		if (vx > SPEED) {
			vx = SPEED;
		}
		polyTranslation({ vx + plus, 0.0f });
	}
	else {
		m_stop = true;
		m_isPlayerMove = false;
	}
}


void LIGHTPLAYER::PlayerGoLeft()
{
	if (polyData.polyPos.x < m_nextPos.x - 5.0f || polyData.polyPos.x > m_nextPos.x + 5.0f) {
		m_stop = false;
		m_isPlayerMove = true;
		float vx = 0.02f * (m_nextPos.x - polyData.polyPos.x);
		float plus = 0.0f;
		if (vx < 0.0f && vx > -SPEED) {
			plus = -SPEED;
		}
		else if (vx > 0.0f) {
			m_isPlayerMove = false;
			vx = 0.0f;
			plus = 0.0f;
		}
		if (vx < -SPEED) {
			vx = -SPEED;
		}
		polyTranslation({ vx + plus, 0.0f });
	}
	else {
		m_stop = true;
		m_isPlayerMove = false;
	}
}


void LIGHTPLAYER::PlayerChangeBlock()
{
	if (m_PlayerBlock > m_eBlock) {
		m_eFace = DIRECTION::LEFT;
		m_nextBlock = m_eBlock + 1;
	}
	else if (m_PlayerBlock < m_eBlock) {
		m_eFace = DIRECTION::RIGHT;
		m_nextBlock = m_eBlock - 1;
	}

	for (int i = 0; i < SBLOCK_NUM; i++)
	{
		if (m_nextBlock == i) {
			m_nextPos.x = (SCREEN_WIDTH / 6 - (((SCREEN_WIDTH / 6) / 2)) + (i * (SCREEN_WIDTH / 6)));
		}
	}
	PlayerGoRight();


	if (m_PlayerBlock < m_eBlock - 1) {
		PlayerGoLeft();
	}
}

//下のプレイヤーのアニメーション処理
void LIGHTPLAYER::PlayerAnime(void)
{
	Float3 pls = polyData.texU;

	if (m_eFace == DIRECTION::RIGHT) {
		if (polyData.texU.x < polyData.texU.y) {
			polyData.texU = { pls.y, pls.x,pls.z };
		}
		setTexU({ pls.z, 0.0f, pls.z });
	}
	else if (m_eFace == DIRECTION::LEFT) {
		if (polyData.texU.x > polyData.texU.y) {
			polyData.texU = { pls.y, pls.x,pls.z };
		}
		setTexU({ 0.0f, pls.z, pls.z });
	}

	//スクロールの時に歩くアニメーション
	if (m_isPlayerMove) {
		switch (m_eFace)
		{
		case DIRECTION::RIGHT:
			texAnimetionU(12, 0);
			break;

		case DIRECTION::LEFT:
			texAnimetionU(12, 1);
			break;
		}
	}
	else {
		if (m_eFace == DIRECTION::RIGHT) {
			setTexU({ 0.5f, 0.25f, pls.z });
		}
		else if (m_eFace == DIRECTION::LEFT) {
			setTexU({ 0.75f, 1.0f, pls.z });
		}
	}
}

//プレイヤーが左右どちらにいるか処理
void LIGHTPLAYER::PlayerDirection(void)
{
	if (polyData.polyPos.x < m_nextPos.x) {
		m_eFace = DIRECTION::LEFT;
	}
	else if (polyData.polyPos.x > m_nextPos.x) {
		m_eFace = DIRECTION::RIGHT;
	}
}
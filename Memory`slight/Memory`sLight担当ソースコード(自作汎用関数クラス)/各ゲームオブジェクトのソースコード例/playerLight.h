#pragma once

#include "polygonVertex.h"
#include "sceneBlock.h"
#include "Camera.h"

class LIGHTPLAYER : public POLYVERTEX
{
private:
	float speed = 10.0f;							// �ړ����x
	bool m_isPlayerMove = false;					// �ړ����Ă邩�̔���
	DIRECTION m_eFace = DIRECTION::RIGHT;			// �v���C���[�̌���
	int m_eBlock = 0;								// �v���C���[������u���b�N
	int m_PlayerBlock = 0;							// 
	int m_lightBlock = 0;							// 
	int m_oldLightBlock = -1;						// ���C�g���Ă���u���b�N���L�^����֐�
	int m_blockDiffer = 0;
	int m_nextBlock = 0;
	Float2 m_nextPos = { 0.0f,0.0f };
	bool m_stop = false;


public:
	LIGHTPLAYER() { POLYVERTEX::polyActivate(); }

	void updateData(void) override;					// �A�b�v�f�[�g
	void PlayerDirection(void);					//�v���C���[�����E�ǂ���ɂ��邩
	void PlayerScroll(void);							// ����
	void PlayerAnime(void);							// �A�j���[�V����

	void setLightBlock(int lb) { m_lightBlock = lb; };
	void setPlayerBlock(int pb) { m_PlayerBlock = pb; };

	DIRECTION getPlayereFace() { return m_eFace; }
	int getPlayerBlock() { return m_eBlock; }			// �u���b�N�����炤

	void initData(char* t, char* tp);

	void PlayerBlock();
	void PlayerGoRight();
	void PlayerGoLeft();
	void PlayerChangeBlock();

	DIRECTION getPlayerDire() { return m_eFace; }

	bool getStopflag() { return m_stop; }

};
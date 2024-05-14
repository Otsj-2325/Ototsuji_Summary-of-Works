#pragma once

#include "polygonVertex.h"
#include "sceneBlock.h"
#include"sleepyEnemy.h"
#include"sound.h"

class ENEMY : public POLYVERTEX
{
private:
	Float2 m_vel = { 3.0f, 5.0f };	// �ړ����x
	STATUS m_status = STAY;		// ���
	Float2 m_basePos = { 0.0f, 0.0f };	// �ŏ��̈ʒu

	DIRECTION m_face = DIRECTION::LEFT;	// �G�l�~�[�̌���
	int m_block = 1;	// �G�l�~�[������u���b�N

	int m_lightBlock = 0;	// ���C�g���Ă���u���b�N
	int m_oldLightBlock = -1;	// ���C�g���Ă���u���b�N���L�^����֐�
	DIRECTION m_lightBlockFace = DIRECTION::LEFT;	// ���C�g���Ă���u���b�N�̌���
	
	ENEMYEFFECT effect;

	int g_BGMNo = 0;
	
public:
	void initData(char* t, char* tp, Float2 p, Float2 w, Float3 u, Float3 v);
	void updateData(void) override;	// �A�b�v�f�[�g�i���C�g�̃I���I�t�Ɋւ��Ȃ��j
	void LightUpdateData(void);		// �A�b�v�f�[�g�i���C�g�I���j
	void noLightUpdateData(void);	// �A�b�v�f�[�g�i���C�g�I�t�j
	void drawData();
	void unInitData();

	void EnemyMove(void);	// ����
	void EnemyAnime(void);	// �A�j���[�V����
	void EnemyBlock(void);	// �u���b�N����

	void isLightBlockNext(void);	// ���C�g���Ă���u���b�N�ׂ͗ɂ��邩�̔���
	void setLightBlock(int n) { m_lightBlock = n; }	// ���C�g���Ă���u���b�N��ǂݍ���
	DIRECTION getEnemyFace() { return m_face; }	// ���������炤
	int getEnemyBlock() { return m_block; }	// �u���b�N�����炤

	void targetChase(POLYVERTEX& target);	// �^�[�Q�b�g��ǂ�������
	void setEnemyBasePos(Float2 basePos) { m_basePos = basePos; };	// ��{�ʒu��ݒ�
	void returnBasePosY(void);	// ��{������߂�
	void setStatusReturn(void);	// ��Ԃ�RETURN��ݒ肷��
	void SetFace(DIRECTION face) {
		m_face = face;
	}
};
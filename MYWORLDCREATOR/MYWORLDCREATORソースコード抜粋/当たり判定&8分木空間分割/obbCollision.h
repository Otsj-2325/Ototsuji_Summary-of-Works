#pragma once

#include "component.h"
#include "volumeBox.h"

typedef struct{
	D3DXVECTOR3 OBBColliderPosition;
	D3DXVECTOR3 OBBRotaion;
	D3DXVECTOR3 OBBUpVector;
	D3DXVECTOR3 OBBRightVector;
	D3DXVECTOR3 OBBForwardVector;
	D3DXVECTOR3 OBBScale;

}OBBConfigData;

class OBBCollider : public Component{
private:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Rot;
	float m_Length[3];
	D3DXVECTOR3 m_NormalDirection[3];
	VolumeBox m_vBox;
	bool m_ProcSelf = false;
	bool m_Active = false;

public:
	void Init(OBBConfigData* oc, GameObject* own);
	void Update();
	void Update(OBBConfigData* oc);
	void Draw();
	void ShowProperty();

	/// <summary>
	/// OBB�̈ʒu�擾
	/// </summary>
	/// <returns></returns>
	D3DXVECTOR3 GetPosition(){ return m_Pos; };
	
	/// <summary>
	/// OBB�̌����擾
	/// </summary>
	/// <param name="index"> 0:Right, 1:Up, 2:Forward </param>
	/// <returns></returns>
	D3DXVECTOR3 GetDirection(int index){ return m_NormalDirection[index]; };

	/// <summary>
	/// OBB�̌����x�N�g���̑傫���擾
	/// </summary>
	/// <param name="index"> 0:Right, 1:Up, 2:Forward </param>
	/// <returns></returns>
	float GetLength(int index){ return m_Length[index]; };

	/// <summary>
	/// OBB�̌����x�N�g���̑傫�����R�����x�N�g���Ƃ��Ď擾
	/// </summary>
	/// <returns></returns>
	D3DXVECTOR3 GetScale(){ return {m_Length[0], m_Length[1], m_Length[2]}; }

	/// <summary>
	/// �Փ˔��菈��
	/// </summary>
	/// <returns></returns>
	float GetLengthOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3);
	
	/// <summary>
	/// target�Ƃ̏Փ˔���
	/// </summary>
	/// <param name="target"></param>
	/// <returns></returns>
	bool OBBCheckCollision(OBBCollider* target);

	/// <summary>
	/// OBB�̂����ł��������_�̎Z�o
	/// </summary>
	/// <returns></returns>
	D3DXVECTOR3 GetMinPoint();

	/// <summary>
	/// OBB�̂����ł��傫���_�̎Z�o
	/// </summary>
	/// <returns></returns>
	D3DXVECTOR3 GetMaxPoint();

	/// <summary>
	/// �{�����[���{�b�N�X�̃R�s�[�C���X�^���X�擾
	/// </summary>
	/// <returns></returns>
	VolumeBox GetVBox(){ return m_vBox; }

};
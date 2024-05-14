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
	/// OBBの位置取得
	/// </summary>
	/// <returns></returns>
	D3DXVECTOR3 GetPosition(){ return m_Pos; };
	
	/// <summary>
	/// OBBの向き取得
	/// </summary>
	/// <param name="index"> 0:Right, 1:Up, 2:Forward </param>
	/// <returns></returns>
	D3DXVECTOR3 GetDirection(int index){ return m_NormalDirection[index]; };

	/// <summary>
	/// OBBの向きベクトルの大きさ取得
	/// </summary>
	/// <param name="index"> 0:Right, 1:Up, 2:Forward </param>
	/// <returns></returns>
	float GetLength(int index){ return m_Length[index]; };

	/// <summary>
	/// OBBの向きベクトルの大きさを３次元ベクトルとして取得
	/// </summary>
	/// <returns></returns>
	D3DXVECTOR3 GetScale(){ return {m_Length[0], m_Length[1], m_Length[2]}; }

	/// <summary>
	/// 衝突判定処理
	/// </summary>
	/// <returns></returns>
	float GetLengthOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3);
	
	/// <summary>
	/// targetとの衝突判定
	/// </summary>
	/// <param name="target"></param>
	/// <returns></returns>
	bool OBBCheckCollision(OBBCollider* target);

	/// <summary>
	/// OBBのうち最も小さい点の算出
	/// </summary>
	/// <returns></returns>
	D3DXVECTOR3 GetMinPoint();

	/// <summary>
	/// OBBのうち最も大きい点の算出
	/// </summary>
	/// <returns></returns>
	D3DXVECTOR3 GetMaxPoint();

	/// <summary>
	/// ボリュームボックスのコピーインスタンス取得
	/// </summary>
	/// <returns></returns>
	VolumeBox GetVBox(){ return m_vBox; }

};
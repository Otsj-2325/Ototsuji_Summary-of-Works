#pragma once

#include "gameObject.h"

class AnimationModel;
class Shadow;

enum DIRECTION{
	NORTH = 0,
	SOUTH,
	WEST,
	EAST
};

class Player : public GameObject // åpè≥
{
private:
	AnimationModel* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	float moveSpeed = 0.02f;
	bool isRun = false;
	D3DXVECTOR3 velocity = {0.0f, 0.0f, 0.0f};
	float groundHeight = 0.0f;
	float gravity = 0.15f;
	bool isJump = false;
	float jumpPower = 0.3f;
	float jmpCnt = 0;

	Shadow* m_Shadow{};

	bool m_IsWalk = false;
	float m_Time = 0;
	float m_BlendRate = 0.0f;
	float m_BlendAdd = 0.1f;
	std::string m_AnimationName;
	std::string m_NextAnimationName;
	void MoveQuat();
	void Jump();


public:
	Player();
	Player(Player& copy);
	~Player();
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ShowProperty() override;

	void SetIsControl(bool flag){ m_IsWalk = flag; }
	bool GetIsControl(){ return m_IsWalk; }
	D3DXVECTOR3 GetForward() override
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;

	}

	bool CollisionInit() override;
	bool CollisionUpdate() override;

};
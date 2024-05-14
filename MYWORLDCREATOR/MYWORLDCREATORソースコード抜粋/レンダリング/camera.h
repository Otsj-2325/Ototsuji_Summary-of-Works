#pragma once

#include "gameObject.h"

class Camera : public GameObject
{
protected:
	D3DXVECTOR3 m_Target{};
	D3DXMATRIX	m_ViewMatrix{};
	
	int cameraMode = -1;
	std::vector<GameObject*> cameraTargetClass{};
	std::vector<GameObject*> targetList{};
	int targetNum = 0;
	float baseRot = 0.0f;
	float acRot = 0.0f;

public:
	virtual void Init();
	virtual void Update();
	virtual void Draw();
	void SetTarget(D3DXVECTOR3 t){ m_Target = t; }
	D3DXVECTOR3 getTarget(){ return m_Target; }
};
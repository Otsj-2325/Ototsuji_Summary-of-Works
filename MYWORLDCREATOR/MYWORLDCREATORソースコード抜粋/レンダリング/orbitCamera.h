#pragma once

#include "camera.h"

class OrbitCamera : public Camera
{
	private:
	D3DXMATRIX m_CalcView;
	GameObject* m_OwnerObject;
	D3DXVECTOR3 m_OwnerDist;
	float m_PitchSpeed = 1.0f;
	float m_YawSpeed = 1.0f;
	float m_deltaYaw = 0.0f;
	float m_deltaPitch = 0.0f;

	public:
	OrbitCamera();
	OrbitCamera(OrbitCamera& copy);
	~OrbitCamera();

	void Init() override;
	void Update() override;
	void Draw() override;
	void ShowProperty() override;


};
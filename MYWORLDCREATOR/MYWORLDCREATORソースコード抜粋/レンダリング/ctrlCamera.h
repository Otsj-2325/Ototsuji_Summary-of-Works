#pragma once

#include "camera.h"

class CtrlCamera : public Camera
{
private:
	float m_XZRotation;
	float m_YZRotation;
	POINT m_OldMouseInput;
	bool m_Yawlerp = false;
	float m_CrntYaw;
	float m_NextYaw;
	float m_YawAlpha;
	D3DXVECTOR3 m_Offset;
	float m_SpeedA;
	bool m_Freeze = false;

	void MoveWorld();


public:
	CtrlCamera();
	CtrlCamera(CtrlCamera& copy);
	~CtrlCamera();
	void Init() override;
	void Update() override;
	void Draw() override;
	void ShowProperty() override;
	void SetFreeze(bool b) { m_Freeze = b; }

};

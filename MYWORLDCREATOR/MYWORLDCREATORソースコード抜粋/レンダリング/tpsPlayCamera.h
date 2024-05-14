#pragma once

#include "camera.h"

class TpsPlayCamera : public Camera
{
private:
	D3DXVECTOR3 m_OwnerDist;


public:
	TpsPlayCamera();
	TpsPlayCamera(TpsPlayCamera& copy);
	~TpsPlayCamera();
	void Init() override;
	void Update() override;
	void Draw() override;
	void ShowProperty() override;

};

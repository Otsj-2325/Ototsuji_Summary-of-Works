#pragma once

#include "gameObject_Model3D.h"

class Skydorm : public Model3D // �p��
{
private:		

public:
	Skydorm();
	Skydorm(Skydorm& copy);
	~Skydorm();
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ShowProperty() override;
	
};
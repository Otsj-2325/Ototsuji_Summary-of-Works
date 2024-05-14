#pragma once

#include "gameObject_Model3D.h"

class Skydorm : public Model3D // åpè≥
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
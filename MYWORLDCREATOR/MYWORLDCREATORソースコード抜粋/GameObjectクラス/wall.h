#pragma once

#include "gameObject_Model3D.h"

class Wall :public Model3D
{
private:

public:
	Wall();
	Wall(Wall& obj);
	~Wall();
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ShowProperty() override;

};
#pragma once

#include "gameObject_Model3D.h"

class Table :public Model3D
{
private:

public:
	Table();
	Table(Table& obj);
	~Table();
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ShowProperty() override;

	bool CollisionInit() override;
	bool CollisionUpdate() override;

};
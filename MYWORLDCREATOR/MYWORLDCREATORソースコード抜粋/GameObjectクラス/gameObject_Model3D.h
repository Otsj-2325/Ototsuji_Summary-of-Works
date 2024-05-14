#pragma once

#include "model.h"
#include "gameObject.h"

class GameObject;

class Model3D : public GameObject
{
	protected:
	Model* mp_Model;
	public:
	Model3D();
	Model3D(Model3D& copy);
	~Model3D();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ShowProperty();
	void LoadModel(std::string Path);

};
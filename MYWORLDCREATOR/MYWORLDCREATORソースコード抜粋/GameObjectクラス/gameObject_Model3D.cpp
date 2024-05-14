#include "main.h"
#include "renderer.h"
#include "modelrender.h"
#include "manager.h"
#include "scene.h"
#include "obbCollision.h"
#include "collisionList.h"
#include "gameObject_Model3D.h"

Model3D::Model3D()
{
	Model3D::Init();
}

Model3D::Model3D(Model3D& copy)
{
	Model3D::Init();
}

Model3D::~Model3D()
{	
	Model3D::Uninit();
}

void Model3D::Init()
{
	m_ObjectName = "NewModel3D";

	mp_Model = new Model();

	auto mrend = AddComponent<ModelRender>();
	mrend->SetModel(mp_Model);

}

void Model3D::Uninit()
{
	mp_Model->Unload();
	GameObject::Uninit();
}

void Model3D::Update()
{
	D3DXQUATERNION quat;
	D3DXVECTOR3 vec = {0.0f, 1.0f, 0.0f};
	D3DXQuaternionRotationAxis(&quat, &vec, 0.1f);
	m_Quaternion *= quat;
}

void Model3D::Draw()
{
	GameObject::Draw();
}

void Model3D::ShowProperty()
{
	if (!isShowProperty) return;

	GameObject::ShowProperty();
}

void Model3D::LoadModel(std::string Path)
{
	if (mp_Model)
	{
		mp_Model = (Model*)ResourceManager::GetResource(Path.c_str());
		auto mrend = GetComponent<ModelRender>();
		mrend->SetModel(mp_Model);
	}
}

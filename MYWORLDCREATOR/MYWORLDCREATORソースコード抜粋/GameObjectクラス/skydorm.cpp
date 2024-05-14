#include "main.h"
#include "renderer.h"
#include "skydorm.h"
#include "modelrender.h"

Skydorm::Skydorm()
{
	m_Component.clear();
	Skydorm::Init();
}

Skydorm::Skydorm(Skydorm& copy)
{
	m_Component.clear();
	Skydorm::Init();
}

Skydorm::~Skydorm()
{
	Skydorm::Uninit();
}

void Skydorm::Init()
{
	m_ObjectName = "Skydorm";

	mp_Model = (Model*)ResourceManager::GetResource("asset\\model\\skydorm.obj");

	auto mrend = AddComponent<ModelRender>();
	mrend->SetModel(mp_Model);

	SetScale({150.0f, 150.0f, 150.0f});
}

void Skydorm::Uninit()
{
	mp_Model->Unload();
	delete mp_Model;
}

void Skydorm::Update()
{
	auto rot = GetRotation();
	rot.y += 0.0001f;
	SetRotation(rot);
}

void Skydorm::Draw()
{
	GameObject::Draw();

}

void Skydorm::ShowProperty()
{
	if (!isShowProperty) return;

	GameObject::ShowProperty();
}

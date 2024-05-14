#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "Wall.h"
#include "obbCollision.h"
#include "collisionList.h"
#include "ModelRender.h"

Wall::Wall()
{
	m_Component.clear();
	Wall::Init();
}

Wall::Wall(Wall& obj)
{
	m_Component.clear();
	Wall::Init();
}

Wall::~Wall()
{
	Wall::Uninit();
}

void Wall::Init()
{
	m_ObjectName = "Wall";

	mp_Model = (Model*)ResourceManager::GetResource("asset\\model\\cube.obj");

	auto mrend = AddComponent<ModelRender>();
	mrend->SetModel(mp_Model);

	SetScale({1.0f, 2.0f, 0.25f});

	auto pOBB = AddComponent<OBBCollider>();
	OBBConfigData ocd;
	ocd.OBBColliderPosition = m_Position;
	ocd.OBBColliderPosition.y += 0.4f;
	ocd.OBBRotaion = m_Rotation;
	ocd.OBBScale = GetScale();
	ocd.OBBForwardVector = GetForward();
	ocd.OBBRightVector = GetRight();
	ocd.OBBUpVector = GetUp();
	pOBB->Init(&ocd, this);

	auto colList = Manager::GetScene()->GetColList();
	colList->AddCollision(pOBB);

}

void Wall::Uninit()
{
	mp_Model->Unload();
	GameObject::Uninit();
}

void Wall::Update()
{
	CalcWorldMatrix();

	GameObject::Update();

}

void Wall::Draw()
{
	GameObject::Draw();

}

void Wall::ShowProperty()
{
	if (!isShowProperty) return;

	GameObject::ShowProperty();
}

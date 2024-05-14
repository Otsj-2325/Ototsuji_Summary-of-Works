#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "table.h"
#include "obbCollision.h"
#include "ModelRender.h"

Table::Table()
{
	m_Component.clear();
	Table::Init();
}

Table::Table(Table& obj)
{
	m_Component.clear();
	Table::Init();
}

Table::~Table()
{
	Table::Uninit();
}

void Table::Init()
{
	m_ObjectName = "Table";

	mp_Model =(Model*) ResourceManager::GetResource("asset\\model\\table\\wooden_table.obj");

	auto mrend = AddComponent<ModelRender>();
	mrend->SetModel(mp_Model);	

	CollisionInit();
}

void Table::Uninit()
{
	mp_Model->Unload();
	GameObject::Uninit();
}

void Table::Update()
{	
	CalcWorldMatrix();

	

	GameObject::Update();
		
}

void Table::Draw()
{	
	GameObject::Draw();

}

void Table::ShowProperty()
{
	if (!isShowProperty) return;

	GameObject::ShowProperty();
}

bool Table::CollisionInit()
{
	{
		auto pOBB = AddComponent<OBBCollider>();
		OBBConfigData ocd;
		ocd.OBBColliderPosition = m_Position;
		ocd.OBBColliderPosition.y += 0.4f;
		ocd.OBBRotaion = m_Rotation;
		ocd.OBBScale = { 0.6f, 0.4f, 0.4f };
		ocd.OBBForwardVector = GetForward();
		ocd.OBBRightVector = GetRight();
		ocd.OBBUpVector = GetUp();
		pOBB->Init(&ocd, this);

		//collisionList‚É“o˜^
		if (Manager::GetScene()->SetCollision(pOBB))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}

bool Table::CollisionUpdate()
{
	auto pOBB = GetComponent<OBBCollider>();
	OBBConfigData ocd;
	ocd.OBBColliderPosition = m_Position;
	ocd.OBBColliderPosition.y += 0.4f;
	ocd.OBBRotaion = m_Rotation;
	ocd.OBBScale = { 0.6f, 0.4f, 0.4f };
	ocd.OBBForwardVector = GetForward();
	ocd.OBBRightVector = GetRight();
	ocd.OBBUpVector = GetUp();
	pOBB->Update(&ocd);
	return true;
}

#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "hierarchy.h"
#include "input.h"
#include "post.h"
#include "obbCollision.h"

//Scene
#include "projectTest.h"
#include "playMode.h"

//GaneObject
#include "light.h"
#include "camera.h"
#include "ctrlCamera.h"
#include "orbitCamera.h"
#include "field.h"
#include "skydorm.h"
#include "player.h"
#include "table.h"
#include "volumeBox.h"
#include "gameObject_Model3D.h"

void PlayMode::Init()
{
	Scene::Init();

	Hierarchy* hierarchy = Manager::GetHierarchy();

	//‚W•ª–ØÕ“ËƒŠƒXƒg‚Ì¶¬
	Scene::m_pTSPMng = new TSPManager();

	if (hierarchy)
	{
		m_HierarchyList = hierarchy;
	}
	else
	{
		auto ccam = m_HierarchyList->AddGameObjectHierarchy<CtrlCamera>("0CtrlCamera");
		auto ocam = m_HierarchyList->AddGameObjectHierarchy<OrbitCamera>("0OrbitCamera");
		m_HierarchyList->AddGameObjectHierarchy<Light>("Light");
		m_HierarchyList->AddGameObjectHierarchy<Skydorm>("Skydorm");
		m_HierarchyList->AddGameObjectHierarchy<Field>("Field");
		auto table = m_HierarchyList->AddGameObjectHierarchy<Table>("Table");
		auto player1 = m_HierarchyList->AddGameObjectHierarchy<Player>("Player1");

		m_HierarchyList->InitHierarchy();


		ccam->SetTarget({ 0.0f, 0.0f, 0.0f });
		ccam->SetPosition({ 0.0f, 2.0f, -10.0f });


		table->SetPosition({ 3.0f, 0.0f, 0.0f });
		player1->SetPosition({ 5.0f, 0.0f , 0.0f });
		player1->SetScale({ 0.01f, 0.01f , 0.01f });

		Manager::GetScene()->GetGameObject<CtrlCamera>()->IsActive(false);
		Manager::GetScene()->GetGameObject<OrbitCamera>()->IsActive(true);
		Manager::SetHierarchy(m_HierarchyList);
	}

	auto pPlayer = m_HierarchyList->GetGameObjectHierarchy<Player>();
	if (pPlayer)
	{
		pPlayer->SetIsControl(true);

	}

	auto hMap = m_HierarchyList->GetList();
	for (auto obj : hMap)
	{
		obj.second->CollisionInit();
	}

}

void PlayMode::Uninit()
{
	delete m_pTSPMng;
	
}

void PlayMode::Update()
{
	Scene::Update();
	ShowProperty();

	if (Input::GetKeyTrigger('L')){
		Manager::GetScene()->GetGameObject<CtrlCamera>()->IsActive(true);
		Manager::GetScene()->GetGameObject<OrbitCamera>()->IsActive(false);
		auto pPlayer = Manager::GetHierarchy()->GetGameObjectHierarchy<Player>();
		if(pPlayer)
		{
			pPlayer->SetIsControl(false);
		}
		Manager::SetScene<ProjectTest>();
		Manager::SetHierarchy(m_HierarchyList);

		return;
	}

	auto hMap = m_HierarchyList->GetList();
	for (auto obj : hMap)
	{
		obj.second->CollisionUpdate();
	}

	m_pTSPMng->Update();
}

void PlayMode::Draw()
{
	Renderer::SetViewportDefault();

	Renderer::BeginDR();
	Scene::Draw();

	Renderer::Begin();
	Manager::GetPost()->Draw(0);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

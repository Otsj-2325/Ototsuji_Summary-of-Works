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
#include "CtrlCamera.h"
#include "orbitCamera.h"
#include "field.h"
#include "skydorm.h"
#include "player.h"
#include "table.h"
#include "Town.h"
#include "volumeBox.h"
#include "gameObject_Model3D.h"
#include "Polygon3D.h"
#include "waveface.h"

Field* pField = {};

void ProjectTest::Init()
{
	Scene::Init();

	Hierarchy* hierarchy = Manager::GetHierarchy();
	if(hierarchy)
	{
		m_HierarchyList = hierarchy;
	}
	else{
		//m_HierarchyList->AddGameObjectHierarchy<Light>("Light");

		auto ccam = m_HierarchyList->AddGameObjectHierarchy<CtrlCamera>("0CtrlCamera");
		ccam->SetTarget({ 0.0f, 0.0f, 0.0f });
		ccam->SetPosition({ 0.0f, 1.0f, -3.0f });
		auto ocam = m_HierarchyList->AddGameObjectHierarchy<OrbitCamera>("0OrbitCamera");
		ocam->IsActive(false);

		m_HierarchyList->AddGameObjectHierarchy<Skydorm>("Skydorm");
		m_HierarchyList->AddGameObjectHierarchy<Field>("Field");

		auto table = m_HierarchyList->AddGameObjectHierarchy<Table>("Table");
		table->SetPosition({ 0.0f, 0.0f, -2.0f });

		auto player1 = m_HierarchyList->AddGameObjectHierarchy<Player>("Player");
		player1->SetPosition({ 0.0f, 0.0f , -1.0f });
		player1->SetScale({0.01f, 0.01f , 0.01f});
		
		m_HierarchyList->InitHierarchy();

	}
	m_Assetlist->InitAssetList();	

	pField = new Field();
}

void ProjectTest::Uninit()
{
}

void ProjectTest::Update()
{
	Scene::Update();
	SelectCursor::Update();
	ShowIMGUI();

	ShowProperty();

	MOUSESTATE ms = GetMouseState();
	if(ms.LBUTTON == MS_TRIGGER_DOWN && !GetMouseSkip()){
		GameObject* pAsset = m_Assetlist->GetSelectAsset();
		if (pAsset != nullptr)
		{
			int n = m_HierarchyList->GetLength();
			auto obj = m_HierarchyList->AddGameObjectHierarchy("NewObject" + std::to_string(n), pAsset);
		}
	}

	if(Input::GetKeyTrigger('L')){
		Manager::GetScene()->GetGameObject<CtrlCamera>()->IsActive(false);
		Manager::GetScene()->GetGameObject<OrbitCamera>()->IsActive(true);
		Manager::SetScene<PlayMode>();
		Manager::SetHierarchy(m_HierarchyList);
	}
}

void ProjectTest::Draw()
{
	//GBufferへの書き込み（デファードレンダリング）
	Renderer::BeginDR();
	Scene::Draw();

	//デファードライティング
	Renderer::BeginDL();
	Manager::GetPost()->Draw(0);

	Renderer::BeginFR();
	pField->Draw();

	Renderer::Begin();
	Manager::GetPost()->Draw(1);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void ProjectTest::ShowIMGUI()
{
	selectPos = SelectCursor::GetCalcPos();
	RAY ray = SelectCursor::GetRay();

	ImGuiIO io = ImGui::GetIO();

	ImGui::Begin("Game Property");

	std::string isActive = "false";
	if (m_CursorClip)
	{
		isActive = "true";
	}

	ImGui::Text("Cursor clip : %s", isActive.c_str());
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::Text("SelectPos : X %.2f, Y %.2f, Z %.2f", selectPos.x, selectPos.y, selectPos.z);
	ImGui::Text("RayDirection : X %.2f, Y %.2f, Z %.2f", ray.Direction.x, ray.Direction.y, ray.Direction.z);

	ImGui::Checkbox("Toggle IMGUIDemo", &m_ShowDemo);

	if (ImGui::Button("Save hierarchy"))
	{
		SaveScene::SaveHierarchy(*m_HierarchyList);
	}

	ImGui::RadioButton("Default Rendering", (int*)& m_RenderMode, (int)RENDERMODE::DefaultRender);
	ImGui::RadioButton("Albedo Rendering", (int*)&m_RenderMode, (int)RENDERMODE::AlbedRender);
	ImGui::RadioButton("Normal Rendering", (int*)&m_RenderMode, (int)RENDERMODE::NormalRender);
	ImGui::RadioButton("WPosition Rendering", (int*)&m_RenderMode, (int)RENDERMODE::WPositionRender);

	if (m_ShowDemo){
		ImGui::ShowDemoWindow(&m_ShowDemo);
	}
	ImGui::End();
}

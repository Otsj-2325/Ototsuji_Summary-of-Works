#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "audio.h"
#include "post.h"
#include "hierarchy.h"

//Scene
#include "title.h"
#include "projectTest.h"
#include "playMode.h"

Scene* Manager::m_Scene{}; // 静的メンバ変数は再宣言が必要
Scene* Manager::m_NextScene{};
Post* Manager::m_post{};
Hierarchy Manager::m_TempHierarchy{};
bool Manager::m_HaveHierarchy = false;
bool Manager::m_HaveColList = false;

void Manager::Init()
{
	Renderer::Init();
	Audio::InitMaster();

#ifdef _DEBUG
	m_Scene = new ProjectTest();
	m_Scene->Init();

#else
	m_Scene = new Title();
	m_Scene->Init();

#endif
 
	m_post = new Post();
	m_post->Init();

	Input::Init();
}

void Manager::Uninit()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Input::Uninit();

	m_post->Uninit();
	delete m_post;

	m_Scene->Uninit();
	delete m_Scene;

	Audio::UninitMaster();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();
	
	m_post->Update();

	if (m_NextScene){
		if (m_Scene){
			m_Scene->Uninit();
			delete m_Scene;
		}

		m_Scene = m_NextScene;
		m_Scene->Init();
		m_post->Init();
		m_NextScene = nullptr;
	}

	m_Scene->Update();
}

void Manager::Draw()
{
	ImGui::Render();

	m_Scene->Draw();

	Renderer::End();
}


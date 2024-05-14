#include "main.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "title.h"
#include "projectTest.h"
#include "gameObject.h"

#include "CtrlCamera.h"
#include "skydorm.h"
#include "Polygon3D.h"
#include "waveface.h"
#include "splash.h"

#include "sprite.h"

void Title::Init()
{
	Scene::Init();

	CtrlCamera* cam = m_HierarchyList->AddGameObjectHierarchy<CtrlCamera>("CtrlCamera");
	
	
	m_HierarchyList->AddGameObjectHierarchy<Splash>("Splash");
	auto logo = m_HierarchyList->AddGameObjectHierarchy<Polygon3D>("Logo");
	m_HierarchyList->InitHierarchy();

	cam->SetTarget({0.0f, 8.0f, 3.0f});
	cam->SetFreeze(true);
	
	logo->SetPosition({0.0f, 90.0f, 200.0f});
	logo->SetScale({160.0f, 90.0f, 1.0f});

	selectPos = { 0.0f, 0.0f, 0.0f };

	m_RenderMode = RENDERMODE::AlbedRender;
}

void Title::Uninit()
{
	Scene::Uninit();

}

void Title::Update()
{
	Scene::Update();

	if(Input::GetKeyPress('L'))
	{
		Manager::SetScene<ProjectTest>();
	}

	Scene::ShowProperty();

}

void Title::Draw()
{
	LIGHT light {};
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXVECTOR3 lightPos = { 0.0f, 0.0f, 0.0f };
	lightPos.y += 20.0f;
	lightPos.z += 20.0f;
	D3DXVECTOR3 lightTarget = {0.0f, 0.0f, 0.0f};
	lightTarget.z -= 10.0f;
	D3DXVECTOR3 lightUp = { 0.0f, 1.0f ,0.0f };
	D3DXMatrixLookAtLH(&light.View, &lightPos, &lightTarget, &lightUp);
	D3DXMatrixPerspectiveFovLH(&light.Projection, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 400.0f);

	Renderer::SetLight(light);


	Renderer::SetViewportDefault();
		
	Renderer::BeginDR();
	Scene::Draw();

	Renderer::Begin();
	Manager::GetPost()->Draw(0);

}

#include "main.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "tpsPlayCamera.h"

TpsPlayCamera::TpsPlayCamera()
{
	m_Component.clear();
	Init();
}

TpsPlayCamera::TpsPlayCamera(TpsPlayCamera& copy)
{
	m_Component.clear();
	Init();
}

TpsPlayCamera::~TpsPlayCamera()
{
	Uninit();
}

void TpsPlayCamera::Init()
{
	m_ObjectName = "0PlayCamera";

	m_Position = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_OwnerDist = { 0.5f, 2.0f, -2.5f };

	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 ViewPos = m_Position;
	D3DXMatrixLookAtLH(&m_ViewMatrix, &ViewPos, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);

}

void TpsPlayCamera::Update()
{
	auto pPlayer = Manager::GetScene()->GetGameObject<Player>();
	auto playerPos = pPlayer->GetPosition();
	m_Position = playerPos + m_OwnerDist;
	m_Target = playerPos;
	m_Target.x += 0.5f;
	m_Target.y += 1.0f;

	ImGuiIO io = ImGui::GetIO();
	ImVec2 delta = io.MouseDelta;


	
}

void TpsPlayCamera::Draw()
{
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 ViewPos = m_Position;
	D3DXMatrixLookAtLH(&m_ViewMatrix, &ViewPos, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

void TpsPlayCamera::ShowProperty()
{
	if (!isShowProperty) return;

	if (ImGui::TreeNode(m_ObjectName.c_str())){
		GameObject::ShowProperty();

		float pos[3] = { m_OwnerDist.x, m_OwnerDist.y, m_OwnerDist.z };

		ImGui::DragFloat3("to Player offset", pos, 0.01f, -9999.0f, 9999.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

		m_OwnerDist.x = pos[0];
		m_OwnerDist.y = pos[1];
		m_OwnerDist.z = pos[2];

		ImGui::TreePop();
	}
}

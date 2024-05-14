#include "main.h"
#include "renderer.h"
#include "child.h"
#include "manager.h"
#include "scene.h"
#include "player.h"

void Child::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_model = new Model();
	m_model->Load("asset\\model\\torus.obj");
}

void Child::Uninit()
{
	if (m_model) m_model->Unload();

	if (m_VertexLayout) m_VertexLayout->Release();
	if (m_VertexShader) m_VertexShader->Release();
	if (m_PixelShader) m_PixelShader->Release();

	GameObject::Uninit();
}

void Child::Update()
{
	m_Position.x = cos(rot.x) * 200.0f;
	m_Position.y = 100.0f;
	m_Position.z = sin(rot.y) * 200.0f;

	rot.x += 0.1f;
	rot.y += 0.1f;
}

void Child::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans * m_Parent->ObjectWorldMatrix();

	Renderer::SetWorldMatrix(&world);

	m_model->Draw();
}

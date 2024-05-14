#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "manager.h"
#include "scene.h"
#include "input.h"
#include "player.h"

Scene* pScene;
Player* pPlayer;

void Camera::Init()
{	
	m_Position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
}

void Camera::Update()
{
	
}

void Camera::Draw()
{
	// ビューマトリクス設定 カメラをどこに置いて、どの向きにするのか
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // 基本yが1
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクションマトリクス設定 ズームイン、ズームアウト→レンズの設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f); // 1.0f(手前)～1000.0f(奥)

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

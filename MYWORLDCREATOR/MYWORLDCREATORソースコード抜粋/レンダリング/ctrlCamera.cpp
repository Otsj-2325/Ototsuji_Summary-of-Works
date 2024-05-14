#include "main.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "CtrlCamera.h"
#include "player.h"
#include "field.h"

void CtrlCamera::MoveWorld()
{
	MOUSESTATE ms;
	ms = GetMouseState();

	if (ms.RBUTTON == MS_PREASS_DOWN && !GetMouseSkip()){
		bool isInput = false;

		if (Input::GetKeyPress('W'))
		{
			m_Position += GetForward() * 0.1f * m_SpeedA;
			isInput = true;
		}
		if (Input::GetKeyPress('S'))
		{
			m_Position -= GetForward() * 0.1f * m_SpeedA;
			isInput = true;
		}
		if (Input::GetKeyPress('D'))
		{
			m_Position += GetRight() * 0.1f * m_SpeedA;
			isInput = true;
		}
		if (Input::GetKeyPress('A'))
		{
			m_Position -= GetRight() * 0.1f * m_SpeedA;
			isInput = true;
		}
		if (Input::GetKeyPress('Q'))
		{
			m_Position += GetUp() * 0.1f * m_SpeedA;
			isInput = true;
		}
		if (Input::GetKeyPress('E'))
		{
			m_Position -= GetUp() * 0.1f * m_SpeedA;
			isInput = true;
		}

		if (isInput)
		{
			m_SpeedA += 0.02f;
			if (m_SpeedA > 1.0f)
			{
				m_SpeedA = 1.0f;
			}
		}
		else
		{
			m_SpeedA = 0.0f;
			
		}

		ImGuiIO io = ImGui::GetIO();
		ImVec2 delta = io.MouseDelta;

		float line = 100.0f;
		if (delta.x > line || delta.x < -line)
		{
			delta.x = 0.0f;
		}
		if (delta.y > line || delta.y < -line)
		{
			delta.y = 0.0f;
		}

		m_Rotation.y += delta.x * 0.001f;
		m_Rotation.x += delta.y * 0.001f;

		
	}
	else if(ms.RBUTTON != MS_PREASS_DOWN)
	{
		m_SpeedA = 0.0f;
	}

	m_Target = m_Position + GetForward();

}

CtrlCamera::CtrlCamera()
{
	m_Component.clear();
	CtrlCamera::Init();
}

CtrlCamera::CtrlCamera(CtrlCamera& copy)
{
	m_Component.clear();
	CtrlCamera::Init();
}

CtrlCamera::~CtrlCamera()
{
	CtrlCamera::Uninit();
}

void CtrlCamera::Init()
{
	m_ObjectName = "CtrlCamera";

	m_Position = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_XZRotation = -3.14f / 2.0f;
	m_YZRotation = 3.14f / 4.0f;
	GetCursorPos(&m_OldMouseInput);

	m_Offset = {0.0f, 0.0f, 0.0f};

	m_SpeedA = 0.0f;

	// ビューマトリクス設定 カメラをどこに置いて、どの向きにするのか
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // 基本yが1
	D3DXVECTOR3 ViewPos = m_Position;
	D3DXMatrixLookAtLH(&m_ViewMatrix, &ViewPos, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクションマトリクス設定 ズームイン、ズームアウト→レンズの設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f); // 1.0f(手前)～1000.0f(奥)

	Renderer::SetProjectionMatrix(&projectionMatrix);

}

void CtrlCamera::Update()
{
	if (m_Freeze)
	{
		return;
	}

	MoveWorld();

}

void CtrlCamera::Draw()
{
	// ビューマトリクス設定 カメラをどこに置いて、どの向きにするのか
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // 基本yが1
	D3DXVECTOR3 ViewPos = m_Position;

	D3DXMatrixLookAtLH(&m_ViewMatrix, &ViewPos, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクションマトリクス設定 ズームイン、ズームアウト→レンズの設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
								(float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f); // 1.0f(手前)～1000.0f(奥)

	Renderer::SetProjectionMatrix(&projectionMatrix);

}

void CtrlCamera::ShowProperty()
{
	if(!isShowProperty) return;

	GameObject::ShowProperty();


	ImGui::Text("Target : %g, %g, %g", m_Target.x, m_Target.y, m_Target.z);
	ImGui::Text("XZRotation : %g", m_Rotation.x);
	ImGui::Text("YZRotation : %g", m_Rotation.y);
	ImGui::Text("SpeedAlpha : %g", m_SpeedA);

}

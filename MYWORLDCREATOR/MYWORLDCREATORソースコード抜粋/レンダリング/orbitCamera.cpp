#include "main.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "orbitCamera.h"

OrbitCamera::OrbitCamera()
{
	m_Component.clear();
	Init();
}

OrbitCamera::OrbitCamera(OrbitCamera& copy)
{
	m_Component.clear();
	Init();
}

OrbitCamera::~OrbitCamera()
{
	Uninit();
}

void OrbitCamera::Init()
{
	auto getOwner = Manager::GetScene()->GetGameObject<Player>();
	if (getOwner)
	{
		m_OwnerObject = getOwner;
	}

	m_OwnerDist = {0.0f, 0.0f, 3.0f};

}

void OrbitCamera::Update()
{
	MOUSESTATE ms;
	ms = GetMouseState();

	if (ms.RBUTTON == MS_PREASS_DOWN && !GetMouseSkip()){
		bool isInput = false;

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

		m_deltaYaw += delta.x * 0.001f;
		m_deltaPitch += delta.y * 0.001f;

		if (m_deltaYaw > 6.27f)
		{
			m_deltaYaw = 0.01f;
		}
		if (m_deltaYaw < 0.01f)
		{
			m_deltaYaw = 6.27f;
		}

		if (m_deltaPitch > 0.0f)
		{
			m_deltaPitch = 0.0f;
		}
		else if (m_deltaPitch < -1.57f)
		{
			m_deltaPitch = -1.57f;
		}

	}

	if(m_OwnerObject)
	{
		//カメラの向きを取得
		D3DXVECTOR3 WorldUp = { 0.0f, 1.0f, 0.0f };
		D3DXVECTOR3 WorldForward = {0.0f, 0.0f, 1.0f};

		//Yaw計算
		//変換する3次元ベクトルを４次元のクォータニオンに拡張
		D3DXQUATERNION quatOffset = { m_OwnerDist.x, m_OwnerDist.y, m_OwnerDist.z, 0.0f };
		D3DXVECTOR3 up = GetUp();
		D3DXQUATERNION quatUp = {up.x, up.y, up.z, 0.0f};
	
		//Y軸を軸にして回転クォータニオンを作る
		D3DXQUATERNION quatYaw;
		D3DXQuaternionRotationAxis(&quatYaw, &WorldUp, m_YawSpeed * m_deltaYaw);
		//共役のクォータニオン
		D3DXQUATERNION invQuatYaw;
		D3DXQuaternionConjugate(&invQuatYaw, &quatYaw);
	
		//quatOffsetを回転クォータニオンで変換し向きを計算
		D3DXVECTOR3 calcOffset;
		D3DXQuaternionMultiply(&quatOffset, &quatYaw, &quatOffset);
		D3DXQuaternionMultiply(&quatOffset, &quatOffset, &invQuatYaw);
		calcOffset = {quatOffset.x, quatOffset.y, quatOffset.z};
		//quatupを回転クォータニオンで変換し向きを計算
		D3DXVECTOR3 calcUp;
		D3DXQuaternionMultiply(&quatUp, &quatYaw, &quatUp);//q * qv
		D3DXQuaternionMultiply(&quatUp, &quatUp, &invQuatYaw);//(q * qv) * q'
		calcUp = {quatUp.x, quatUp.y, quatUp.z};

		D3DXVECTOR3 forward = calcOffset;
		D3DXVec3Normalize(&forward, &forward);
		D3DXVECTOR3 right;
		D3DXVec3Cross(&right, &up, &forward);

		//Y軸を軸にして回転クォータニオンを作る
		D3DXQUATERNION quatPitch;
		D3DXQuaternionRotationAxis(&quatPitch, &right, m_PitchSpeed * m_deltaPitch);
		//共役のクォータニオン
		D3DXQUATERNION invQuatPitch;
		D3DXQuaternionConjugate(&invQuatPitch, &quatPitch);

		//quatOffsetを回転クォータニオンで変換し向きを計算
		D3DXQuaternionMultiply(&quatOffset, &quatPitch, &quatOffset);
		D3DXQuaternionMultiply(&quatOffset, &quatOffset, &invQuatPitch);
		calcOffset = { quatOffset.x, quatOffset.y, quatOffset.z };
		//upを回転クォータニオンで変換し向きを計算
		D3DXQuaternionMultiply(&quatUp, &quatPitch, &quatUp);
		D3DXQuaternionMultiply(&quatUp, &quatUp, &invQuatPitch);
		calcUp = { quatUp.x, quatUp.y, quatUp.z };

		D3DXVECTOR3 target = m_OwnerObject->GetPosition() + m_OwnerObject->GetForward() * 0.1f;
		target.y += 0.5f;
		D3DXVECTOR3 camPos = target - calcOffset;
		SetPosition(camPos);
		D3DXMatrixLookAtLH(&m_CalcView, &camPos, &target, &calcUp);
		Renderer::SetViewMatrix(&m_CalcView);
	}
	else
	{
		auto getOwner = Manager::GetScene()->GetGameObject<Player>();
		if (getOwner)
		{
			m_OwnerObject = getOwner;
		}
	}
}

void OrbitCamera::Draw()
{
	Renderer::SetViewMatrix(&m_CalcView);

	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

void OrbitCamera::ShowProperty()
{
	if (!isShowProperty) return;

	if (ImGui::TreeNode(m_ObjectName.c_str())){
		GameObject::ShowProperty();

		float deltaY = m_deltaYaw;
		float deltaP = m_deltaPitch;
		ImGui::DragFloat("Yaw", &deltaY, 0.01f, 0.0f, 6.28f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::DragFloat("Pitch", &deltaP, 0.01f, -1.57f, 0.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

		m_deltaYaw = deltaY;
		if (m_deltaYaw > 6.27f)
		{
			m_deltaYaw = 0.01f;
		}
		if(m_deltaYaw < 0.01f)
		{
			m_deltaYaw = 6.27f;
		}

		m_deltaPitch = deltaP;

		auto forward = GetForward();
		ImGui::Text("Forward : %g, %g, %g", forward.x, forward.y, forward.z);

		ImGui::TreePop();
	}

}

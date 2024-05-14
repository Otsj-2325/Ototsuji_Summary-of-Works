#include <vector>

#include "main.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "resourceManager.h"

#include "shadow.h"
#include "animationModel.h"
#include "aabbCollision.h"
#include "obbCollision.h"

#include "CtrlCamera.h"
#include "orbitCamera.h"
#include "table.h"
#include "Town.h"

Player::Player()
{
	m_Component.clear();
	Player::Init();
}

Player::Player(Player& copy)
{
	m_Component.clear();
	Player::Init();
}

Player::~Player()
{
	Player::Uninit();
}

void Player::Init()
{
	m_ObjectName = "Player";
	// モデルクラスのインスタンス生成
	m_Model = (AnimationModel*)ResourceManager::GetResource("asset\\model\\bot\\Ch03_nonPBR.fbx");

	m_Model->LoadAnimation("asset\\model\\bot\\Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\bot\\Walking.fbx", "Walk");
	m_Model->LoadAnimation("asset\\model\\bot\\Slow Run.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\bot\\Jumping.fbx", "Jump");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\deferredGeometryVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\deferredGeometryPS.cso");

	m_AnimationName = "Idle";
	m_NextAnimationName = "Idle";

	m_IsWalk = false;

	GameObject::Init();

	SetScale({0.01f, 0.01f, 0.01f});

}

void Player::Uninit()
{
	GameObject::Uninit();

	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Player::Update()
{
	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if(m_IsWalk){
		MoveQuat();
		Jump();
	}

	
	
	GameObject::Update();
}

void Player::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	CalcWorldMatrix();

	if (m_IsWalk)
	{

#ifndef _DEBUG
		m_Model->Update(m_AnimationName.c_str(), m_Time, m_NextAnimationName.c_str(), m_Time, m_BlendRate);
		m_Time += 0.4f;
		m_BlendRate += m_BlendAdd;
		if (m_BlendRate > 1.0f){
			m_BlendRate = 1.0f;
		}
#endif

	}

	
	m_Model->Draw();

}

void Player::MoveQuat()
{
	bool isW = Input::GetKeyPress('W');
	bool isS = Input::GetKeyPress('S');
	bool isA = Input::GetKeyPress('A');
	bool isD = Input::GetKeyPress('D');
	bool isShift = Input::GetKeyPress(VK_LSHIFT);
		
	bool isWS = (isW || isS);
	bool isAD = (isA || isD);
	bool isDoubleInput = isWS && isAD;

	float speed = moveSpeed;
	float theta = 0.0f;
	bool isDown;

	if (isW || isS || isA || isD){
		if (m_NextAnimationName != "Walk" && !isRun && !isJump){
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Walk";
			m_BlendRate = 0.0f;
		}

		//新しいForwardへの振り向き角度計算
		{
			Scene* pScene = Manager::GetScene();
			auto pCam = pScene->GetGameObject<OrbitCamera>();

			if (pCam->IsActive())
			{
				m_IsWalk = true;
			}
			else
			{
				velocity = {0.0f, 0.0f, 0.0f};
				m_IsWalk = false;
				return;
			}

			D3DXVECTOR3 campos = pCam->GetPosition();

			//新しいForwardベクトル
			D3DXVECTOR3 newforward;
			newforward = GetPosition() - campos;
			D3DXVec3Normalize(&newforward, &newforward);

			newforward.y = 0.0f;

			D3DXVECTOR3 originforward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			D3DXVec3Normalize(&originforward, &originforward);

			//回転角度の絶対値（左右がない）
			{
				// (1) originforward * NewForward
				float dot = D3DXVec3Dot(&originforward, &newforward);

				// (2) ||originforward|*||NewForward||*cos(Theta)
				float nowScalar = (originforward.x * originforward.x + originforward.z * originforward.z);
				float newScalar = (newforward.x * newforward.x + newforward.z * newforward.z);
				float dotcos = sqrtf(nowScalar) * sqrtf(newScalar);

				//Theta = (1) / (2)
				if (dot > 1.0f){
					dot = 1.0f;
				}
				if (dotcos > 1.0f){
					dotcos = 1.0f;
				}

				float val = dot / dotcos;
				if (val > 1.0f){
					val = 1.0f;
				}
				else if (val < -1.0f)
				{
					val = -1.0f;
				}

				theta = acos(val);

			}

			//左右の判定（外積のベクトルの上下）
			isDown = false;
			{
				D3DXVECTOR3 cross;
				D3DXVec3Cross(&cross, &originforward, &newforward);
				if (cross.y < 0){
					isDown = true;
				}
			}
		}

		//入力方向に合わせたteataの調整
		{
			if (isS){
				theta += 3.14f;

				if (isA){
					theta -= 1.57f;
				}
				if (isD){
					theta += 1.57f;
				}
			}
			else{
				if (isA){
					theta += 1.57f;
				}
				if (isD){
					theta -= 1.57f;
				}
			}
		}

		//振り向き球面線形補間
		{
			D3DXQUATERNION quat = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
			if (isDown){
				D3DXVECTOR3 axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				D3DXQuaternionRotationAxis(&quat, &axis, -theta);
			}
			else{
				D3DXVECTOR3 axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				D3DXQuaternionRotationAxis(&quat, &axis, theta);
			}

			D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &quat, 0.1f);

		}

		if (isDoubleInput){
			speed /= 1.414f;
		}

		if (isShift){
			speed *= 2.5f;
			m_BlendAdd = 0.1f;

			if (m_NextAnimationName != "Run" && !isRun && !isJump && speed > 0.0f){
				m_AnimationName = m_NextAnimationName;
				m_NextAnimationName = "Run";
				m_BlendRate = 0.0f;
			}

			isRun = true;
		}
		else{
			isRun = false;
		}

		velocity += GetForward() * speed;

	}
	else{
		if (m_NextAnimationName != "Idle" && !isJump){
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Idle";
			m_BlendRate = 0.0f;
		}
	}

}

void Player::Jump()
{
	if (Input::GetKeyTrigger(' ')) {
		isJump = true;
		jmpCnt = 0;

		if (m_NextAnimationName != "Jump"){
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Jump";
			m_BlendRate = 0.0f;
			m_Time = 30;
		}
	}

	if (isJump){

		if(jmpCnt > 0){
			velocity.y = jumpPower * sinf(jmpCnt * 3.14f / 180);
			jmpCnt += 360.0f / 60.0f;
		}

		if (jmpCnt > 300){
			isJump = false;

			if (m_NextAnimationName != "Idle"){
				m_AnimationName = m_NextAnimationName;
				m_NextAnimationName = "Idle";
				m_BlendRate = 0.0f;
			}
		}

		jmpCnt++;
	}

	
}

void Player::ShowProperty()
{
	if (!isShowProperty) return;

	if (ImGui::TreeNode(m_ObjectName.c_str())){
		GameObject::ShowProperty();

		ImGui::DragFloat("Player move speed", &moveSpeed, 0.01f,  0.0f, 10.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::Text("GroundHeight : %g", groundHeight);
		ImGui::Text("Velocity : %g, %g, %g", velocity.x, velocity.y, velocity.z);
		ImGui::TreePop();
	}
}

bool Player::CollisionInit()
{
	{
		//OBBの作成
		auto pOBB = AddComponent<OBBCollider>();
		OBBConfigData ocd;
		ocd.OBBColliderPosition = m_Position;
		ocd.OBBColliderPosition.y += 1.0f;
		ocd.OBBRotaion = m_Rotation;
		ocd.OBBScale = { 0.2f, 0.9f, 0.2f };
		ocd.OBBForwardVector = GetForward();
		ocd.OBBRightVector = GetRight();
		ocd.OBBUpVector = GetUp();
		pOBB->Init(&ocd, this);
				
		//collisionListに登録
		if (Manager::GetScene()->SetCollision(pOBB))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;

}

bool Player::CollisionUpdate()
{
	groundHeight = 0.0f;
	auto pOBB = GetComponent<OBBCollider>();
	OBBConfigData ocd;
	ocd.OBBColliderPosition = m_Position;
	ocd.OBBColliderPosition.y += 1.0f;
	ocd.OBBRotaion = m_Rotation;
	ocd.OBBScale = { 0.2f, 0.9f, 0.2f };
	ocd.OBBForwardVector = GetForward();
	ocd.OBBRightVector = GetRight();
	ocd.OBBUpVector = GetUp();
	pOBB->Update(&ocd);

	//当たり判定
	{
		//シーン内のテーブルを取得
		auto pTableVector = Manager::GetScene()->GetGameObjects<Table>();

		//取得したテーブルごとに当たり判定
		for (auto pTable : pTableVector){
			if (pTable && CheckIsCollision(pTable->GetName())){

				//接触方向（Playerから見てどの方向にテーブルがあるか）に合わせて移動制御
				D3DXVECTOR3 direction = pTable->GetPosition() - m_Position;
				D3DXVec3Normalize(&direction, &direction);
				float topHeight = 0.85f;
				if (m_Position.y < topHeight){

					D3DXVec3Normalize(&direction, &direction);
					if (direction.x > 0.9f){
						if (velocity.x > 0.0f){
							velocity.x = 0.0f;
						}
					}
					else if (direction.x < -0.9f){
						if (velocity.x < 0.0f){
							velocity.x = 0.0f;
						}
					}
					if (direction.z > 0.75f){
						if (velocity.z > 0.0f){
							velocity.z = 0.0f;
						}
					}
					else if (direction.z < -0.75f){
						if (velocity.z < 0.0f){
							velocity.z = 0.0f;
						}
					}
				}
			}
		}

	}

	if (m_Position.y > groundHeight){
		velocity.y -= gravity;
	}

	m_Position += velocity;

	//接地判定
	if (m_Position.y < groundHeight) {
		m_Position.y = groundHeight;
	}

	return true;

}


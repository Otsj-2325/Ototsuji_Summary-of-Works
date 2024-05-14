#include "main.h"
#include "obbCollision.h"
#include "gameObject.h"

void OBBCollider::Init(OBBConfigData* oc, GameObject* own)
{
	ownerObject = own;

	m_Pos = oc->OBBColliderPosition;
	m_Rot = oc->OBBRotaion;

	m_NormalDirection[0] = oc->OBBRightVector;
	m_NormalDirection[1] = oc->OBBUpVector;
	m_NormalDirection[2] = oc->OBBForwardVector;

	m_Length[0] = oc->OBBScale.x;
	m_Length[1] = oc->OBBScale.y;
	m_Length[2] = oc->OBBScale.z;

}

void OBBCollider::Update()
{
	if (m_ProcSelf)
	{
		return;
	}

	GameObject* owner = ownerObject;

	m_Pos = owner->GetPosition();

	m_NormalDirection[0] = owner->GetRight();
	m_NormalDirection[1] = owner->GetUp();
	m_NormalDirection[2] = owner->GetForward();

	auto scl= owner->GetScale();
	m_Length[0] = scl.x;
	m_Length[1] = scl.y;
	m_Length[2] = scl.z;

	D3DXVECTOR3 vBoxScale = {m_Length[0], m_Length[1], m_Length[2]};

}

void OBBCollider::Update(OBBConfigData* oc)
{
	m_ProcSelf = true;

	m_Pos = oc->OBBColliderPosition;

	m_NormalDirection[0] = oc->OBBRightVector;
	m_NormalDirection[1] = oc->OBBUpVector;
	m_NormalDirection[2] = oc->OBBForwardVector;

	m_Length[0] = oc->OBBScale.x;
	m_Length[1] = oc->OBBScale.y;
	m_Length[2] = oc->OBBScale.z;
}

void OBBCollider::Draw()
{
}

void OBBCollider::ShowProperty()
{
	if (!ownerObject->isShowProperty) return;

	std::string treename = ownerObject->GetName() + " collision";
	float pos[3] = { m_Pos.x, m_Pos.y, m_Pos.z };
	float rot[3] = { m_Rot.x, m_Rot.y, m_Rot.z };
	float scl[3] = { m_Length[0], m_Length[1], m_Length[2]};

	if (ImGui::TreeNode(treename.c_str())){
		ImGui::Text("Position : (%g, %g, %g)", pos[0], pos[1], pos[2]);
		ImGui::DragFloat3("Position", pos, 0.01f, -9999.0f, 9999.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Text("Rotation : (%g, %g, %g)", rot[0], rot[1], rot[2]);
		ImGui::DragFloat3("Rotation", rot, 0.01f, -9999.0f, 9999.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Text("Scale : (%g, %g, %g)", scl[0], scl[1], scl[2]);
		ImGui::DragFloat3("Scale", scl, 0.01f, -9999.0f, 9999.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::TreePop();
	}

		D3DXMATRIX rotMtx;
		D3DXVECTOR3 forward, right, up;

		D3DXMatrixRotationYawPitchRoll(&rotMtx, rot[1], rot[0], rot[2]);
		forward.x = rotMtx._31;
		forward.y = rotMtx._32;
		forward.z = rotMtx._33;

		right.x = rotMtx._11;
		right.y = rotMtx._12;
		right.z = rotMtx._13;

		up.x = rotMtx._21;
		up.y = rotMtx._22;
		up.z = rotMtx._23;

		OBBConfigData ocd;
		ocd.OBBColliderPosition = pos;
		ocd.OBBRotaion = rot;
		ocd.OBBScale = scl;
		ocd.OBBForwardVector = forward;
		ocd.OBBRightVector = right;
		ocd.OBBUpVector = up;

		Update(&ocd);

}

float OBBCollider::GetLengthOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0)
{
	float r1 = fabs(D3DXVec3Dot(Sep, e1));
	float r2 = fabs(D3DXVec3Dot(Sep, e2));
	float r3;
	if(e3){
		r3 = fabs(D3DXVec3Dot(Sep, e3));
	}
	else{
		r3 = 0;
	}

	return r1 + r2 + r3;
}

bool OBBCollider::OBBCheckCollision(OBBCollider* target)
{
	//自分自身の方向ベクトルを用意
	D3DXVECTOR3 NAe0 = GetDirection(0), Ae0 = NAe0 * GetLength(0);
	D3DXVECTOR3 NAe1 = GetDirection(1), Ae1 = NAe1 * GetLength(1);
	D3DXVECTOR3 NAe2 = GetDirection(2), Ae2 = NAe2 * GetLength(2);

	//相手のOBBの方向ベクトルを用意
	D3DXVECTOR3 NBe0 = target->GetDirection(0), Be0 = NBe0 * target->GetLength(0);
	D3DXVECTOR3 NBe1 = target->GetDirection(1), Be1 = NBe1 * target->GetLength(1);
	D3DXVECTOR3 NBe2 = target->GetDirection(2), Be2 = NBe2 * target->GetLength(2);

	//自分と相手の距離
	D3DXVECTOR3 interval = GetPosition() - target->GetPosition();

	//Separate plane : A.Right
	float rA = D3DXVec3Length(&Ae0);
	float rB = GetLengthOnSeparateAxis(&NAe0, &Be0, &Be1, &Be2);
	float l = fabs(D3DXVec3Dot(&interval, &NAe0));
	if (l > rA + rB){
		return false;
	}

	//Separate plane : A.Up
	rA = D3DXVec3Length(&Ae1);
	rB = GetLengthOnSeparateAxis(&NAe0, &Be0, &Be1, &Be2);
	l = fabs(D3DXVec3Dot(&interval, &NAe1));
	if (l > rA + rB){
		return false;
	}

	//Separate plane : A.Forward
	rA = D3DXVec3Length(&Ae2);
	rB = GetLengthOnSeparateAxis(&NAe0, &Be0, &Be1, &Be2);
	l = fabs(D3DXVec3Dot(&interval, &NAe2));
	if (l > rA + rB){
		return false;
	}

	//Separate plane : B.Right
	rA = D3DXVec3Length(&Be0);
	rB = GetLengthOnSeparateAxis(&NBe0, &Ae0, &Ae1, &Ae2);
	l = fabs(D3DXVec3Dot(&interval, &NBe0));
	if (l > rA + rB){
		return false;
	}

	//Separate plane : B.Up
	rA = D3DXVec3Length(&Be1);
	rB = GetLengthOnSeparateAxis(&NBe1, &Ae0, &Ae1, &Ae2);
	l = fabs(D3DXVec3Dot(&interval, &NBe1));
	if (l > rA + rB){
		return false;
	}

	//Separate plane : B.Forward
	rA = D3DXVec3Length(&Be2);
	rB = GetLengthOnSeparateAxis(&NBe2, &Ae0, &Ae1, &Ae2);
	l = fabs(D3DXVec3Dot(&interval, &NBe2));
	if (l > rA + rB){
		return false;
	}

	//外積分離軸
	D3DXVECTOR3 cross;
	
	//A.Right, B.Rightに垂直な分離軸
	D3DXVec3Cross(&cross, &NAe0, &NBe0);
	rA = GetLengthOnSeparateAxis(&cross, &Ae1, &Ae2);
	rB = GetLengthOnSeparateAxis(&cross, &Be1, &Be2);
	l = fabs(D3DXVec3Dot(&interval, &cross));
	if (l > rA + rB){
		return false;
	}

	//A.Right, B.Upに垂直な分離軸
	D3DXVec3Cross(&cross, &NAe0, &NBe1);
	rA = GetLengthOnSeparateAxis(&cross, &Ae1, &Ae2);
	rB = GetLengthOnSeparateAxis(&cross, &Be0, &Be2);
	l = fabs(D3DXVec3Dot(&interval, &cross));
	if (l > rA + rB){
		return false;
	}

	//A.Right, B.Forwardに垂直な分離軸
	D3DXVec3Cross(&cross, &NAe0, &NBe2);
	rA = GetLengthOnSeparateAxis(&cross, &Ae1, &Ae2);
	rB = GetLengthOnSeparateAxis(&cross, &Be0, &Be1);
	l = fabs(D3DXVec3Dot(&interval, &cross));
	if (l > rA + rB){
		return false;
	}

	//A.Up, B.Rightに垂直な分離軸
	D3DXVec3Cross(&cross, &NAe1, &NBe0);
	rA = GetLengthOnSeparateAxis(&cross, &Ae0, &Ae2);
	rB = GetLengthOnSeparateAxis(&cross, &Be1, &Be2);
	l = fabs(D3DXVec3Dot(&interval, &cross));
	if (l > rA + rB){
		return false;
	}

	//A.Up, B.Upに垂直な分離軸
	D3DXVec3Cross(&cross, &NAe1, &NBe1);
	rA = GetLengthOnSeparateAxis(&cross, &Ae0, &Ae2);
	rB = GetLengthOnSeparateAxis(&cross, &Be0, &Be2);
	l = fabs(D3DXVec3Dot(&interval, &cross));
	if (l > rA + rB){
		return false;
	}

	//A.Up, B.Forwardに垂直な分離軸
	D3DXVec3Cross(&cross, &NAe1, &NBe2);
	rA = GetLengthOnSeparateAxis(&cross, &Ae0, &Ae2);
	rB = GetLengthOnSeparateAxis(&cross, &Be0, &Be1);
	l = fabs(D3DXVec3Dot(&interval, &cross));
	if (l > rA + rB){
		return false;
	}

	//A.Forward, B.Rightに垂直な分離軸
	D3DXVec3Cross(&cross, &NAe2, &NBe0);
	rA = GetLengthOnSeparateAxis(&cross, &Ae0, &Ae1);
	rB = GetLengthOnSeparateAxis(&cross, &Be1, &Be2);
	l = fabs(D3DXVec3Dot(&interval, &cross));
	if (l > rA + rB){
		return false;
	}

	//A.Forward, B.Upに垂直な分離軸
	D3DXVec3Cross(&cross, &NAe2, &NBe1);
	rA = GetLengthOnSeparateAxis(&cross, &Ae0, &Ae1);
	rB = GetLengthOnSeparateAxis(&cross, &Be0, &Be2);
	l = fabs(D3DXVec3Dot(&interval, &cross));
	if (l > rA + rB){
		return false;
	}

	//A.Forward, B.Forwardに垂直な分離軸
	D3DXVec3Cross(&cross, &NAe2, &NBe2);
	rA = GetLengthOnSeparateAxis(&cross, &Ae0, &Ae1);
	rB = GetLengthOnSeparateAxis(&cross, &Be0, &Be1);
	l = fabs(D3DXVec3Dot(&interval, &cross));
	if (l > rA + rB){
		return false;
	}

	return true;
}

D3DXVECTOR3 OBBCollider::GetMinPoint()
{
	D3DXVECTOR3 center = GetPosition();
	for (int i = 0; i < 3; i++)
	{
		center -= m_NormalDirection[i] * m_Length[i];
	}

	return center;
}

D3DXVECTOR3 OBBCollider::GetMaxPoint()
{
	D3DXVECTOR3 center = GetPosition();
	for (int i = 0; i < 3; i++)
	{
		center += m_NormalDirection[i] * m_Length[i];
	}

	return center;
}

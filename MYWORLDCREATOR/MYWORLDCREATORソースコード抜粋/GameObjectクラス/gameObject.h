#pragma once

#include <vector>
#include <list>
#include <string>

#include "main.h"
#include "renderer.h"
#include "hierarchy.h"
#include "component.h"
#include "resourceManager.h"
#include "TSPObject.h"

class GameObject
{
protected:
	std::string m_ObjectName = "NoName";
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXQUATERNION m_Quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	
	D3DXMATRIX m_WorldMatrix;

	GameObject* m_Parent;
	std::list<GameObject*> m_ChildList;
	std::list<Component*> m_Component; // STLのリスト構造
	std::vector<std::string> m_CollisionList;

	bool m_Draw = true;
	bool isDestroy = false;
	bool m_Active = true;
	
public:

	GameObject()
	{
		Init();
	}

	/// <summary>コピーコンストラクタ</summary>
	/// <param name="obj">コピー元</param>
	GameObject(GameObject& obj)
	{
		*this = obj;
	}

	~GameObject()
	{
		Uninit();
	}

	///<summary>インスペクタの表示切り替え</summary>
	bool isShowProperty = false;
	
	///<summary>GameObject 初期化処理</summary>
	virtual void Init()
	{
		m_Position = {0.0f, 0.0f, 0.0f};
		m_Rotation = {0.0f, 0.0f, 0.0f};
		m_Scale = {1.0f, 1.0f, 1.0f};
		CalcWorldMatrix();

		for (Component* component : m_Component)
		{
			component->Init(this);
		}
	}

	///<summary>GameObject 解放処理</summary>
	virtual void Uninit()
	{
		m_ChildList.clear(); // 入れ物を消す リスト構造の削除


		for (Component* component : m_Component) // 範囲forループ
		{
			component->Uninit();
			delete component;
		}

		m_Component.clear(); // 入れ物を消す リスト構造の削除
	}

	///<summary>GameObject 更新処理</summary>
	virtual void Update()
	{
		for (Component* component : m_Component)
		{
			component->Update();
		}

		for (GameObject* child : m_ChildList)
		{
			child->Update();
		}
	}

	///<summary>GameObject 描画処理</summary>
	virtual void Draw()
	{
		CalcWorldMatrix();

		Renderer::SetWorldMatrix(&m_WorldMatrix);

		for (Component* component : m_Component)
		{
			component->Draw(); // ポリモフィズム
		}
	}

	/// <summary>インスペクタ表示処理</summary>
	virtual void ShowProperty()
	{
		if(!isShowProperty) return;

		std::string treename = m_ObjectName + " transform";
		if(ImGui::TreeNode(treename.c_str())){

			float pos[3] = {m_Position.x, m_Position.y, m_Position.z};
			float rot[3] = {m_Rotation.x, m_Rotation.y, m_Rotation.z};
			float quat[3];
			float scl[3] = {m_Scale.x, m_Scale.y, m_Scale.z};

			ImGui::Text("Position : (%g, %g, %g)", m_Position.x, m_Position.y, m_Position.z);
			ImGui::DragFloat3("Position", pos, 0.01f, -9999.0f, 9999.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
		
			ImGui::Text("Rotation : (%g, %g, %g)", m_Rotation.x, m_Rotation.y, m_Rotation.z);
			ImGui::DragFloat3("Rotation", rot, 0.01f, -9999.0f, 9999.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
		
			ImGui::Text("Scale    : (%g, %g, %g)", m_Scale.x, m_Scale.y, m_Scale.z);
			ImGui::DragFloat3("Scale", scl, 0.01f, -9999.0f, 9999.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
		
			m_Position = { pos[0], pos[1], pos[2] };
			m_Rotation = { rot[0], rot[1], rot[2] };
			m_Scale = { scl[0], scl[1], scl[2]};

			D3DXVECTOR3 forward = GetForward();
			ImGui::Text("Forward : (%g, %g, %g)", forward.x, forward.y, forward.z);

			ImGui::TreePop();
		}

		for (auto child : m_ChildList)
		{
			child->ShowProperty();
		}

	}
		
	///<summary>オブジェクト名設定</summary>
	void SetName(std::string name)
	{
		m_ObjectName = name;
	}

	///<summary>オブジェクト名取得</summary>
	std::string GetName()
	{
		return m_ObjectName;
	}

	///<summary>更新処理の実行有無設定</summary>
	void IsActive(bool b) { m_Active = b; }

	///<summary>更新処理の実行有無取得</summary>
	bool IsActive() { return m_Active; }

	/// <summary>コンポネント追加</summary>
	/// <param name="T">スーパークラスが"Class Component"であるクラス</param>
	/// <returns name="Return Value">追加したコンポネントの実体へのポインタ</returns>
	template <typename T> // テンプレート関数 型を引数で渡せる
	T* AddComponent()
	{
		Component* component = new T();
		m_Component.push_back(component);
		component->Init(this);

		return (T*)component;
	}

	/// <summary>コンポネント検索</summary>
	/// <param name="T">スーパークラスが"Class Component"であるクラス</param>
	/// <returns name="Return Value">検索したコンポネントがあったらそのポインタ、ない場合はnullptr</returns>
	template <typename T> // テンプレート関数 型を引数で渡せる
	T* GetComponent()
	{
		for (Component* c : m_Component){
			if (typeid(*c) == typeid(T)){
				return (T*)c;
			}
		}

		return nullptr;
	}

	///<summary>m_Parentの設定（親子関係の親の設定）</summary>
	void SetParent(GameObject* obj){
		m_Parent = obj;
	}

	///<summary>m_ChildListへの追加（親子関係の子の設定）</summary>
	void AddChild(GameObject* obj){
		m_ChildList.push_back(obj);
	}

	///<summary>m_ChildList全体の設定（子オブジェクトを全て再設定）</summary>
	void SetChildList(std::list<GameObject*> list){
		m_ChildList.clear();
		m_ChildList = list;
	}


	///<summary>WorldMatrix計算・設定</summary>
	///<param name = "m_WorldMatrix">本関数終了後に値がある変数</param>
	void CalcWorldMatrix()
	{
		D3DXMATRIX world, scale, rot, quat, trans;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		D3DXMatrixRotationQuaternion(&quat, &m_Quaternion);
		D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);

		world = scale * quat * rot * trans;

		if(m_Parent) world *= m_Parent->ObjectWorldMatrix();

		D3DXMATRIX dummmy = rot * quat * trans;
		ObjectWorldMatrix(world);

		Renderer::SetWorldMatrix(&world);

	};
	
	///<summary>m_WorldMatrixの設定</summary>
	void ObjectWorldMatrix(D3DXMATRIX world){ m_WorldMatrix = world; };
	
	///<summary>m_WorldMatrixの取得</summary>
	D3DXMATRIX ObjectWorldMatrix(){ return m_WorldMatrix; };

	/************************************************************************/
	/********座標・回転・スケールの各ベクトル要素の書き込み・取得処理********/
	/************************************************************************/
	D3DXVECTOR3 SetPosition(D3DXVECTOR3 set) {
		m_Position = set;
		return m_Position;
	}

	D3DXVECTOR3 GetPosition() {
		return m_Position;
	}

	D3DXVECTOR3 SetRotation(D3DXVECTOR3 set) {
		m_Rotation = set;
		return m_Rotation;
	}

	D3DXVECTOR3 GetRotation() {
		return m_Rotation;
	}

	D3DXVECTOR3 SetScale(D3DXVECTOR3 set) {
		m_Scale = set;
		return m_Scale;
	}

	D3DXVECTOR3 GetScale() {
		return m_Scale;
	}


	///<summary></summary>
	virtual D3DXVECTOR3 GetForward() {
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	///<summary></summary>
	D3DXVECTOR3 GetRight() {
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 right;
		right.x = rot._11;
		right.y = rot._12;
		right.z = rot._13;

		return right;
	}

	///<summary></summary>
	D3DXVECTOR3 GetUp(){
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 up;
		up.x = rot._21;
		up.y = rot._22;
		up.z = rot._23;

		return up;
	}

	///<summary></summary>
	bool GetIsDraw()
	{
		return m_Draw;
	}

	///<summary>オブジェクト削除フラグ設定</summary>
	///<param name="isDestroy">設定先フラグ変数</param>
	void SetIsDestroy(bool set) {
		isDestroy = set;
	}

	///<summary>オブジェクト削除フラグ取得</summary>
	bool GetIsDestroy() {
		return isDestroy;
	}

	///<summary>AABB衝突判定</summary>
	bool isEnterObject(GameObject* target) {
		D3DXVECTOR3 objPos = target->GetPosition();
		D3DXVECTOR3 objScl = target->GetScale();

		D3DXVECTOR2 areaX = D3DXVECTOR2(objPos.x - objScl.x, objPos.x + objScl.x);
		D3DXVECTOR2 areaY = D3DXVECTOR2(objPos.y - objScl.y, objPos.y + objScl.y);
		D3DXVECTOR2 areaZ = D3DXVECTOR2(objPos.z - objScl.z, objPos.z + objScl.z);

		bool isEnterX = (m_Position.x > areaX.x && m_Position.x < areaX.y);
		bool isEnterY = (m_Position.y > areaY.x && m_Position.y < areaY.y);
		bool isEnterZ = (m_Position.z > areaZ.x && m_Position.z < areaZ.y);

		if (isEnterX && isEnterY && isEnterZ) {
			return true;
		}
		else {
			return false;
		}
	}

	///<summary>衝突相手リストクリア</summary>
	void ClearColliionList()
	{
		m_CollisionList.clear();
	}

	///<summary>衝突相手リスト追加</summary>
	///<param name="target">衝突相手オブジェクトのm_ObjectName</param>
	void AddCollisionList(std::string target)
	{
		m_CollisionList.push_back(target);
	}

	/// <summary>ターゲットとの衝突の有無の取得</summary>
	///<param name="target">衝突相手オブジェクトのm_ObjectName</param>
	bool CheckIsCollision(std::string target)
	{
		int cntNum = std::count_if(m_CollisionList.begin(), m_CollisionList.end(),
			[target](std::string listName){
				return target == listName;
			});

		if (cntNum)
		{
			return true;
		}

		return false;
	}	

	virtual bool CollisionInit()
	{
		return false;
	}

	virtual bool CollisionUpdate()
	{
		return false;
	}
};

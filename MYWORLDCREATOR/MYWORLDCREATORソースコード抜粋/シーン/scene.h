#pragma once

#include "renderer.h"
#include "gameObject.h"
#include "input.h"
#include "hierarchy.h"
#include "AssetList.h"
#include "selectCursor.h"
#include "saveScene.h"
#include "TSPManager.h"

#include <list>

enum class RENDERMODE
{
	DefaultRender,
	AlbedRender,
	NormalRender,
	WPositionRender
};

class Scene
{
protected:
	Scene* scene;
	Hierarchy* m_HierarchyList;
	AssetList* m_Assetlist;
	D3DXVECTOR3 selectPos;
	bool m_CursorClip = false;
	bool m_ShowDemo = false;
	RENDERMODE m_RenderMode = RENDERMODE::DefaultRender;
	
	TSPManager* m_pTSPMng = NULL;


public:
	virtual void Init()
	{
		selectPos = { 0.0f, 0.0f, 0.0f };

		m_HierarchyList = new Hierarchy();
		m_Assetlist = new AssetList();

		SelectCursor::Init();
	}

	virtual void Uninit()
	{
		m_Assetlist->UninitAssetList();
		m_HierarchyList->UninitHierarchy();
	}

	virtual void Update()
	{	
		CursorClip();

		m_HierarchyList->UpdateHierarchy();
		m_Assetlist->UpdateAssetList();
	}

	virtual void Draw()
	{
		PARAM p;
		p.Param[0] = (int)m_RenderMode;
		Renderer::SetParam(p);

		m_HierarchyList->DrawHierarchy();
		SelectCursor::Draw();
	}

	void CursorClip()
	{
		RECT wndRect;
		HWND wnd;

		//フラグ切り替え
		if (Input::GetKeyTrigger('P')){
			m_CursorClip = !m_CursorClip;
		}

		//クリップ処理がないならデスクトップにカーソルを返す
		if (!m_CursorClip){
			wnd = GetDesktopWindow();
			GetWindowRect(wnd, &wndRect);
			ClipCursor(&wndRect);

			return;
		}

		//クリップ処理があるなら、両端処理をする
		wnd = GetWindow();
		if (wnd == GetFocus()){
			POINT cursorPos;
			GetWindowRect(wnd, &wndRect);
			ClipCursor(&wndRect);

			GetCursorPos(&cursorPos);
			if (cursorPos.x <= (float)SCENE_WIDTH_L + 1.0f){
				cursorPos.x = (float)SCENE_WIDTH_R - 2.0f;
			}
			if (cursorPos.x >= (float)SCENE_WIDTH_R - 1.0f){
				cursorPos.x = (float)SCENE_WIDTH_L + 2.0f;
			}
			
			if (cursorPos.y <= (float)1.0f){
				cursorPos.y = (float)SCREEN_HEIGHT - 2.0f;
			}
			if (cursorPos.y >= (float)SCREEN_HEIGHT - 1.0f){
				cursorPos.y = (float)1.0f;
			}
			SetCursorPos((int)cursorPos.x, (int)cursorPos.y);

		}
	}

	virtual void ShowProperty()
	{
		m_HierarchyList->ShowProperty();
		m_Assetlist->ShowAssetList();
	}

	Hierarchy GetHierarchy(){ return *m_HierarchyList; }

	template <typename T> // テンプレート関数 型を引数で渡せる
	T* AddGameObject(int layer)
	{
		auto gameObject = m_HierarchyList->AddGameObjectHierarchy<T>();
		return gameObject;
	}

	auto AddGameObject(GameObject* pObj)
	{
		auto gameObject = m_HierarchyList->AddGameObjectHierarchy("NewObject", pObj);
		return gameObject;
	}

	template <typename T>
	T* GetGameObject() 
	{
		auto gameObject = m_HierarchyList->GetGameObjectHierarchy<T>();
		if (gameObject){
			return gameObject;
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		return m_HierarchyList->GetGameObjectHierarchyByVector<T>();

	}

	GameObject* GetGameObject(std::string name)
	{
		auto gameObject = m_HierarchyList->GetGameObjectHierarchy(name);
		if (gameObject){
			return gameObject;
		}

		return nullptr;
	}
	
	bool CheckCollisionProc()
	{
		if (m_pTSPMng)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	bool SetCollision(OBBCollider* obb)
	{
		if (m_pTSPMng)
		{
			if (m_pTSPMng->SetColTreeObject(obb))
			{
				return true;
			}
			
		}

		return false;
		
	}

};
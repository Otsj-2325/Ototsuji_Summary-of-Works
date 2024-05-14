#pragma once

#include "post.h"
#include "Hierarchy.h"

class Scene;

class Manager
{
private:
	static Scene* m_Scene; // staticにするとインスタンス生成しなくても使えるようになる 静的は.cpp側でもう一回呼ばないといけない
	static Scene* m_NextScene;
	static Post* m_post;
	static Hierarchy m_TempHierarchy;

	static bool m_HaveHierarchy;
	static bool m_HaveColList;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() { return m_Scene; }
	static Post* GetPost() { return m_post; }
	
	template <typename TScene>
	static void SetScene() {
		
		m_NextScene = new TScene();
	}

	static void SetHierarchy(Hierarchy* temp)
	{
		m_TempHierarchy = *temp;
		m_HaveHierarchy = true;
	}

	static Hierarchy* GetHierarchy()
	{
		if (!m_HaveHierarchy)
		{
			return nullptr;
		}

		return &m_TempHierarchy;
	}


};
#pragma once

#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>

class GameObject;

class Hierarchy
{
private:
	std::map<std::string, GameObject*> hierarchy; 

public:
	void InitHierarchy();
	void UninitHierarchy();
	void UpdateHierarchy();
	void DrawHierarchy();
	void ShowProperty();
	void ShowInspector();
	void ShowHierarchyList();

	auto& GetList() { return hierarchy; }

	template<typename T>
	T* AddGameObjectHierarchy(std::string name){
		T* obj = new T();
		hierarchy.try_emplace(name, obj);

		return obj;
	}

	
	GameObject* AddGameObjectHierarchy(std::string name, GameObject* set);

	template<typename T>
	T* GetGameObjectHierarchy(){
		for (auto obj : hierarchy){
			if (typeid(*obj.second) == typeid(T)){
				return (T*)obj.second;
			}
		}

		return nullptr;
	}

	GameObject* GetGameObjectHierarchy(std::string name){
		for (auto obj : hierarchy){
			if (obj.first == name){
				return (GameObject*)obj.second;
			}
		}

		return nullptr;
	}

	int GetLength(){ return (int)hierarchy.size(); }

	void AddChild(std::string name, GameObject* pObj);
};
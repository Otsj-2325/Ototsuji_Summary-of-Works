#include "main.h"
#include "gameObject.h"
#include "collisionList.h"
#include "hierarchy.h"
#include <map>

void Hierarchy::InitHierarchy()
{
	for (auto obj : hierarchy){
		obj.second->SetName(obj.first);
	}

}

void Hierarchy::UninitHierarchy()
{
	/*for (auto obj : hierarchy){
		obj.second->Uninit();

		delete obj.second;
	}*/

	//HierarchyListŽ©‘Ì‚Ìíœ
	hierarchy.clear();
}

void Hierarchy::UpdateHierarchy()
{
	for (auto obj : hierarchy){
		if(!(obj.second->IsActive())) continue;
		obj.second->Update();
	}
}

void Hierarchy::DrawHierarchy()
{
	for (auto obj : hierarchy){
		if (!obj.second->IsActive()) continue;
		if(obj.second->GetIsDraw()) obj.second->Draw();
	}
}

void Hierarchy::ShowProperty()
{
	ShowInspector();
	ShowHierarchyList();
}

void Hierarchy::ShowInspector()
{
	ImGuiWindowFlags windowFlag = 0;
	windowFlag |= ImGuiWindowFlags_NoResize;
	windowFlag |= ImGuiWindowFlags_NoScrollbar;
	windowFlag |= ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowSize(ImVec2(300.0f, SCREEN_HEIGHT / 2), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 300.0f, 0.0f));
	ImGui::Begin("Inspector", 0, windowFlag);

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(300.0f, 600.0f));
	for (auto obj : hierarchy){
		obj.second->ShowProperty();
	}
	ImGui::EndChild();

	ImGui::End();
}

void Hierarchy::ShowHierarchyList()
{
	ImGuiWindowFlags windowFlag = 0;
	windowFlag |= ImGuiWindowFlags_NoResize;
	windowFlag |= ImGuiWindowFlags_NoScrollbar;
	windowFlag |= ImGuiWindowFlags_NoCollapse;

	int hierarchyCnt = hierarchy.size();
	std::string titleName = "Hierarchy : " + std::to_string(hierarchyCnt);

	ImGui::SetNextWindowSize(ImVec2(150, SCREEN_HEIGHT / 2), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::Begin(titleName.c_str(), 0, windowFlag);

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(150, 300));
	for (int i = 1; i < hierarchy.size(); i++) {
		auto obj = *(std::next(hierarchy.begin(), i));

		std::string buttonName = obj.first;
		if (*(obj.first.begin()) == '0')
		{
			buttonName.erase(buttonName.begin());
		}

		if (ImGui::Button(buttonName.c_str(), ImVec2(100, 20)))
		{
			obj.second->isShowProperty = !obj.second->isShowProperty;
			SetMouseSkip();
		}

	}
	ImGui::EndChild();

	ImGui::End();

}

GameObject* Hierarchy::AddGameObjectHierarchy(std::string name, GameObject* set){
	hierarchy.try_emplace(name, set);
	set->SetName(name);
	return set;

}

void Hierarchy::AddChild(std::string name, GameObject* pObj){
	auto func = [name](GameObject* obj)
		{
			if (obj->GetName() == name){
				return true;
			}

			return false;
		};

	auto itr = hierarchy.begin();
	for (auto obj : hierarchy){
		if (func(obj.second)){
			break;
		}

		itr++;
	}

	auto PreItr = itr;
	PreItr--;
	auto FwItr = itr;
	FwItr++;

	if ((PreItr) == hierarchy.end()){
		return;
	}
	else if (itr == hierarchy.end()) {
		hierarchy.emplace(name, pObj);

		auto parent = *(hierarchy.end());
		pObj->SetParent(parent.second);
		parent.second->AddChild(pObj);
	}
	else{
		hierarchy.insert(std::make_pair(pObj->GetName(), pObj));

		auto parent = *(itr);
		pObj->SetParent(parent.second);
		parent.second->AddChild(pObj);
	}
}
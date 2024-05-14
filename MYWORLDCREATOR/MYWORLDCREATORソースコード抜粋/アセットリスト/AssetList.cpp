#include "main.h"
#include "gameObject.h"
#include "selectCursor.h"
#include "AssetList.h"
#include <map>

//GaneObject
#include "camera.h"
#include "CtrlCamera.h"
#include "field.h"
#include "skydorm.h"
#include "player.h"
#include "table.h"
#include "wall.h"
#include "volumeBox.h"
#include "gameObject_Model3D.h"
#include "polygon3D.h"
#include "waveface.h"

void AssetList::InitAssetList()
{
	AddGameAssetList("Skydorm");
	AddGameAssetList("Field");	
	AddGameAssetList("Table");
	AddGameAssetList("Wall");
	AddGameAssetList("Model3D");
	AddGameAssetList("Player");

}

void AssetList::UninitAssetList()
{	
	assetNameList.clear();
}

void AssetList::UpdateAssetList()
{
		
}

void AssetList::ShowAssetList()
{
	ImGuiWindowFlags windowFlag = 0;
	windowFlag |= ImGuiWindowFlags_NoResize;
	windowFlag |= ImGuiWindowFlags_NoScrollbar;
	windowFlag |= ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowSize(ImVec2(150, SCREEN_HEIGHT / 2), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0.0f, SCREEN_HEIGHT / 2));
	ImGui::Begin("AssetList", 0, windowFlag);

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(150, 300));
	
	bool flag = false;
	for (auto i = assetNameList.begin(); i < assetNameList.end(); i++) {
		auto obj = *i;
		if (flag = ImGui::Button(obj.second.c_str(), ImVec2(100, 20)))
		{
			SelectHandle = obj.first;
			SetMouseSkip();
		}

	}
	ImGui::EndChild();

	ImGui::End();
}

GameObject* AssetList::GetSelectAsset()
{
	int input = SelectHandle;
	auto itr = std::find_if(assetNameList.begin(), assetNameList.end(),
		[input](auto asset){
			return asset.first == input;
		});
	if (itr == assetNameList.end())
	{
		return nullptr;
	}
	auto pair = *itr;
	SelectHandle = -1;

	if (pair.second == "Skydorm")
	{
		return new Skydorm();
	}
	if (pair.second == "Field")
	{
		return new Field();
	}
	if (pair.second == "Table")
	{
		return new Table();
	}
	if (pair.second == "Wall")
	{
		return new Wall();
	}
	if (pair.second == "Model3D")
	{
		return new Model3D();
	}
	if (pair.second == "Player")
	{
		return new Player();
	}
}
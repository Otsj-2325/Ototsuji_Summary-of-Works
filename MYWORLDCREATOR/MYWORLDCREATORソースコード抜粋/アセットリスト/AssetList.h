#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include "gameObject_Model3D.h"

class AssetList
{
private:
	std::vector<std::pair<int, std::string>> assetNameList;
	int SelectHandle = 0;
	int handleNum = 0;

public:
	void InitAssetList();
	void UninitAssetList();
	void UpdateAssetList();
	void ShowAssetList();

	void AddGameAssetList(std::string name){
		handleNum++;
		auto set = std::pair<int, std::string>(handleNum, name);
		assetNameList.push_back(set);
	}

	GameObject* GetSelectAsset();
};
#include <sstream>

#include "main.h"
#include "renderer.h" 
#include "resourceManager.h"

#include "model.h"
#include "animationModel.h"

std::vector<std::pair<std::string, void*>> ResourceManager::m_ResourceList = {};

void ResourceManager::CheckDataType(std::string fullPath)
{
	std::istringstream ss1(fullPath);
	std::string piece;
	std::vector<std::string> fpCut;

	while (std::getline(ss1, piece, '\\'))
	{
		fpCut.push_back(piece);
	}

	//最後のcutが拡張子を持っているか
	std::string lastPiece = *(fpCut.end() - 1);
	auto haveExtension = std::find_if(lastPiece.begin(), lastPiece.end(), 
		[](auto s){
			return s == '.';
		});
	//拡張子が見つからない
	if (haveExtension == lastPiece.end())
	{
		/*Pathが正常ではない*/
	}

	std::string extension;
	std::istringstream ss2(lastPiece);
	std::vector<std::string> extCut;

	while (std::getline(ss2, extension, '.'));

	if (extension == "png")
	{
		ID3D11ShaderResourceView* texture;
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
			fullPath.c_str(),
			NULL,
			NULL,
			&texture,
			NULL);

		m_ResourceList.push_back({ fullPath, texture });
	}
	if (extension == "jpg")
	{
		ID3D11ShaderResourceView* texture;
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
			fullPath.c_str(),
			NULL,
			NULL,
			&texture,
			NULL);

		m_ResourceList.push_back({ fullPath, texture });

	}
	if (extension == "obj")
	{
		//.objの展開
		Model* model;
		model = new Model();
		model->Load(fullPath.c_str());

		m_ResourceList.push_back({ fullPath, model });
	}
	if (extension == "fbx")
	{
		AnimationModel* amodel;
		amodel = new AnimationModel();
		amodel->Load(fullPath.c_str());

		m_ResourceList.push_back({ fullPath, amodel });
	}

}

void* ResourceManager::GetResource(std::string dirPath)
{
	auto itr = std::find_if(m_ResourceList.begin(), m_ResourceList.end(), 
		[dirPath](auto a){
			return a.first == dirPath;
		});

	//既にロード済みのデータがあった場合
	if(itr != m_ResourceList.end()){
		auto resData = *(itr);
		return resData.second;
	}

	//データ種別チェック
	CheckDataType(dirPath);
	return GetResource(dirPath);

}



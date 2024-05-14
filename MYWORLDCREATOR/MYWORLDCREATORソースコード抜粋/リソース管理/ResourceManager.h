#pragma once

#include <vector>
#include <string>

class ResourceManager
{
	private:
	static std::vector<std::pair<std::string, void*>> m_ResourceList;
	static void CheckDataType(std::string fullPath);

	public:
	static void* GetResource(std::string dirPath);

};

#include "Model.h"

unique_ptr<AssetManager>	g_assetManagerPtr;

void AssetManager::Init()
{
	AddModelsFromDirectory("assets/models/molecules");
	AddModelsFromDirectory("assets/models/parts");
	AddModelsFromDirectory("assets/models/cells");
	AddModelsFromDirectory("assets/models/hotspots");
	AddModelsFromDirectory("assets/models/terrains");
}

void AssetManager::AddModelsFromDirectory(const string& directory)
{
	directory_iterator end;

	for (directory_iterator it(boost::filesystem::path(directory.c_str())); it != end; ++it)
		if (!is_directory(*it)) AddModelFromFile((*it).path().string());
}

void AssetManager::AddModelFromFile(const string& filepath)
{
	ptree pt;

	read_json(filepath, pt);

	MakeModel(pt.get<string>("type"))->Load(pt);
}

shared_ptr<Model> AssetManager::GetModel(const string& name) const
{
	for (auto it = m_models.begin(); it != m_models.end(); ++it)
		if ((*it)->GetName() == name && dynamic_pointer_cast<Model>(*it) != nullptr) return dynamic_pointer_cast<Model>(*it);

	return nullptr;
}

#include "Model.h"

#include "ViewerManager.h"
#include "Viewer.h"

#include <boost/property_tree/json_parser.hpp>

void Model::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<Viewer>();
}

ptree Model::AddToJSON(ptree pt) const
{
	pt.put("name", m_name);

	return pt;
}

void Model::SetFromJSON(const ptree & pt)
{
	m_name = pt.get<string>("name");
}

string Model::GetFilename() const
{
	auto filename = m_name + ".json";
	auto f = [](unsigned char c) { return char(c == 0x20 ? 0x5f : tolower(c)); };

	transform(filename.begin(), filename.end(), filename.begin(), f);

	return filename;
}

string Model::GetFilepath() const
{
	return "assets/model/" + GetFilename();
}


#pragma once

#include "Part.h"

class PartManager
{
public:
	vector<shared_ptr<PartModel>>	m_models;

public:
	PartManager();
	~PartManager();

	const shared_ptr<PartModel>&	AddModel();
	const shared_ptr<PartModel>&	GetModel(const string& name) const;
};

extern shared_ptr<PartManager>	g_partManagerPtr;
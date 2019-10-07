﻿#pragma once

#include "Model.h"

// AssetManagerに登録可能なModel
// JSONファイルに対応している
class AssetModel
	: public Model
{
	string	m_name;

public:
	AssetModel(const string& name)
		: m_name(name)
	{}

	virtual void	MakeViewer() {}

	// Set
	void	SetName(const string& name);

	// Get
	const string& GetName() const { return m_name; }

	// Path
	virtual string	GetFilename() const;
	virtual string	GetFilepath() const { return "assets/model/" + GetFilename(); }

	// JSON
	void	load_this(const ptree& pt);
	virtual void	load(const ptree& pt) { load_this(pt); }
	void	save_this(ptree& pt) const;
	virtual void	save(ptree& pt) const { save_this(pt); }
};

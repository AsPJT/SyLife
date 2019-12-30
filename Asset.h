﻿#pragma once

#include "Model.h"

// AssetManagerに登録可能なModel
// JSONファイルに対応している
class Asset
	: public Model
{
	String	m_name;
	String	m_filepath;

public:
	Asset();

	// Set
	void	setName(const String& name) { m_name = name; }
	void	setFilePath(const String& filepath) { m_filepath = filepath; }

	// Get
	const String& getName() const { return m_name; }
	const String& getFilePath() const { return m_filepath; }

	// Icon
	virtual void	drawIcon() const {}

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	virtual String	getTypeName() { return U"Asset"; }
};

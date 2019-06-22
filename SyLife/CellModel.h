#pragma once

#include "Model.h"
#include "PartConfig.h"

class CellModel
	: public Model
{
public:
	vector<shared_ptr<PartConfig>>	m_parts;

	// �ߎ��~
	double	m_mass;
	double	m_radius;
	double	m_inertia;

public:
	void	AddPartConfig(const ptree& pt);

	template <typename T>
	void	AddPartConfig(const ptree& pt) { m_parts.emplace_back(make_shared<T>())->Load(pt); }

	template <typename T>
	shared_ptr<T>			GetPart(const string& name) const
	{
		for (auto it = m_parts.begin(); it != m_parts.end(); ++it)
			if ((*it)->m_name == name && dynamic_pointer_cast<T>(*it) != nullptr) return dynamic_pointer_cast<T>(*it);

		return nullptr;
	}

	template <typename T>
	vector<shared_ptr<T>>	GetParts() const
	{
		vector<shared_ptr<T>>	tModels;

		for (auto it = m_parts.begin(); it != m_parts.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tModels.emplace_back(dynamic_pointer_cast<T>(*it));

		return tModels;
	}

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }

	string	GetFilepath() const override { return "assets/cell/" + GetFilename(); }

	void	CalculateDisk();
};
﻿#pragma once

#include "Model.h"
#include "Molecule.h"

class MoleculeModel;

class Storage
	: public Model
{
	Array<pair<shared_ptr<MoleculeModel>, unsigned int>>	m_molecules;

public:
	// operator
	Storage	operator +(const Storage& s) const { return Storage(*this) += s; }
	Storage	operator -(const Storage& s) const { return Storage(*this) -= s; }
	bool operator >=(const Storage& s) const
	{
		for (const auto& m : s.m_molecules)
			if (Num(m.first) < m.second) return false;

		return true;
	}	
	bool operator <=(const Storage& s) const
	{
		for (const auto& m : m_molecules)
			if (s.Num(m.first) < m.second) return false;

		return true;
	}
	Storage& operator +=(const Storage& s) noexcept
	{
		for (const auto& m : s.m_molecules)
			Add(m.first, m.second);

		return *this;
	}
	Storage& operator -=(const Storage& s) noexcept
	{
		for (const auto& m : s.m_molecules)
			Pull(m.first, m.second);

		return *this;
	}

	// molecule
	void	Add(const shared_ptr<MoleculeModel>& model, unsigned int size = 1)
	{
		auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

		if (it == m_molecules.end()) m_molecules.emplace_back(model, size);
		else (*it).second += size;
	}
	void	Pull(const shared_ptr<MoleculeModel>& model, unsigned int size = 1)
	{
		auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

		if (it == m_molecules.end()) throw new exception;
		else
		{
			if (((*it).second -= size) < 0) throw new exception;
			else if ((*it).second == 0) m_molecules.erase(it);
		}
	}
	unsigned int	Num(const shared_ptr<MoleculeModel>& model) const
	{
		auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

		if (it == m_molecules.end()) return 0;
		else return (*it).second;
	}

	const Array<pair<shared_ptr<MoleculeModel>, unsigned int>>&	GetMolecules() const { return m_molecules; }

	bool	IsEmpty() const { return m_molecules.empty(); }

	void	Clear() { m_molecules.clear(); }

	// JSON
	void	Load_this(const ptree& pt)
	{
		// molecules
		for (auto m : pt.get_child("molecules"))
		{
			auto name = m.second.get<string>("molecule");

			const auto& model = g_assetManagerPtr->GetModel<MoleculeModel>(name);

			m_molecules.emplace_back(model, m.second.get<int>("size"));
		}

		Model::Load_this(pt);
	}
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const 
	{
		Model::Save_this(pt);

		// type
		pt.put("type", "Storage");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
};

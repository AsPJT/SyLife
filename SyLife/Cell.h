#pragma once

#include "Model.h"
#include "Storage.h"
#include "Molecule.h"
#include "Part.h"

#include "Body.h"
#include "Equipment.h"
#include "Module.h"

class CellModel
	: public Model
{
public:
	Storage	m_material;
	vector<shared_ptr<PartConfig>>	m_partConfigs;

	// Rigidbody
	double	m_mass;
	double	m_radius;
	double	m_inertia;

public:
	template <typename T>
	vector<shared_ptr<T>>	GetParts() const;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	UpdateProperties();
};

class CellState
	: public Rigidbody
{
public:
	double	m_deathTimer;
	Storage	m_storage;

	shared_ptr<CellModel>	m_model;

	shared_ptr<BodyState>	m_body;
	vector<shared_ptr<EquipmentState>>	m_equipments;
	vector<shared_ptr<ModuleState>>		m_modules;

	vector<shared_ptr<PartState>>		m_partStates;

public:
	CellState(const shared_ptr<CellModel>& model)
		: m_model(model)
		, m_deathTimer(10.0)
	{
		m_mass = m_model->m_mass;
		m_radius = m_model->m_radius;
		m_inertia = m_model->m_inertia;
		
		// parts
		for (const auto& pc : m_model->m_partConfigs)
		{
			const auto& ps = m_partStates.emplace_back(pc->m_model->MakeState());

			ps->m_config = pc;
		}
	}

	void	Update();
	void	Draw();

	// FieldからStorageに受け取る
	void	TakeMolecule(const shared_ptr<MoleculeState>& molecule);

	// StorageからFieldに吐き出す
	void	ExpireMolecule(const shared_ptr<MoleculeModel>& model);
	void	ExpireMolecule(const shared_ptr<MoleculeModel>& model, int size);
};

template <typename T>
inline vector<shared_ptr<T>> CellModel::GetParts() const
{
	vector<shared_ptr<T>> tModels;

	for (auto it = m_partConfigs.begin(); it != m_partConfigs.end(); ++it)
		if (dynamic_pointer_cast<T>(*it) != nullptr) tModels.emplace_back(dynamic_pointer_cast<T>(*it));

	return tModels;
}
#include "Cell.h"
#include "Part.h"
#include "MoleculeManager.h"
#include <numeric>

void CellModel::SetFromJSON(const ptree& pt)
{
	// parts
	for (auto part : pt.get_child("parts")) m_partConfigs.emplace_back(make_shared<PartConfig>())->Load(part.second);

	CalculateDisk();

	Model::SetFromJSON(pt);
}


void CellModel::CalculateDisk()
{
	// mass
	{
		m_mass = 0.0;

		for (const auto& p : m_partConfigs) m_mass += p->m_model->m_mass;
	}

	// center
	{
		// body
		Vector2D center(0.0, 0.0);

		for (const auto& p : m_partConfigs) center += p->m_model->m_mass * (p->m_position + ((p->m_model->m_approximateRect.first + p->m_model->m_approximateRect.second) / 2.0).rotated(p->m_rotation));

		center /= m_mass;

		// 位置調整
		for (const auto& p : m_partConfigs) p->m_position -= center;
	}

	// inertia
	{
		m_inertia = 0.0;

		for (const auto& p : m_partConfigs) m_inertia += p->GetInertia();
	}

	// radius
	m_radius = sqrt(2 * m_inertia / m_mass);
}

void CellModel::CalculateMaterial()
{
}

void CellState::Update()
{

}

void CellState::Draw()
{

}

void CellState::TakeMolecule(const shared_ptr<MoleculeState>& molecule)
{
	m_storage.AddMoleculeState(molecule->m_model);
	m_molecules.AddMoleculeState(molecule->m_model);

	molecule->m_isDestroyed = true;
}

void CellState::ExpireMolecule(const shared_ptr<MoleculeModel>& model)
{
	// 吐き出す方向
	auto v = Vector2D(1.0, 0.0).rotated(rand() / 3600.0);

	// 吐き出されたMoleculeState
	const auto& t = g_moleculeManagerPtr->AddMoleculeState(model);
	t->m_position = m_position + v * (m_radius + model->m_radius);
	t->m_velocity = v * 2.0;

	// Storageから出す
	m_storage.PullMolecule(model);

	// MoleculeStateのカウントを減らす
	m_molecules.PullMolecule(model);
}

void CellState::ExpireMolecule(const shared_ptr<MoleculeModel>& model, int size)
{
	for (int i = 0; i < size; ++i) ExpireMolecule(model);
}

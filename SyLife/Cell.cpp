#include "Cell.h"
#include "Part.h"
#include "MoleculeManager.h"
#include <numeric>

void CellModel::SetFromJSON(const ptree& pt)
{
	// parts
	for (auto part : pt.get_child("parts")) m_partConfigs.emplace_back(make_shared<PartConfig>())->Load(part.second);

	// mass
	m_mass = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double mass, const auto& p) { return mass + p->m_model->m_mass; });

	CalculateDisk();

	Model::SetFromJSON(pt);
}


void CellModel::CalculateDisk()
{
	// center
	{
		// body
		Vector2D center(0.0, 0.0);

		for (const auto& p : m_partConfigs) center += p->m_model->m_mass * (p->m_position + ((p->m_model->m_approximateRect.first + p->m_model->m_approximateRect.second) / 2.0).rotated(p->m_rotation));

		center /= m_mass;

		// �ʒu����
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
	m_material.Clear();

	for (const auto& pc : m_partConfigs)
		m_material.AddStorage(pc->m_model->m_material);
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
	// �f���o������
	auto v = Vector2D(1.0, 0.0).rotated(rand() / 3600.0);

	// �f���o���ꂽMoleculeState
	const auto& t = g_moleculeManagerPtr->AddMoleculeState(model);
	t->m_position = m_position + v * (m_radius + model->GetRadius());
	t->m_velocity = v * 2.0;

	// Storage����o��
	m_storage.PullMolecule(model);

	// MoleculeState�̃J�E���g�����炷
	m_molecules.PullMolecule(model);
}

void CellState::ExpireMolecule(const shared_ptr<MoleculeModel>& model, int size)
{
	for (int i = 0; i < size; ++i) ExpireMolecule(model);
}

#include "Cell.h"
#include "Part.h"
#include "MoleculeManager.h"
#include <numeric>

void Cell::Update()
{

}

void Cell::Draw()
{

}

void Cell::TakeMolecule(const shared_ptr<Molecule>& molecule)
{
	m_storage.AddMolecule(molecule->m_model);
	m_molecules.AddMolecule(molecule->m_model);

	molecule->m_destroyFlag = true;
}

void Cell::ExpireMolecule(const shared_ptr<MoleculeModel>& model)
{
	// �f���o������
	auto v = Vector2D(1.0, 0.0).rotated(rand() / 3600.0);

	// �f���o���ꂽMolecule
	const auto& t = g_moleculeManagerPtr->AddMolecule(model);
	t->m_position = m_position + v * (m_radius + model->m_radius);
	t->m_velocity = v * 2.0;

	// Storage����o��
	m_storage.PullMolecule(model);

	// Molecule�̃J�E���g�����炷
	m_molecules.PullMolecule(model);
}

void Cell::ExpireMolecule(const shared_ptr<MoleculeModel>& model, int size)
{
	for (int i = 0; i < size; ++i) ExpireMolecule(model);
}

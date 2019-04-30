#include "MoleculeManager.h"
#include "Molecule.h"
#include "FieldManager.h"

unique_ptr<MoleculeManager> g_moleculeManager;

MoleculeManager::MoleculeManager()
{
	m_molecules.reserve(10000);
	m_models.reserve(1000);
}


MoleculeManager::~MoleculeManager()
{
}

const shared_ptr<Molecule>& MoleculeManager::AddMolecule(const shared_ptr<Molecule::Model>& model)
{
	const auto& m = m_molecules.emplace_back(make_shared<Molecule>());

	m->m_model = model;
	m->m_radius = model->m_radius;
	m->m_mass = model->m_mass;

	g_fieldManager->m_rigidbodies.emplace_back(m);

	return m;
}

void MoleculeManager::AddMoleculesRandom(const shared_ptr<Molecule::Model>& model, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		const auto& m = g_moleculeManager->AddMolecule(model);
		m->m_position.m_x = rand() % int(g_fieldManager->m_size.m_x);
		m->m_position.m_y = rand() % int(g_fieldManager->m_size.m_y);

		m->Init();
	}
}

const shared_ptr<Molecule::Model>& MoleculeManager::AddModel()
{
	const auto& m = m_models.emplace_back(make_shared<Molecule::Model>());

	return m;
}

void MoleculeManager::Update()
{
	m_molecules.erase(remove_if(m_molecules.begin(), m_molecules.end(), [](const auto& m) { return m->m_destroyFlag; }), m_molecules.end());
}

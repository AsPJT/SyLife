#include "CellManager.h"
#include "Cell.h"
#include "FieldManager.h"
#include "RigidbodySearcher.h"
#include "Molecule.h"
#include "MoleculeManager.h"

unique_ptr<CellManager>	g_cellManager;

CellManager::CellManager()
{
	m_cells.reserve(10000);
}


CellManager::~CellManager()
{
}

const shared_ptr<Cell>& CellManager::AddCell()
{
	const auto& c = m_cells.emplace_back(make_shared<Cell>());

	g_fieldManager->m_rigidbodies.emplace_back(c);

	return c;
}

void CellManager::Update()
{
	for (const auto& c : m_cells)
	{
		// 接触したMoleculeの取り込み
		for (const auto& l : g_rigidbodySearcher->GetNearRigidbodies(c->m_position, c->m_radius * 2.0))
		{
			auto t = g_fieldManager->m_rigidbodies[l.first];
			auto length = (t->m_position - c->m_position).length();

			if (!t->m_destroyFlag && t != c && length - t->m_radius - c->m_radius < 0.0)
			{
				auto m = dynamic_pointer_cast<Molecule>(t);

				if (m != nullptr) c->TakeMolecule(m);
			}
		}

		// 余剰のMoleculeの投棄
		for (auto it = c->m_storage.m_molecules.begin(); it != c->m_storage.m_molecules.end(); ++it)
		{
			if ((*it).second > 10)
			{
				c->ExpireMolecule((*it).first);
				c->m_storage.PullMolecule((*it).first);
				break;
			}
		}

		if (c->m_storage.NumMolecule("Amino acid") < 5 &&
			c->m_storage.NumMolecule("Carbon") > 0 &&
			c->m_storage.NumMolecule("Oxygen") > 0 &&
			c->m_storage.NumMolecule("Nitrogen") > 0)
		{
			c->m_storage.AddMolecule(g_moleculeManager->GetModel("Amino acid"));
			c->m_storage.PullMolecule(g_moleculeManager->GetModel("Carbon"));
			c->m_storage.PullMolecule(g_moleculeManager->GetModel("Oxygen"));
			c->m_storage.PullMolecule(g_moleculeManager->GetModel("Nitrogen"));
		}

		if (c->m_storage.NumMolecule("Amino acid") >= 5 &&
			c->m_storage.NumMolecule("Carbon") >= 5 &&
			c->m_storage.NumMolecule("Oxygen") >= 5)
		{
			c->m_storage.PullMolecule(g_moleculeManager->GetModel("Amino acid"), 5);
			c->m_storage.PullMolecule(g_moleculeManager->GetModel("Carbon"), 5);
			c->m_storage.PullMolecule(g_moleculeManager->GetModel("Oxygen"), 5);

			const auto& nc = g_cellManager->AddCell();

			nc->m_radius = 32.0;
			nc->m_mass = nc->m_radius * nc->m_radius * 1.0;
			nc->m_position = c->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0);
			nc->Init();
		}

		c->m_deathTimer -= g_fieldManager->GetDeltaTime();
		if (c->m_deathTimer <= 0.0)
		{
			for (auto it = c->m_storage.m_molecules.begin(); it != c->m_storage.m_molecules.end(); ++it) c->ExpireMolecule((*it).first, (*it).second);

			c->ExpireMolecule(g_moleculeManager->GetModel("Amino acid"), 5);
			c->ExpireMolecule(g_moleculeManager->GetModel("Carbon"), 5);
			c->ExpireMolecule(g_moleculeManager->GetModel("Oxygen"), 5);

			c->m_destroyFlag = true;
		}
	}

	m_cells.erase(remove_if(m_cells.begin(), m_cells.end(), [](const auto& c) { return c->m_destroyFlag; }), m_cells.end());
}

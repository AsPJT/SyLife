#pragma once

#include "Rigidbody.h"
#include "Storage.h"
#include "Nucleus.h"
#include "Molecule.h"

class Part;

class Cell : public Rigidbody
{
public:
	double	m_deathTimer;
	Nucleus m_nucleus;
	Storage	m_storage;
	Storage	m_molecules;
	vector<shared_ptr<Part>>	m_part;

public:
	Cell();
	~Cell();

	void	Update();
	void	Draw();

	void	TakeMolecule(const shared_ptr<Molecule>& molecule);
	void	ExpireMolecule(const shared_ptr<Molecule::Model>& model) const;
	void	ExpireMolecule(const shared_ptr<Molecule::Model>& model, int size) const;
};


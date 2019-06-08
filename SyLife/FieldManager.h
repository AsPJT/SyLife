#pragma once

#include "Indexer.h"
#include "Rigidbody.h"

class FieldManager
{
public:
	Indexer<Rigidbody>	m_indexer;
	Vector2D	m_size;

public:
	FieldManager() : m_size(800, 600) {}

	void	Init();
	void	Draw();
	void	Update();

	double	GetDeltaTime() const { return 1.0 / 60.0; }
};

extern unique_ptr<FieldManager>	g_fieldManagerPtr;
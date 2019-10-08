﻿#include "SynthesizerState.h"
#include "SynthesizerAsset.h"

#include "PartConfig.h"

#include "Cell.h"

#include "SystemManager.h"

void SynthesizerState::draw(const CellState& cell) const
{
	getPartConfig()->getModel()->draw(min(m_timer / 2.0, 1.0) * 0.75 + 0.25);
}

void SynthesizerState::update(CellState& cell)
{
	m_timer += g_systemManagerPtr->GetDeltaTime();

	auto model = dynamic_pointer_cast<SynthesizerAsset>(getPartConfig()->getModel());
	if (m_timer > 2.0 && cell.m_storage >= model->GetImport() && cell.m_model->getMaterial().numMolecule(model->GetExport()) > cell.m_storage.numMolecule(model->GetExport()))
	{
		m_timer = 0.0;

		cell.m_storage -= model->GetImport();
		cell.m_storage.addMolecule(model->GetExport(), 1);
	}
}
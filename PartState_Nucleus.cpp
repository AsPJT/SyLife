﻿#include "PartState_Nucleus.h"
#include "PartAsset_Nucleus.h"

#include "PartConfig.h"

PartState_Nucleus::PartState_Nucleus(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_partAsset_Nucleus(std::dynamic_pointer_cast<PartAsset_Nucleus>(partConfig->getPartAsset()))
{}

void PartState_Nucleus::update(CellState& cellState)
{
}

﻿#pragma once

#include "PartAsset.h"

class PartAsset_Needle
	: public PartAsset
{
	int		m_penetrating;

public:
	// State
	std::shared_ptr<PartState>	makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;

	int	getPenetrating() const { return m_penetrating; }

	void	preRender(Image& image, double scale, const std::shared_ptr<PartConfig>& partConfig, double a = 0.5) const override {}

	// TypeName
	String	getTypeName() override { return U"PartAsset_Needle"; }
};

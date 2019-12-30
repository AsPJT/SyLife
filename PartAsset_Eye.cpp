﻿#include "PartAsset_Eye.h"
#include "PartState_Eye.h"

shared_ptr<PartState> PartAsset_Eye::makePartState(const shared_ptr<PartConfig>& partConfig) const
{
	return make_shared<PartState_Eye>(partConfig);
}

void PartAsset_Eye::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_maxDistance = json[U"maxDistance"].get<double>();
}

void PartAsset_Eye::save(JSONWriter& json) const
{
	PartAsset::save(json);

	json.key(U"maxDistance").writeDouble(m_maxDistance);
}

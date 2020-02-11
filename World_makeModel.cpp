﻿#include "World.h"

#include "CellAsset.h"
#include "ProteinAsset.h"
#include "Part_BodyAsset.h"
#include "Part_SynthesizerAsset.h"
#include "Part_WingAsset.h"
#include "Part_NeedleAsset.h"
#include "Part_NucleusAsset.h"
#include "Part_EyeAsset.h"

std::shared_ptr<Asset> World::makeAsset(const String& type)
{
	if (type == U"CellAsset")			return makeAsset<CellAsset>();

	if (type == U"ProteinAsset")		return makeAsset<ProteinAsset>();

	if (type == U"Part_BodyAsset")			return makeAsset<Part_BodyAsset>();
	if (type == U"Part_SynthesizerAsset")	return makeAsset<Part_SynthesizerAsset>();
	if (type == U"Part_WingAsset")			return makeAsset<Part_WingAsset>();
	if (type == U"Part_NeedleAsset")		return makeAsset<Part_NeedleAsset>();
	if (type == U"Part_NucleusAsset")		return makeAsset<Part_NucleusAsset>();
	if (type == U"Part_EyeAsset")			return makeAsset<Part_EyeAsset>();

	throw Error(U"存在しないタイプのAssetを生成しようとしました\r Type:" + type);
}

﻿#include "AssetManager.h"

#include "Cell.h"
#include "Molecule.h"
#include "Body.h"
#include "Equipment.h"
#include "Module.h"
#include "Eye.h"
#include "Chloroplast.h"
#include "Synthesizer.h"
#include "Wing.h"
#include "Needle.h"
#include "Hotspot.h"
#include "Terrain.h"

shared_ptr<Model> AssetManager::MakeModel(const string& type)
{
	if (type == "CellModel")		return MakeModel<CellModel>();
	if (type == "MoleculeModel")	return MakeModel<MoleculeModel>();

	if (type == "BodyModel")		return MakeModel<BodyModel>();
	if (type == "EquipmentModel")	return MakeModel<EquipmentModel>();
	if (type == "ModuleModel")		return MakeModel<ModuleModel>();

	if (type == "EyeModel")			return MakeModel<EyeModel>();
	if (type == "ChloroplastModel")	return MakeModel<ChloroplastModel>();
	if (type == "SynthesizerModel")	return MakeModel<SynthesizerModel>();
	if (type == "WingModel")		return MakeModel<WingModel>();
	if (type == "NeedleModel")		return MakeModel<NeedleModel>();

	if (type == "HotspotModel")		return MakeModel<HotspotModel>();
	if (type == "TerrainModel")		return MakeModel<TerrainModel>();

	return nullptr;
}

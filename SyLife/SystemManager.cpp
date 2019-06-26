#include "SystemManager.h"
#include "FieldManager.h"
#include "AssetManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"

unique_ptr<SystemManager>	g_systemManagerPtr;

SystemManager::SystemManager()
{
	g_assetManagerPtr = make_unique<AssetManager>();
	g_cellManagerPtr = make_unique<CellManager>();
	g_fieldManagerPtr = make_unique<FieldManager>();
	g_moleculeManagerPtr = make_unique<MoleculeManager>();

	g_assetManagerPtr->Init();
	g_fieldManagerPtr->Init();
}
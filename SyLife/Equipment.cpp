﻿#include "Equipment.h"
#include "ViewerManager.h"

void EquipmentModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<EquipmentViewer>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> EquipmentModel::MakeState()
{
	return make_shared<EquipmentState>();
}

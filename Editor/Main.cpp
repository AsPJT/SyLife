﻿#include "SystemManager.h"

#include "ViewerManager.h"
#include "AssetViewer.h"

void Main()
{
	g_systemManagerPtr = make_unique<SystemManager>();

	g_viewerManagerPtr->MakeViewer<AssetViewer>();

	g_systemManagerPtr->Start();

	while (System::Update())
	{
		g_systemManagerPtr->Update();
	}
}

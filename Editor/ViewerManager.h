#pragma once

#include "Viewer.h"

class ViewerManager
{
public:
	vector<unique_ptr<Viewer>>	m_viewers;

public:
};

extern unique_ptr<ViewerManager>	g_viewerManagerPtr;

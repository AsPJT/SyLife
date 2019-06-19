#pragma once

#include "Viewer.h"

class PartPaletteViewer :
	public Viewer
{
public:
	PartPaletteViewer()
	{
		m_drawRect = s3d::RectF(450, 0, 150, 600);
	}

	void	Update() override;
};


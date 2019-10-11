﻿#include "ReleaseViewer.h"

#include "ViewerManager.h"

#include "FieldViewer.h"
#include "CellMakingViewer.h"
#include "CellAsset.h"
#include "PartConfig.h"
#include "PartAsset.h"

void ReleaseViewer::update()
{
	auto cmv = g_viewerManagerPtr->getViewer<CellMakingViewer>();
	auto fv = g_viewerManagerPtr->getViewer<FieldViewer>();

	auto t = fv->getCamera().createTransformer();
	Circle circle(Cursor::PosF(), cmv->m_cellAsset->getRadius() * 2.0);

	circle
		.draw(circle.mouseOver() ? Palette::Orange : Palette::Skyblue)
		.drawFrame(4.0, Palette::Black);

	// part
	{
		auto t1 = Transformer2D(Mat3x2::Translate(Cursor::PosF()));

		for (const auto& p : cmv->m_cellAsset->getPartConfigs())
		{
			auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
				.translated(p->getPosition().x, p->getPosition().y));

			for (const auto& s : p->getModel()->getShapes())
				s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
		}
	}
}

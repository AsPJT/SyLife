﻿#include "ReleaseViewer.h"

#include "ViewerManager.h"
#include "CellManager.h"
#include "AssetManager.h"

#include "FieldViewer.h"
#include "CellMakingViewer.h"
#include "CellAsset.h"
#include "CellState.h"
#include "PartConfig.h"
#include "PartAsset.h"

const shared_ptr<CellAsset>& ReleaseViewer::getCellAsset() const
{
	return g_viewerManagerPtr->getViewer<CellMakingViewer>()->m_cellAsset;
}

ReleaseViewer::ReleaseViewer()
{
	setPriority(3);
	setDrawRect(Scene::Rect());
	setBackgroundColor(Color(0, 0));
}

void ReleaseViewer::update()
{
	if (isInvisible()) return;

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

			for (const auto& l : p->getModel()->getShape())
				l.m_polygon.draw(ColorF(l.m_color, 0.5)).drawFrame(1.0, Palette::Black);
		}
	}

	// Release
	if (MouseL.up())
	{
		// CellAssetのリセット

		// 新規Cell
		const auto& c = g_cellManagerPtr->addCellState(cmv->m_cellAsset);
		c->setPosition(Cursor::PosF());
		c->setVelocity(Vec2::Zero());
		c->init();

		// CMVのリスタート
		cmv->makeAsset();
		cmv->setMode(CellMakingViewer::Mode::Close);

		return;
	}
}

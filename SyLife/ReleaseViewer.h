﻿#pragma once

#include "Viewer.h"
#include "PartAsset.h"
#include "Cell.h"
#include "ModelEditor.h"

class ReleaseViewer
	: public ModelEditor
{
public:
	bool	m_isDragged;

public:
	ReleaseViewer()
	{
		SetDrawRect(Scene::Width() - 400, 320, 300, 100);
	}

	void	update() override
	{
		Circle circle(50, 50, 45);

		circle
			.draw(circle.mouseOver() ? Palette::Orange : Palette::Skyblue)
			.drawFrame(4.0, Palette::Black);

		if (m_isDragged) circle.draw(Palette::Red);

		if (circle.leftClicked()) m_isDragged = true;
		if (!MouseL.pressed()) m_isDragged = false;

		// part
		{
			auto t1 = Transformer2D(Mat3x2::Scale(45.0 / getModel<CellAsset>()->getRadius() / 2.0).translated(circle.center));

			for (const auto& p : getModel<CellAsset>()->GetPartConfigs())
			{
				auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
					.translated(p->getPosition().x, p->getPosition().y));

				for (const auto& s : p->getModel()->getShapes())
					s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
			}
		}
	}
};


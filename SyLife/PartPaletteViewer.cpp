﻿#include "PartPaletteViewer.h"
#include "PartAsset.h"
#include "BodyAsset.h"
#include "EquipmentAsset.h"
#include "ModuleAsset.h"

void	PartPaletteViewer::update()
{
	Rect(64, 0, 16, 400).stretched(-2).drawFrame(1.0, Palette::White);

	// Bar
	{
		static bool barPressed = false;
		static double barDelta = 0.0;
		static double barPosition = 0.0;
		auto r = Rect(64, int(Math::Lerp<int>(0, 400 - 48, m_bar)), 16, 48).stretched(-4);

		if (r.leftClicked())
		{
			barPressed = true;
			barDelta = 0.0;
			barPosition = m_bar * double(400 - 48);
		}

		if (!MouseL.pressed()) barPressed = false;

		if (barPressed)
		{
			barDelta += Cursor::DeltaF().y;

			m_bar = Clamp<double>((barPosition + barDelta) / double(400 - 48), 0.0, 1.0);
		}
		else if (IsMouseOver())
		{
			m_bar = Clamp<double>(m_bar + Mouse::Wheel() * 0.05, 0.0, 1.0);
		}

		r.draw(ColorF(1.0, r.mouseOver() ? 0.5 : 0.25));
	}

	{
		const double h = 32 * 3 - 16 + g_assetManagerPtr->getModels<PartAsset>().size() * 64 - 400.0;

		MovedrawPos(0, -h * m_bar);

		drawModels<BodyAsset>("BodyAsset.h");
		drawModels<EquipmentAsset>("EquipmentAsset");
		drawModels<ModuleAsset>("ModuleAsset");
	}
}
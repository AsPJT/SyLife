﻿#include "MainViewer.h"
#include "CellAsset.h"
#include "ProteinAsset.h"
#include "PartAsset_Synthesizer.h"
#include "PartAsset_Nucleus.h"
#include "PartAsset_Needle.h"
#include "PartConfig.h"
#include "GUIText.h"
#include "GUITextBox.h"
#include "GUIButton.h"

MainViewer::CellAssetViewer::CellAssetViewer(const shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
{
}

void MainViewer::CellAssetViewer::init()
{
	setName(m_cellAsset->getName());
	setViewerSize(500, 600);

	// name
	addChildViewer<GUIText>(m_cellAsset->getNameJP(), Font(24, Typeface::Heavy), GUIText::Mode::DrawLeftCenter)
		->mouseoverDisable()
		->setViewerRectInLocal(5, 5, 400, 40);

	// close
	addChildViewer<GUIButton>(U"✖", [this]() { destroy(); })
		->setViewerRectInLocal(450, 5, 40, 40);
}

void MainViewer::CellAssetViewer::update()
{
	DraggableViewer::update();

	RectF(getViewerSize()).rounded(5.0).draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

	// 数の推移
	{
		setDrawPos(10, 50);

		RectF rect(480, 80);
		const int scale = 60;

		rect.draw(Color(11, 22, 33)).drawFrame(1.0, 0.0, Palette::Black);

		rect = rect.stretched(-5);

		double max1 = getMax(rect, scale, [](const auto& status) { return status.m_numCell; });
		double max2 = getMax(rect, scale, [](const auto& status) { return status.m_numEgg; });

		drawGraph(rect, Palette::Green, Max(max1, max2), scale, [](const auto& status) { return status.m_numEgg; });
		drawGraph(rect, Palette::Red, Max(max1, max2), scale, [](const auto& status) { return status.m_numCell; });
	}

	// アイコン
	{
		setDrawPos(5, 140);
		const double size = 200;

		Circle(size / 2.0, size / 2.0, size / 2.0)
			.draw(ColorF(0.2))
			.drawFrame(2.0, 0.0, Palette::Black);

		auto t = Transformer2D(Mat3x2::Scale(0.9 * size / 2.0 / m_cellAsset->getDrawRadius()).translated(size / 2.0, size / 2.0));
		m_cellAsset->draw();
	}

	// 性能
	{
		setDrawPos(210, 140);

		static Font font(16, Typeface::Default);

		// Proteins
		font(U"この生き物を作るのに必要なもの").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, 20);
		{
			moveDrawPos(8, 0);

			font(U"エレメント(緑色で広がっているもの)").draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);
			{
				moveDrawPos(8, 0);
				font(ToString(int(m_cellAsset->getMaterial().getElement())) + U"elm").draw(Vec2::Zero(), Palette::Black);
				moveDrawPos(0, 30);
				moveDrawPos(-8, 0);
			}

			font(U"プロテイン(合成器官で出来るもの)").draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);
			{
				moveDrawPos(8, 0);
				for (const auto& protein : m_cellAsset->getMaterial().getProteinList())
				{
					bool canMakeSelf = false;
					for (const auto& partConfig : m_cellAsset->getPartConfigs())
						if (auto synthesizer = dynamic_pointer_cast<PartAsset_Synthesizer>(partConfig->getPartAsset()))
							if (synthesizer->getExport() == protein.first) { canMakeSelf = true; break; }

					if (canMakeSelf) font(protein.first->getNameJP() + U": " + ToString(protein.second) + U"個" + U"(自分で作れます)").draw(Vec2::Zero(), Palette::Black);
					else font(protein.first->getNameJP() + U": " + ToString(protein.second) + U"個" + U"(自分で作れません)").draw(Vec2::Zero(), Palette::Red);

					moveDrawPos(0, 20);
				}
				moveDrawPos(0, 10);
				moveDrawPos(-8, 0);
			}

			moveDrawPos(-8, 0);
		}

		font(U"この生き物が作れるプロテイン").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, 20);
		{
			moveDrawPos(8, 0);

			for (const auto& partConfig : m_cellAsset->getPartConfigs())
			{
				if (auto synthesizer = dynamic_pointer_cast<PartAsset_Synthesizer>(partConfig->getPartAsset()))
				{
					font(synthesizer->getExport()->getNameJP(), int(synthesizer->getProductTime()), U"秒ごとに").draw(Vec2::Zero(), Palette::Black);
					moveDrawPos(0, 20);
				}
			}
			moveDrawPos(0, 10);

			moveDrawPos(-8, 0);
		}

		font(U"この生き物の特性").draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, 20);
		{
			moveDrawPos(8, 0);

			font(U"孵化までの時間", int(m_cellAsset->getNucleusAsset()->getBornTime()), U"秒").draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);

			font(U"産卵までの時間", int(m_cellAsset->getNucleusAsset()->getYieldTime()), U"秒").draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);

			font(U"寿命:", int(m_cellAsset->getNucleusAsset()->getLifespanTime()), U"秒").draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);

			font(U"硬さ:", int(m_cellAsset->getNucleusAsset()->getArmor())).draw(Vec2::Zero(), Palette::Black);
			moveDrawPos(0, 20);

			{
				int penetrating = 0;

				for (const auto& partConfig : m_cellAsset->getPartConfigs())
					if (auto needle = dynamic_pointer_cast<PartAsset_Needle>(partConfig->getPartAsset()))
						penetrating = Max(penetrating, needle->getPenetrating());

				font(U"トゲの貫通力:", penetrating).draw(Vec2::Zero(), Palette::Black);
				moveDrawPos(0, 20);
			}

			moveDrawPos(-8, 0);
		}
	}
}

double MainViewer::CellAssetViewer::getMax(const RectF& rect, int scale, std::function<double(const CellAsset::Log::Status&)> function) const
{
	double max = 0.0;

	// 最大値
	for (int i = 0; i < rect.w; ++i)
	{
		if (m_cellAsset->m_log.m_statuses.size() < (i + 1) * scale) break;

		max = Max(max, function(*(m_cellAsset->m_log.m_statuses.end() - (i + 1) * scale)));
	}

	return max;
}

void MainViewer::CellAssetViewer::drawGraph(const RectF& rect, const Color& color, double max, int scale, std::function<double(const CellAsset::Log::Status&)> function) const
{
	auto t = Transformer2D(Mat3x2::Translate(rect.pos));

	// 描画
	for (int i = 0; i < rect.w - 1; ++i)
	{
		if (m_cellAsset->m_log.m_statuses.size() < (i + 2) * scale) break;

		double v1 = rect.h * (1.0 - function(*(m_cellAsset->m_log.m_statuses.end() - (i + 1) * scale)) / max);
		double v2 = rect.h * (1.0 - function(*(m_cellAsset->m_log.m_statuses.end() - (i + 2) * scale)) / max);
		Line(rect.w - i, v1, rect.w - i - 1, v2).draw(2.0, color);
	}
}

#include "AssetViewer.h"
#include "AssetManager.h"
#include "AddNewModelViewer.h"
#include "Cell.h"
#include "Molecule.h"
#include "Part.h"
#include "Body.h"
#include "Equipment.h"
#include "Module.h"

void AssetViewer::Update()
{
	m_drawPos = s3d::Vec2::Zero();

	DrawModels<CellModel>("CellModel");
	DrawModels<MoleculeModel>("MoleculeModel");
	DrawModels<BodyModel>("BodyModel");
	DrawModels<EquipmentModel>("EquipmentModel");
	DrawModels<ModuleModel>("ModuleModel");

	// AddNewModel
	{
		static s3d::Font font(15, s3d::Typeface::Bold);

		auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(m_drawPos), true);
		auto f = font(U"新しいModelの追加");

		f.region().draw(s3d::ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
		f.draw();

		if (f.region().leftClicked() && !g_viewerManagerPtr->HasViewer<AddNewModelViewer>()) g_viewerManagerPtr->AddViewer<AddNewModelViewer>();

		m_drawPos.moveBy(0.0, 16.0);
	}

	// Save
	if (s3d::KeyControl.pressed() && s3d::KeyS.down() && m_selectedModel != nullptr)
	{
		ptree pt;
		m_selectedModel->Save(pt);
		write_json(m_selectedModel->GetFilepath(), pt);
	}
}

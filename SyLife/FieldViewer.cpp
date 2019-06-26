#include "FieldViewer.h"
#include "FieldManager.h"
#include "Curtain.h"
#include "CellManager.h"

#include "ViewerManager.h"
#include "ReleaseViewer.h"
#include "AssemblyViewer.h"
#include "PartPaletteViewer.h"

void FieldViewer::Init()
{
	m_newModel = make_shared<CellModel>();

	m_partPaletteViewer = g_viewerManagerPtr->AddViewer<PartPaletteViewer>(m_newModel);
	m_assemblyViewer = g_viewerManagerPtr->AddViewer<AssemblyViewer>(m_newModel);
	m_releaseViewer = g_viewerManagerPtr->AddViewer<ReleaseViewer>(m_newModel);
}

void FieldViewer::Update(bool isMouseOver)
{
	if (isMouseOver) m_cursorCamera2D.update();
	auto t = m_cursorCamera2D.createTransformer();

	auto f = s3d::KeySpace.pressed() ? 5 : 1;

	for (int i = 0; i < f; ++i)
		g_fieldManagerPtr->Update();

	g_fieldManagerPtr->Draw();

	if (m_releaseViewer->m_isDragged && isMouseOver)
	{
		// part
		{
			auto t1 = s3d::Transformer2D(s3d::Mat3x2::Translate(s3d::Cursor::PosF()));

			for (const auto& p : m_releaseViewer->m_model->m_partConfigs)
			{
				auto t2 = s3d::Transformer2D(s3d::Mat3x2::Rotate(p->m_rotation).translated(p->m_position.m_x, p->m_position.m_y));

				for (const auto& s : p->m_model->m_shapes)
					s.m_polygon.draw(s3d::ColorF(s.m_color, 0.5)).drawFrame(1.0, s3d::Palette::Black);
			}
		}

		if (s3d::MouseL.up())
		{
			const auto& c = g_cellManagerPtr->AddCellState(m_releaseViewer->m_model);
			c->m_molecules.AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Amino Acid"), 5);
			c->m_molecules.AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Carbon"), 5);
			c->m_molecules.AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Oxygen"), 5);
			c->m_position.m_x = s3d::Cursor::PosF().x;
			c->m_position.m_y = s3d::Cursor::PosF().y;
			c->m_velocity = Vector2D::Zero();
			c->Init();

			m_newModel = make_shared<CellModel>();
			m_partPaletteViewer->m_model = m_newModel;
			m_assemblyViewer->m_model = m_newModel;
			m_releaseViewer->m_model = m_newModel;
		}
	}

	static Curtain curtain(s3d::Color(11, 22, 33), 0.5);
	curtain.OpenUpdate();
	m_audio.setVolume(s3d::Min(curtain.m_st.sF() / curtain.m_duration, 1.0));
}

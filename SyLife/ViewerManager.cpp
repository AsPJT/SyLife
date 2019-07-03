#include "Viewer.h"

unique_ptr<ViewerManager>	g_viewerManagerPtr;

void ViewerManager::Update()
{
	// mouseOver ���Z�b�g
	for (auto it = m_viewers.begin(); it < m_viewers.end(); ++it)
		(*it)->m_isMouseOver = false;

	// mouseOver �K�p
	for (auto it = m_viewers.rbegin(); it < m_viewers.rend(); ++it)
	{
		if ((*it)->m_drawRect.mouseOver())
		{
			(*it)->m_isMouseOver = true;

			break;
		}
	}

	// Viewer �X�V
	for (auto it = m_viewers.begin(); it < m_viewers.end(); ++it)
	{
		auto vp = ViewportBlock2D(Rect((*it)->m_drawRect));
		auto t = Transformer2D(Mat3x2::Identity(), Mat3x2::Translate((*it)->m_drawRect.pos));

		RectF((*it)->m_drawRect.size).draw((*it)->m_backgroundColor);

		(*it)->Update();

		RectF((*it)->m_drawRect.size).drawFrame(1.0, 0.0, ColorF(Palette::Red, 0.5));
	}
}

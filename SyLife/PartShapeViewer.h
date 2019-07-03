#pragma once

#include "ModelViewer.h"
#include "ShapeModel.h"
#include "TinyCamera.h"
#include "Part.h"

class PartShapeViewer
	: public ModelViewer
{
public:
	shared_ptr<PartModel>	m_model;

	Array<Vec2>	m_verticles;

	Vec2*		m_selectedVerticle = nullptr;

	CursorCamera2D	m_camera;

public:
	PartShapeViewer(const shared_ptr<PartModel>& model)
		: m_model(model)
	{
		SetDrawRect(0, 0, 600, 450);

		m_camera.setTargetCenter(Window::BaseSize() / 2 - GetDrawRect().size / 2);
		m_camera.setCenter(Window::BaseSize() / 2 - GetDrawRect().size / 2);
	}

	void	Init() override;
	void	Update() override;
};


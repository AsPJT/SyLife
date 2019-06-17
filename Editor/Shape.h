#pragma once

#include "Model.h"

class Shape
	: public Model
{
public:
	s3d::HSV	m_color;
	s3d::Array<s3d::Vec2>	m_verticles;

public:
	Shape(const s3d::Array<s3d::Vec2>& verticles)
		: m_verticles(verticles)
		, m_color(s3d::RandomHSV())
	{}
	Shape()
		: m_verticles()
		, m_color(s3d::RandomHSV())
	{}

	ptree	ToJSON() const override;
	void	Load(const string& filepath) override;
	void	Save() const override;
};


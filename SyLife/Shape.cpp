﻿#include "stdafx.h"
#include "Shape.h"

bool Shape::updateProperties()
{
	// 初期化
	m_polygon = Polygon();

	Array<Polygon> copies;
	for (const auto& l : *this) 
	{
		if (l.m_polygon.isEmpty()) return false;

		copies.emplace_back(l.m_polygon);
	}

	Polygon result;
	while (!copies.empty())
	{
		bool flag = true;

		for (auto it = copies.begin(); it != copies.end(); ++it)
		{
			auto polygons = Geometry2D::Or(result, *it);

			if (polygons.size() == 1)
			{
				result = polygons.front();
				copies.erase(it);

				flag = false;

				break;
			}
		}

		if (flag) return false;
	}

	m_polygon = result;

	return true;
}

void Shape::load_this(const ptree& pt)
{
	// layers
	for (auto m : pt.get_child("layers"))
	{
		emplace_back().load(m.second);
	}
}

void Shape::save_this(ptree& pt) const
{
	// layers
	{
		ptree layers;

		for (const auto& m : *this)
		{
			ptree pt2;
			m.save(pt2);

			layers.push_back(std::make_pair("", pt2));
		}

		pt.add_child("layers", layers);
	}
}

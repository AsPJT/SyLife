#pragma once

#include "Model.h"
#include "EquipmentModel.h"

class EquipmentConfig
	: public Model
{
public:
	shared_ptr<EquipmentModel>	m_model;
	s3d::Vec2	m_position;
	double		m_rotation;

public:
	EquipmentConfig(const shared_ptr<EquipmentModel>& model)
		: m_model(model)
	{}

	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) override;
};


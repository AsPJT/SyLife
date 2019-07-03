#pragma once

#include "ModelViewer.h"
#include "Storage.h"
#include "ShapeModel.h"
#include "PartShapeViewer.h"

class CellState;
class PartConfig;
class PartState;
class MoleculeModel;

class PartModel
	: public Model
{
public:
	friend class PartViewer;

	double	m_mass;
	Storage	m_material;
	vector<ShapeModel>	m_shapes;

public:
	// Get
	double	GetMass() const { return m_mass; }
	const Storage&	GetMaterial() const { return m_material; }
	const vector<ShapeModel>&	GetShapes() const { return m_shapes; }

	void MakeViewer() override;
	virtual shared_ptr<PartState>	MakeState();

	void	Draw(double a = 0.5) { for (const auto& s : m_shapes) s.Draw(a); }

	Vec2	GetApproximateRectTopLeft() const
	{
		double x = m_shapes.front().m_polygon.vertices().front().x;
		double y = m_shapes.front().m_polygon.vertices().front().y;

		for (const auto& s : m_shapes)
		{
			for (const auto& v : s.m_polygon.vertices())
			{
				if (x > v.x) x = v.x;
				if (y > v.y) y = v.y;
			}
		}

		return Vec2(x, y);
	}
	Vec2	GetApproximateRectBottomDown() const
	{
		double x = m_shapes.front().m_polygon.vertices().front().x;
		double y = m_shapes.front().m_polygon.vertices().front().y;

		for (const auto& s : m_shapes)
		{
			for (const auto& v : s.m_polygon.vertices())
			{
				if (x < v.x) x = v.x;
				if (y < v.y) y = v.y;
			}
		}

		return Vec2(x, y);
	}
	double	GetRectInertia() const
	{
		auto w = (GetApproximateRectBottomDown() - GetApproximateRectTopLeft()).x;
		auto h = (GetApproximateRectBottomDown() - GetApproximateRectTopLeft()).y;

		return  m_mass * (w * w + h * h) / 12.0;
	}
	Vec2	GetCenter() const { return (GetApproximateRectTopLeft() + GetApproximateRectBottomDown()) / 2.0; }

	// JSON
	void	SetFromJSON(const ptree& pt)
	{
		// mass
		m_mass = pt.get<double>("mass");

		// shapes
		for (auto shape : pt.get_child("shapes"))
			m_shapes.emplace_back().SetFromJSON(shape.second);

		// material
		m_material.Load(pt.get_child("material"));

		Model::SetFromJSON(pt);
	}
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const
	{
		// mass
		pt.put<double>("mass", m_mass);

		// shapes
		{
			ptree shapes;

			for (const auto& v : m_shapes)
			{
				ptree shape; v.Save(shape);
				shapes.push_back(std::make_pair("", shape));
			}

			pt.add_child("shapes", shapes);
		}

		// material
		{
			ptree material;

			m_material.Save(material);

			pt.add_child("material", material);
		}

		Model::AddToJSON(pt);

		pt.put("type", "PartModel");
	}
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

class PartConfig
	: public Model
{
	shared_ptr<PartModel>	m_partModel;
	Vec2	m_position;
	double		m_rotation;

public:
	// Get
	const shared_ptr<PartModel>&	GetModel() const { return m_partModel; }
	const Vec2&	GetPosition() const { return m_position; }
	double	GetRotation() const { return m_rotation; }

	// Set
	void	SetModel(const shared_ptr<PartModel>& partModel) { m_partModel = partModel; }
	void	SetPosition(const Vec2& position) { m_position = position; }
	void	SetRotation(double rotation) { m_rotation = rotation; }

	double	GetInertia() const { return m_partModel->GetRectInertia() + (m_position + m_partModel->GetCenter().rotated(m_rotation)).lengthSq() * m_partModel->GetMass(); }

	void	SetFromJSON(const ptree& pt)
	{
		// model
		m_partModel = g_assetManagerPtr->GetModel<PartModel>(pt.get<string>("model"));

		// position
		m_position = Vec2(pt.get<double>("position.x"), pt.get<double>("position.y"));

		// rotation
		m_rotation = pt.get<double>("rotation");

		Model::SetFromJSON(pt);
	}
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const
	{
		// model
		pt.put("model", GetModel()->GetName());

		// position
		{
			ptree position;

			position.put("x", m_position.x);
			position.put("y", m_position.y);

			pt.push_back(std::make_pair("position", position));
		}

		// rotation
		pt.put("rotation", m_rotation);

		Model::AddToJSON(pt);

		pt.put("type", "PartConfig");
	}
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

class PartState
{
	shared_ptr<PartConfig>	m_partConfig;

public:
	virtual ~PartState() {}

	// Get
	const shared_ptr<PartConfig>&	GetPartConfig() const { return m_partConfig; }

	// Set
	void	SetPartConfig(const shared_ptr<PartConfig>& partConfig) { m_partConfig = partConfig; }

	virtual void	Draw(const CellState& cell) const {}
	virtual void	Update(CellState& cell) {}
};

class PartViewer
	: public ModelViewer
{
	TextEditState		m_textEditState_mass;

public:
	PartViewer()
	{
		SetDrawRect(0, 450, 600, 150);
	}

	void Update() override
	{
		// mass
		/*{
			SimpleGUI::TextBox(m_textEditState_mass, Vec2(10, 50), 240);
			m_model->m_mass = Parse<double>(m_textEditState_mass.text);
		}*/
	}
};

inline shared_ptr<PartState> PartModel::MakeState() { return make_shared<PartState>(); }
inline void PartModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<PartViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

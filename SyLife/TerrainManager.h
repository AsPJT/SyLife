#pragma once

class TerrainModel;
class HotspotState;

// �n�`�̕ۑ��ƕ`����s���B
class TerrainManager
{
	shared_ptr<TerrainModel> m_terrainModel;

	// ������������
	Texture	m_texture;
	PerlinNoise	m_noise;
	Array<shared_ptr<HotspotState>>	m_hotspots;
	Array<MultiPolygon>	m_polygons;

public:

	double	GetHeight(const Vec2& position) const;
	Color	GetColor(const Vec2& position);

	void	MakeTexture(int textureSize = 2048);
	void	MakeHotspots(int numHotspots = 300);

	void	SetTerrainModel(const shared_ptr<TerrainModel>& model);
	const shared_ptr<TerrainModel>&	GetTerrainModel() { return m_terrainModel; }

	void	UpdateTerrain();
	void	DrawTerrain();
};

extern unique_ptr<TerrainManager>	g_terrainManagerPtr;

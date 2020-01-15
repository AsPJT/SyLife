﻿#include "Particle.h"
#include "TileState.h"
#include "World.h"

void Particle::addForce(const Vec2& force)
{
	m_velocity += force / m_mass * DeltaTime;
}

void Particle::updateParticle()
{
	// 並進運動
	m_position += m_velocity * DeltaTime;

	// 壁
	if (m_position.x < 0) m_position.x = 0;
	if (m_position.y < 0) m_position.y = 0;
	if (m_position.x > World::GetInstance()->getFieldSize().x) m_position.x = World::GetInstance()->getFieldSize().x;
	if (m_position.y > World::GetInstance()->getFieldSize().y) m_position.y = World::GetInstance()->getFieldSize().y;

	// 水流
	{
		const double scale = 100.0;
		const auto waveVelocity = scale * World::GetInstance()->getTile(m_position).getWaveVelocity();

		m_velocity = waveVelocity + (m_velocity - waveVelocity) * 0.95;
	}
}

void Particle::load(Deserializer<ByteArray>& reader)
{
	reader >> m_mass;
	reader >> m_position;
	reader >> m_velocity;
}

void Particle::save(Serializer<MemoryWriter>& writer) const
{
	writer << m_mass;
	writer << m_position;
	writer << m_velocity;
}

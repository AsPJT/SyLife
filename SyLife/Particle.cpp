﻿#include "Particle.h"

#include "SystemManager.h"
#include "WaveManager.h"

void Particle::addForce(const Vec2& force)
{
	m_velocity += force / m_mass * g_systemManagerPtr->GetDeltaTime();
}

void Particle::updateParticle()
{
	// 並進運動
	m_position += m_velocity * g_systemManagerPtr->GetDeltaTime();

	if (KeyO.pressed())
	{
		m_velocity *= 0.95;
	}
	else
	{
		// 摩擦抵抗
		auto waveVelocity = g_waveManagerPtr->getWaveVelocity(m_position);
		m_velocity = waveVelocity + (m_velocity - waveVelocity) * 0.95;
	}
}
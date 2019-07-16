#include "Particle.h"

void Particle::UpdateParticle()
{
	// ���i�^��
	m_position += m_velocity * g_systemManagerPtr->GetDeltaTime();

	if (KeyO.pressed())
	{
		m_velocity *= 0.95;
	}
	else
	{
		// ���C��R
		auto waveVelocity = g_waveManagerPtr->GetWaveVelocity(m_position);
		m_velocity = waveVelocity + (m_velocity - waveVelocity) * 0.95;
	}
}
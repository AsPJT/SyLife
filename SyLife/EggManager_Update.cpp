#include "EggManager.h"
#include "FieldManager.h"
#include "AssetManager.h"
#include "WaveManager.h"
#include "CellManager.h"
#include "Cell.h"
#include "Egg.h"

void EggManager::Update()
{
	for (const auto& e : GetEggStates())
	{
		// Update
		e->Update();

		// ���i�^��
		e->m_position += e->m_velocity;

		// ��]�^��
		e->m_rotation += e->m_angularVelocity;

		// �ǂł̔���
		if (e->m_position.m_x < 0 && e->m_velocity.m_x < 0) e->m_velocity.m_x = -e->m_velocity.m_x;
		if (e->m_position.m_y < 0 && e->m_velocity.m_y < 0) e->m_velocity.m_y = -e->m_velocity.m_y;
		if (e->m_position.m_x > g_fieldManagerPtr->GetSize().m_x && e->m_velocity.m_x > 0) e->m_velocity.m_x = -e->m_velocity.m_x;
		if (e->m_position.m_y > g_fieldManagerPtr->GetSize().m_y && e->m_velocity.m_y > 0) e->m_velocity.m_y = -e->m_velocity.m_y;

		// ���C��R
		{
			auto waveVelocity = g_waveManagerPtr->GetWaveVelocity(e->m_position);

			e->m_velocity = waveVelocity + (e->m_velocity - waveVelocity) / (1.0 + e->m_radius * 0.01);
			e->m_angularVelocity /= (1.0 + e->m_radius * 0.01);
		}
	}

	GetEggStates().erase(remove_if(GetEggStates().begin(), GetEggStates().end(), [](const auto& e) { return e->m_isDestroyed; }), GetEggStates().end());
	m_indexer.Update();
}

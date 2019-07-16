#include "Rigidbody.h"

void Rigidbody::AddImpulseInWorld(const Vec2& impulse, const Vec2& worldPosition)
{
	// �^����
	const Vec2 momentum = GetVelocity() * GetMass() + impulse;

	// ���x
	SetVelocity(momentum / GetMass());

	// �͂̃��[�����g
	const double momentOfForce = (worldPosition - GetPosition()).cross(impulse);

	// �p�^����
	const double angularMomentum = momentOfForce + m_inertia * m_angularVelocity;

	// �p���x
	m_angularVelocity = angularMomentum / m_inertia;
}

void Rigidbody::UpdateRigidbody()
{
	// ��]�^��
	m_rotation += m_angularVelocity * g_systemManagerPtr->GetDeltaTime();

	// ���C��R
	m_angularVelocity *= 0.95;
}

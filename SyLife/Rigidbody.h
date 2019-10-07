﻿#pragma once

#include "Particle.h"

class Rigidbody
	: public Particle
{
	double	m_radius = 0.0;
	double	m_rotation = 0.0;
	double	m_inertia = 0.0;
	double	m_angularVelocity = 0.0;

public:
	virtual ~Rigidbody() = default;

	// Get
	double	getRadius() const { return m_radius; }
	double	getRotation() const { return m_rotation; }
	double	getInertia() const { return m_inertia; }
	double	GetAngularVelocity() const { return m_angularVelocity; }

	// Set
	void	SetRadius(double radius) { m_radius = radius; }
	void	setRotation(double rotation) { m_rotation = rotation; }
	void	SetInertia(double inertia) { m_inertia = inertia; }
	void	SetAngularVelocity(double angularVelocity) { m_angularVelocity = angularVelocity; }

	void	Init()
	{
		// 角運動量の計算
		m_inertia = getMass() * m_radius * m_radius * 0.5;
	}

	Vec2	GetVelocityAt(const Vec2& position) const { return GetVelocity() + (position - getPosition()).rotated(m_rotation) * m_angularVelocity; }
	Vec2	GetWorldPosition(const Vec2& localPosition) const { return getPosition() + localPosition.rotated(m_rotation); }

	void	AddForceInWorld(const Vec2& force, const Vec2& worldPosition);
	void	AddForceInLocal(const Vec2& force, const Vec2& localPosition) { AddForceInWorld(force.rotated(m_rotation), GetWorldPosition(localPosition)); }
	void	AddImpulseInLocal(const Vec2& impulse, const Vec2& localPosition) { AddImpulseInWorld(impulse.rotated(m_rotation), GetWorldPosition(localPosition)); }
	void	AddImpulseInWorld(const Vec2& impulse, const Vec2& worldPosition);

	void	UpdateRigidbody();
};

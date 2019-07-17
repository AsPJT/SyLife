#pragma once

// Engine�̋��ʂ��Ă����Ղ̋��L
class SystemManager
{
public:
	SystemManager();

	constexpr double	GetDeltaTime() const { return 1.0 / 60.0; }

	void	Update();
};

extern unique_ptr<SystemManager>	g_systemManagerPtr;

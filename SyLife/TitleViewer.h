#pragma once

#include "Viewer.h"

#include "FieldViewer.h"
#include "ViewerManager.h"
#include "Curtain.h"

class TitleViewer
	: public Viewer
{
	class Bubble
	{
	public:
		s3d::Vec3	m_position;
		double		m_timer = 0.0;
	};

	s3d::Array<Bubble>	m_bubbles;
	s3d::Audio m_audio;

public:
	TitleViewer()
		: m_audio(U"assets/music/神秘の世界.mp3")
	{
		m_drawRect = s3d::RectF(s3d::Window::Size());
		m_audio.setLoop(true);
		m_audio.play();
	}

	void	UpdateBubbles()
	{
		if (s3d::RandomBool(0.1))
		{
			auto& b = m_bubbles.emplace_back();

			b.m_position = s3d::Vec3(120.0 * s3d::Random(-1.0, 1.0), -100.0, 200.0 + 120.0*s3d::Random(-1.0, 1.0));
		}

		for (auto& b : m_bubbles)
		{
			b.m_timer += 1.0;
			b.m_position.x += 0.05 * s3d::Random(-1.0, 1.0);
			b.m_position.y += 0.15 * s3d::Random(0.25, 1.0);
			b.m_position.z += 0.05 * s3d::Random(-1.0, 1.0);
		}

		m_bubbles.remove_if([](const auto& b) { return b.m_timer > 1800.0; });
	}

	void	DrawBubbles()
	{
		for (auto& b : m_bubbles)
		{
			static double t = 0.0;
			t += 1.0;
			auto s = 5.0;
			s3d::Vec3 camPos(sin(t * 0.00001 * 11) * s, sin(t * 0.00001 * 13) * s, sin(t * 0.00001 * 17) * s);
			s3d::Vec3 p = b.m_position - camPos;

			auto x = (asin(p.x / p.z) / (3.14 / 3.0) + 0.5) * s3d::Window::Size().x;
			auto y = (-asin(p.y / p.z) / (3.14 / 3.0) + 0.5) * s3d::Window::Size().y;
			auto r = 2000.0 / p.length() * s3d::Min(b.m_timer / 1000.0, 1.0);
			auto a = s3d::Min((1800.0 - b.m_timer) / 500.0, 1.0) * 0.1;

			s3d::Circle(x + r * 0.15, y - r * 0.55, r * 0.3).draw(s3d::ColorF(s3d::Palette::Lightblue, a));
			s3d::Circle(x + r * 0.55, y - r * 0.15, r * 0.3).draw(s3d::ColorF(s3d::Palette::Lightblue, a));
			s3d::Circle(x + r * 0.40, y - r * 0.40, r * 0.3).draw(s3d::ColorF(s3d::Palette::Lightblue, a));

			s3d::Circle(x, y, r * 0.3).draw(s3d::ColorF(s3d::Palette::Lightblue, a));
			s3d::Circle(x, y, r * 0.6).draw(s3d::ColorF(s3d::Palette::Lightblue, a));
			s3d::Circle(x, y, r * 1.0).draw(s3d::ColorF(s3d::Palette::Lightblue, a));
		}
	}

	void	Init() override
	{
		for (int i = 0; i < 2000; ++i) UpdateBubbles();
	}

	void	Update(bool isMouseOver) override
	{
		// bubbles
		{
			for (int i = 0; i < 3; ++i) UpdateBubbles();

			DrawBubbles();
		}

		// title
		{
			static s3d::Font titleFont(256, s3d::Typeface::Bold);
			static auto t = 1.0;
			t += 1.0 / 60.0;

			const auto a = s3d::Min(0.25, t * 0.01);
			const auto p = s3d::Vec2(s3d::Window::Center()).movedBy(0.0, -s3d::Window::Height() * 0.3);
			const auto x = sin(t / 10.0 * 13) * 2.0;
			const auto y = sin(t / 10.0 * 11) * 2.0;

			titleFont(U"SyLife").drawAt(p.movedBy(x * 3.0, y * 3.0), s3d::ColorF(1.0, a));
			titleFont(U"SyLife").drawAt(p.movedBy(x * 2.0, y * 2.0), s3d::ColorF(1.0, a));
			titleFont(U"SyLife").drawAt(p.movedBy(x * 1.0, y * 1.0), s3d::ColorF(1.0, a));
			titleFont(U"SyLife").drawAt(p.movedBy(x * 0.0, y * 0.0), s3d::ColorF(1.0, a));
		}

		// message
		{
			static s3d::Font messageFont(24, s3d::Typeface::Bold);
			static auto t = -5.0;
			t += 1.0 / 60.0;

			const auto a = s3d::Clamp(t * 0.1, 0.0, 0.4);
			const auto p = s3d::Vec2(s3d::Window::Center()).movedBy(0.0, s3d::Window::Height() * 0.3);

			messageFont(U"始めるにはスペースキーを押してください...").drawAt(p, s3d::ColorF(1.0, a * (0.5 + 0.5 * abs(sin(t)))));
		}


		static Curtain curtain(s3d::Color(11, 22, 33), 1.0);
		curtain.OpenUpdate();
		m_audio.setVolume(s3d::Min(curtain.m_st.sF() / curtain.m_duration, 1.0));

		// scene遷移
		{
			static bool f = false;

			if (s3d::KeySpace.down()) f = true;

			if (f)
			{
				static Curtain closeCurtain(s3d::Color(11, 22, 33), 1.0);
				closeCurtain.CloseUpdate();
				m_audio.setVolume(s3d::Max(1.0 - closeCurtain.m_st.sF() / closeCurtain.m_duration, 0.0));

				if (closeCurtain.m_st.sF() > 1.0)
				{
					g_viewerManagerPtr->ClearViewers();
					g_viewerManagerPtr->AddViewer<FieldViewer>();
				}
			}
		}
	}
};


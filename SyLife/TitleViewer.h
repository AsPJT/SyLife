#pragma once

#include "Viewer.h"

#include "FieldViewer.h"
#include "EditorViewer.h"
#include "Curtain.h"

class TitleViewer
	: public Viewer
{
	class Bubble
	{
	public:
		Vec3	m_position;
		double	m_timer = 0.0;
	};

	enum Option
	{
		LaunchNewGame,
		ContinueGame,
		LaunchEditor,
		Exit,
	};

	Audio	m_audio;
	Option	m_selectedOption = Option::LaunchNewGame;
	Curtain m_closeCurtain;
	Curtain m_openCurtain;

	Array<Bubble>	m_bubbles;

public:
	TitleViewer()
		: m_audio(U"assets/music/�_��̐��E.mp3")
		, m_closeCurtain(Color(0, 0), Color(11, 22, 33), 0.5)
		, m_openCurtain(Color(11, 22, 33), Color(0, 0), 0.5, true)
	{
		SetDrawRect(Scene::Size());
		m_audio.setLoop(true);
		m_audio.play();
	}

	void	UpdateBubbles()
	{
		if (RandomBool(0.8))
		{
			auto& b = m_bubbles.emplace_back();

			b.m_position = Vec3(120.0 * Random(-1.0, 1.0), -100.0, 150.0 + 120.0 * Random(-1.0, 1.0));
		}
		static PerlinNoise noise1(Random(0xFFFFFFFF));
		static PerlinNoise noise2(Random(0xFFFFFFFF));
		static Vec3 liner(0, 0, 0);
		for (auto& b : m_bubbles)
		{
			b.m_timer += 1.0;
			b.m_position.x += 0.15 * noise1.noise(b.m_position * 0.02 + liner);
			b.m_position.y += 0.15 * Random(0.25, 1.0);
			b.m_position.z += 0.15 * noise2.noise(b.m_position * 0.02 + liner);
		}
		liner.moveBy(0, 0, 0.01);

		m_bubbles.remove_if([](const auto& b) { return b.m_timer > 1800.0; });
	}

	void	DrawBubbles()
	{
		static Texture texture(U"assets/image/particle.png", TextureDesc::Mipped);

		ScopedRenderStates2D blend(BlendState::Additive);

		static double t = 0.0;
		t += 1.0;
		for (auto& b : m_bubbles)
		{
			auto s = 5.0;
			Vec3 camPos(sin(t * 0.001 * 11) * s, sin(t * 0.001 * 13) * s, sin(t * 0.001 * 17) * s);
			Vec3 p = b.m_position - camPos;

			auto x = (asin(p.x / p.z) / (3.14 / 3.0) + 0.5) * Scene::Size().x;
			auto y = (-asin(p.y / p.z) / (3.14 / 3.0) + 0.5) * Scene::Size().y;
			auto r = 2000.0 / p.length() * Min(b.m_timer / 1000.0, 1.0) * 10.0;
			auto a = Min((1800.0 - b.m_timer) / 500.0, 1.0) * 0.1;

			texture.resized(r * 0.6).drawAt(x, y, ColorF(Palette::Lightblue, a));
			texture.resized(r * 1.0).drawAt(x, y, ColorF(Palette::Lightblue, a));
		}
	}

	void	Init() override
	{
		for (int i = 0; i < 2000; ++i) UpdateBubbles();
	}

	void	Update() override
	{

		// title
		{
			auto te = Transformer2D(Mat3x2::Scale(1.4, Scene::Center()));
			static Font titleFont(256, Typeface::Bold);
			static auto t = 1.0;
			t += 1.0 / 60.0;

			const auto a = 0.25;
			const auto p = Vec2(Scene::Center()).movedBy(0.0, -Scene::Height() * 0.1);
			const auto x = sin(t / 10.0 * 13) * 5.0;
			const auto y = sin(t / 10.0 * 11) * 5.0;

			titleFont(U"SyLife").drawAt(p.movedBy(x * 3.0, y * 3.0), ColorF(1.0, a));
			titleFont(U"SyLife").drawAt(p.movedBy(x * 2.0, y * 2.0), ColorF(1.0, a));
			titleFont(U"SyLife").drawAt(p.movedBy(x * 1.0, y * 1.0), ColorF(1.0, a));
			titleFont(U"SyLife").drawAt(p.movedBy(x * 0.0, y * 0.0), ColorF(1.0, a));
		}

		// bubbles
		{
			for (int i = 0; i < 3; ++i) UpdateBubbles();

			DrawBubbles();
		}

		// �I�v�V�����I��
		{
			// draw
			{
				static const Font messageFont(48, Typeface::Bold);

				// New Game
				{
					auto f = messageFont(U"New Game");
					auto r = f.region().setCenter(0, 0);
					auto t = Transformer2D(Mat3x2::Translate(Vec2(Scene::Center()).movedBy(0.0, Scene::Height() * 0.3)).translated(0, -96), true);

					r.draw(r.mouseOver() ? ColorF(1.0, 0.5) : ColorF(0.0, 0.0));
					f.drawAt(Vec2::Zero(), m_selectedOption == Option::LaunchNewGame ? ColorF(1.0, 0.6) : ColorF(1.0, 0.3));

					if (r.leftClicked())
					{
						SelectOption(Option::LaunchNewGame);
						LockEnter();
					}
				}

				// Continue
				{
					auto f = messageFont(U"Continue");
					auto r = f.region().setCenter(0, 0);
					auto t = Transformer2D(Mat3x2::Translate(Vec2(Scene::Center()).movedBy(0.0, Scene::Height() * 0.3)).translated(0, -32), true);

					r.draw(r.mouseOver() ? ColorF(1.0, 0.5) : ColorF(0.0, 0.0));
					f.drawAt(Vec2::Zero(), m_selectedOption == Option::ContinueGame ? ColorF(1.0, 0.6) : ColorF(1.0, 0.3));

					if (r.leftClicked())
					{
						SelectOption(Option::ContinueGame);
						LockEnter();
					}
				}

				// Editor
				{
					auto f = messageFont(U"Editor");
					auto r = f.region().setCenter(0, 0);
					auto t = Transformer2D(Mat3x2::Translate(Vec2(Scene::Center()).movedBy(0.0, Scene::Height() * 0.3)).translated(0, 32), true);

					r.draw(r.mouseOver() ? ColorF(1.0, 0.5) : ColorF(0.0, 0.0));
					f.drawAt(Vec2::Zero(), m_selectedOption == Option::LaunchEditor ? ColorF(1.0, 0.6) : ColorF(1.0, 0.3));

					if (r.leftClicked())
					{
						SelectOption(Option::LaunchEditor);
						LockEnter();
					}
				}

				// Exit
				{
					auto f = messageFont(U"Exit");
					auto r = f.region().setCenter(0, 0);
					auto t = Transformer2D(Mat3x2::Translate(Vec2(Scene::Center()).movedBy(0.0, Scene::Height() * 0.3)).translated(0, 96), true);

					r.draw(r.mouseOver() ? ColorF(1.0, 0.5) : ColorF(0.0, 0.0));
					f.drawAt(Vec2::Zero(), m_selectedOption == Option::Exit ? ColorF(1.0, 0.6) : ColorF(1.0, 0.3));

					if (r.leftClicked())
					{
						SelectOption(Option::Exit);
						LockEnter();
					}
				}
			}

			if (!IsEnterLocked())
			{
				// ���ɑJ��
				if (KeyS.down() || KeyDown.down())
				{
					switch (m_selectedOption)
					{
					case Option::LaunchNewGame:	m_selectedOption = Option::ContinueGame; break;
					case Option::ContinueGame:	m_selectedOption = Option::LaunchEditor; break;
					case Option::LaunchEditor:	m_selectedOption = Option::Exit; break;
					case Option::Exit:			break;
					}
				}

				// ��ɑJ��
				if (KeyW.down() || KeyUp.down())
				{
					switch (m_selectedOption)
					{
					case Option::LaunchNewGame:	break;
					case Option::ContinueGame:	m_selectedOption = Option::LaunchNewGame; break;
					case Option::LaunchEditor:	m_selectedOption = Option::ContinueGame; break;
					case Option::Exit:			m_selectedOption = Option::LaunchEditor; break;
					}
				}

				// �Z���N�g
				if (KeyEnter.down()) LockEnter();
			}
		}


		// Open Curtain
		{
			if (m_openCurtain.IsRunning() && m_openCurtain.Update()) m_audio.setVolume(m_openCurtain.GetProgress());
			else m_audio.setVolume(1.0);
		}

		// CloseCurtain
		if (IsEnterLocked())
		{
			if (m_closeCurtain.Update())
			{
				// ���g�̃C���X�^���X���폜�����̂ŁAreturn�ŏ������΂��K�v������B

				switch (m_selectedOption)
				{
				case Option::LaunchNewGame:	Enter_LaunchNewGame(); return;
				case Option::ContinueGame:	Enter_ContinueGame(); return;
				case Option::LaunchEditor:	Enter_LaunchEditor(); return;
				case Option::Exit:		 	Enter_Exit(); return;
				}
			}

			m_audio.setVolume(m_closeCurtain.GetProgress());
		}
	}

	bool	IsEnterLocked() { return m_closeCurtain.IsRunning(); }
	void	LockEnter() { m_closeCurtain.Start(); }
	void	SelectOption(Option option)
	{
		m_selectedOption = option;
	}

	void	Enter_LaunchNewGame()
	{
		g_viewerManagerPtr->ClearViewers();
		g_viewerManagerPtr->MakeViewer<FieldViewer>();
	}

	void	Enter_ContinueGame()
	{
		g_viewerManagerPtr->ClearViewers();
		g_viewerManagerPtr->MakeViewer<FieldViewer>();
	}

	void	Enter_LaunchEditor()
	{
		g_viewerManagerPtr->ClearViewers();
		g_viewerManagerPtr->MakeViewer<EditorViewer>();
	}

	void	Enter_Exit()
	{
		// �V�X�e���̏I��
		System::Exit();
	}
};

#include "flpch.h"
#include "GameLayer.h"
#include "Flora/Renderer/Renderer2D.h"
#include "Flora/Renderer/Font.h"

namespace Flora {

	static Ref<Framebuffer> s_Framebuffer;

	GameLayer::GameLayer()
		: Layer("Game") {

	}

	void GameLayer::OnAttatch() {
		RenderCommand::Init();
	}

	void GameLayer::OnDetatch() {
	}

	void GameLayer::OnUpdate(Timestep ts) {
		Renderer2D::ResetStats();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		SceneCamera camera;
		camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
		camera.SetOrthographic(20.0f, -999.0f, 999.0f);
		camera.SetViewportSize(1920, 1080);

		Renderer2D::BeginScene(camera.GetProjection());
		Renderer2D::DrawLine({-50, -50, -50}, {50, 50, 50});
		TextConfig conf;
		conf.TextString = "poooo";
		Renderer2D::DrawString(conf);
		Renderer2D::EndScene();
	}

	void GameLayer::OnImGuiRender() {

	}

	void GameLayer::OnEvent(Event& e) {

	}
}
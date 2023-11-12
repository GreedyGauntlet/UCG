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
		Renderer2D::Init();
		m_ActiveScene = new Scene();
	}

	void GameLayer::OnDetatch() {
	}

	void GameLayer::OnUpdate(Timestep ts) {
		Renderer2D::ResetStats();
		RenderCommand::SetClearColor({ 0.9f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		EditorCamera camera;
		Renderer2D::BeginScene(camera.GetViewProjection());
		Renderer2D::DrawCircle(glm::mat4(1.0f), glm::vec4(0, 1, 0, 1), 1.0f);
		Renderer2D::EndScene();

		/*
		Entity entity = m_ActiveScene->CreateEntity("Camera");
		Entity text = m_ActiveScene->CreateEntity("Text");
		entity.AddComponent<CameraComponent>();
		entity.GetComponent<CameraComponent>().Camera.SetOrthographic(10.0f, -10.0f, 10.0f);
		text.AddComponent<TextComponent>().TextString = "wooooaah";
		m_ActiveScene->SetPrimaryCamera((int)(uint32_t)entity);
		m_ActiveScene->OnRuntimeStart();
		m_ActiveScene->OnUpdateRuntime(ts);
		m_ActiveScene->OnRuntimeStop();*/
	}

	void GameLayer::OnImGuiRender() {

	}

	void GameLayer::OnEvent(Event& e) {

	}
}
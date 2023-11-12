#include "flpch.h"
#include "GameLayer.h"
#include "Flora/Renderer/Renderer2D.h"
#include "Flora/Renderer/Font.h"
#include "Scenes/BattleScene.h"
#include "Flora/Renderer/Framebuffer.h"
#include "Flora/Core/Input.h"

namespace UCG {

	static Flora::Ref<Flora::Framebuffer> fb;

	GameLayer::GameLayer()
		: Layer("Game") {

	}

	void GameLayer::OnAttatch() {
		m_Scene = new BattleScene();
		m_Scene->Start();

		Flora::FramebufferSpecification fbSpec;
		fbSpec.Attachments = { Flora::FramebufferTextureFormat::RGBA8, Flora::FramebufferTextureFormat::RED_INTEGER, Flora::FramebufferTextureFormat::Depth };
		fbSpec.Width = 1600;
		fbSpec.Height = 900;
		fb = Flora::Framebuffer::Create(fbSpec);
	}

	void GameLayer::OnDetatch() {
	}

	void GameLayer::OnUpdate(Flora::Timestep ts) {
		PreUpdate();
		m_Scene->Update(ts);
		int out = fb->ReadPixel(1, Flora::Input::GetMouseX(), Flora::Input::GetMouseY());
		FL_CORE_INFO("{}, {} : {}", Flora::Input::GetMouseX(), Flora::Input::GetMouseY(), out);
		PostUpdate();
		Flora::Renderer2D::ResetStats();
		Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Flora::RenderCommand::Clear();
		m_Scene->Update(ts);
	}

	void GameLayer::OnImGuiRender() {

	}

	void GameLayer::OnEvent(Flora::Event& e) {

	}

	void GameLayer::PreUpdate() {
		Flora::Renderer2D::ResetStats();

		fb->Bind();

		Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Flora::RenderCommand::Clear();
	}

	void GameLayer::PostUpdate() {

		fb->Unbind();
	}
}
#include "flpch.h"
#include "GameLayer.h"
#include "Flora/Renderer/Renderer2D.h"
#include "Flora/Renderer/Font.h"
#include "Scenes/BattleScene.h"
#include "Flora/Renderer/Framebuffer.h"
#include "Flora/Core/Input.h"

namespace UCG {

	GameLayer::GameLayer()
		: Layer("Game") {

	}

	void GameLayer::OnAttatch() {
		m_Scene = new BattleScene();
		m_Scene->Start();
	}

	void GameLayer::OnDetatch() {
	}

	void GameLayer::OnUpdate(Flora::Timestep ts) {
		m_Scene->Update(ts);
	}

	void GameLayer::OnImGuiRender() {

	}

	void GameLayer::OnEvent(Flora::Event& e) {

	}

	void GameLayer::PreUpdate() {
		
	}

	void GameLayer::PostUpdate() {

	}
}
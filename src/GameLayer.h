#pragma once
#include "Flora.h"
#include "Flora/Core/Layer.h"
#include "Scenes/GameScene.h"

namespace UCG {
	class GameLayer : public Flora::Layer {
	public:
		GameLayer();
		virtual ~GameLayer() = default;
		virtual bool RequestCloseProtection() override { return true; }
		virtual bool ConfirmClose() override { return true; }
		virtual void ProcessWindowClose() override {};
		virtual void OnAttatch() override;
		virtual void OnDetatch() override;
		void OnUpdate(Flora::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Flora::Event& e) override;
	private:
		void PreUpdate();
		void PostUpdate();
	private:
		GameScene* m_Scene;
	};
}
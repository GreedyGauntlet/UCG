#pragma once
#include "Flora.h"
#include "Flora/Scene/Scene.h"

namespace UCG {
	typedef glm::vec2 WindowDimensions;

	class GameScene : public Flora::Scene {
	public:
		virtual void Start() = 0;
		virtual void Update(Flora::Timestep ts);
		virtual void Stop() = 0;
	public:
		Flora::Entity* HoveredEntity();
		glm::vec2 MouseCoordinates();
		void ResizeWindow(uint32_t width, uint32_t height);
	protected:
		void GenericUpdate(Flora::Timestep ts);
	protected:
		Flora::Ref<Flora::SceneCamera> m_Camera;
	private:
		Flora::Ref<Flora::Framebuffer> m_Framebuffer;
		int64_t m_HoveredEntity = -1;
		WindowDimensions m_WindowDimensions;
	};
}
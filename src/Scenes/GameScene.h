#pragma once
#include "Flora.h"
#include "Flora/Scene/Scene.h"

namespace UCG {
	class GameScene : protected Flora::Scene {
	public:
		virtual void Start() = 0;
		virtual void Update(Flora::Timestep ts);
		virtual void Stop() = 0;
	protected:
		Flora::Ref<Flora::SceneCamera> m_Camera;
	};
}
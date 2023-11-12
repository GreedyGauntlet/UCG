#include "GameScene.h"

namespace UCG {

	void GameScene::Update(Flora::Timestep ts) {
		OnUpdateRuntime(ts, m_Camera->GetProjection());
	}

}
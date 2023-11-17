#include "Slime.h"

namespace UCG {

	void Slime::Initialize(Flora::Scene* context, Flora::Entity tile) {
		m_Context = context;
		m_Body = m_Context->CreateEntity("Slime");
		Flora::TransformComponent& tc = m_Body.GetComponent<Flora::TransformComponent>();
		Flora::SpriteRendererComponent& src = m_Body.AddComponent<Flora::SpriteRendererComponent>();
		tc.Translation = tile.GetComponent<Flora::TransformComponent>().Translation;
		tc.Translation.y += 0.5f;
		tc.Translation.z = 3.0f;
		tc.Scale = { 1.0f, 2.0f, 1.0f };
		src.Type = Flora::SpriteRendererComponent::SpriteType::ANIMATION;
		src.Path = "assets/Monsters/Slime.png";
		src.Frames = 12;
		src.StartFrame = 1;
		src.EndFrame = 12;
		src.FPS = 10;
		src.Rows = 1;
		src.Columns = 12;
	}

	void Slime::Update(Flora::Timestep ts) {

	}

	void Slime::Destroy() {

	}

}
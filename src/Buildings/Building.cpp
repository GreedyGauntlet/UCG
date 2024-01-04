#include "Building.h"
#include "Flora/Math/Math.h"
#include "../Utils/SceneUtils.h"
#include "Flora/Renderer/Renderer2D.h"
#include "Flora/Utils/ComponentUtils.h"
#include "../Scenes/BattleScene.h"

namespace UCG {
	void Building::Initialize(Flora::Scene* context, Flora::Entity tile) {
		m_Context = (BattleScene*)context;
		m_TileBody = tile;
		m_Body = context->CreateEntity("Building");
		m_Body.AddComponent<Flora::ParentComponent>().Parent = tile;
		Flora::SpriteRendererComponent& src = m_Body.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& tc = m_Body.GetComponent<Flora::TransformComponent>();
		tc.Translation.z = 0.01f;
		if (!tile.HasComponent<Flora::ChildComponent>()) tile.AddComponent<Flora::ChildComponent>();
		tile.GetComponent<Flora::ChildComponent>().AddChild(m_Body);
		src.Path = tile.GetComponent<Flora::SpriteRendererComponent>().Path;
		src.Color = glm::vec4(0.0f);
	}

	void Building::DrawHealth() {
		bool hovered = false;
		if (m_Context->HoveredEntity() >= 0)
			if ((uint32_t)(m_Context->HoveredEntity()) == (uint32_t)m_TileBody || (uint32_t)(m_Context->HoveredEntity()) == (uint32_t)m_Body) hovered = true;
		if ((m_Status.Health != m_Status.MaxHealth || hovered) && (m_Type != BuildingType::EMPTY) && (m_Type != BuildingType::NEXUS)) {
			glm::vec3 translation, rotation, scale;
			Flora::Math::DecomposeTransform(Flora::ComponentUtils::GetWorldTransform(m_Body), translation, rotation, scale);
			translation += glm::vec3(0.0f, -0.2f, 0.1f);
			Flora::Renderer2D::BeginScene(SceneUtils::MainCamera()->GetProjection());
			float node_width = 0.035f;
			float padding = 0.02f;
			float width = (node_width + padding) * m_Status.MaxHealth + padding;
			Flora::Renderer2D::DrawQuad(Flora::Math::ComposeTransform(translation, { 0.0f, 0.0f, 0.0f }, { width, 0.08, 1.0f }), { 0.0f, 0.0f, 0.0f, 1.0f });
			for (int i = 0; i < m_Status.MaxHealth; i++) {
				glm::vec4 color = i + 1 <= m_Status.Health ? glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
				Flora::Renderer2D::DrawQuad(
					Flora::Math::ComposeTransform(
						translation + glm::vec3((padding * (i + 2)) + (node_width * i) - (width / 2.0f), 0.0f, 0.1f),
						{ 0.0f, 0.0f, 0.0f },
						{ node_width, 0.04f, 1.0f }),
					color
				);
			}
			Flora::Renderer2D::EndScene();
		}
	}

	void Building::SetType(BuildingType type) {
		m_Type = type;
		switch (type) {
		case BuildingType::FOREST:
			m_Status.Health = m_Status.MaxHealth = 5;
			break;
		case BuildingType::MOUNTAIN:
			m_Status.Health = m_Status.MaxHealth = 10;
			break;
		case BuildingType::NEXUS:
			m_Status.Health = m_Status.MaxHealth = 20;
			break;
		default:
			break;
		}
	}
}

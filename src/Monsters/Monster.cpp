#include "Monster.h"
#include "Flora/Math/Math.h"
#include "../Utils/SceneUtils.h"
#include "Flora/Renderer/Renderer2D.h"

namespace UCG {

	void Monster::Destroy() {
		m_Context->DestroyEntity(m_Body);
		m_Dead = true;
	}

	void Monster::Damage(int damage) {
		m_Health -= damage;
		m_Damaged = true;
		m_LocalTime = 0.0f;
	}

	void Monster::Update(Flora::Timestep ts) {
		DrawHealth();
		DamageAnim(ts);
	}

	void Monster::DamageAnim(Flora::Timestep ts) {
		if (m_Damaged) {
			m_LocalTime += ts;
			if (m_LocalTime < 0.1f || (m_LocalTime > 0.15f && m_LocalTime < 0.25f)) {
				m_Body.GetComponent<Flora::SpriteRendererComponent>().Color = glm::vec4(10.0f);
			} else m_Body.GetComponent<Flora::SpriteRendererComponent>().Color = glm::vec4(1.0f);
			if (m_LocalTime > 1.0f) {
				m_LocalTime = 0.0f;
				m_Damaged = false;
			}
		}
		if (m_Health <= 0) {
			DeathAnim(ts);
		}
	}

	void Monster::DeathAnim(Flora::Timestep ts) {
		Destroy();
	}

	void Monster::DrawHealth() {
		bool hovered = false;
		if (m_Context->HoveredEntity())
			if (*m_Context->HoveredEntity() == m_Tile) hovered = true;
		if ((m_Health != m_MaxHealth || hovered) && m_Health > 0) {
			glm::vec3 translation = m_Body.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(0.0f, -0.9f, 0.1f);
			Flora::Renderer2D::BeginScene(SceneUtils::MainCamera()->GetProjection());
			float node_width = 0.035f;
			float padding = 0.02f;
			float width = (node_width + padding) * m_MaxHealth + padding;
			Flora::Renderer2D::DrawQuad(Flora::Math::ComposeTransform(translation, { 0.0f, 0.0f, 0.0f }, { width, 0.08, 1.0f }), { 0.0f, 0.0f, 0.0f, 1.0f });
			for (int i = 0; i < m_MaxHealth; i++) {
				glm::vec4 color = i + 1 <= m_Health ? glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
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

}
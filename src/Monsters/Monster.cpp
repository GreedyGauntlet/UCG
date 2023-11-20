#include "Monster.h"
#include "Flora/Math/Math.h"
#include "../Utils/SceneUtils.h"
#include "Flora/Renderer/Renderer2D.h"

namespace UCG {

	void Monster::Destroy() {
		m_Context->DestroyEntity(m_Body);
		m_Status.Dead = true;
	}

	void Monster::Damage(int damage) {
		m_Status.Health -= damage;
		m_Status.Damaged = true;
		m_Status.RelativeTime = 0.0f;
	}

	void Monster::Update(Flora::Timestep ts) {
		DrawHealth();
		DamageAnim(ts);
		UpdateAnimation(ts);
	}

	void Monster::DamageAnim(Flora::Timestep ts) {
		if (m_Status.Damaged) {
			m_Status.RelativeTime += ts;
			if (m_Status.RelativeTime < 0.1f || (m_Status.RelativeTime > 0.15f && m_Status.RelativeTime < 0.25f)) {
				m_Body.GetComponent<Flora::SpriteRendererComponent>().Color = glm::vec4(10.0f);
			} else m_Body.GetComponent<Flora::SpriteRendererComponent>().Color = glm::vec4(1.0f);
			if (m_Status.RelativeTime > 1.0f) {
				m_Status.RelativeTime = 0.0f;
				m_Status.Damaged = false;
			}
		}
		if (m_Status.Health <= 0) {
			DeathAnim(ts);
		}
	}

	void Monster::DeathAnim(Flora::Timestep ts) {
		Destroy();
	}

	void Monster::DrawHealth() {
		bool hovered = false;
		if (m_Context->HoveredEntity())
			if (*m_Context->HoveredEntity() == m_Tile || *m_Context->HoveredEntity() == m_Body) hovered = true;
		if ((m_Status.Health != m_Status.MaxHealth || hovered) && m_Status.Health > 0) {
			glm::vec3 translation = m_Body.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(0.0f, -0.9f, 0.1f);
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

	void Monster::QueueAnimation(AnimationCommand command) {
		m_AnimationQueue.Queue.push_back(command);
	}

	void Monster::OverrideAnimation(AnimationCommand command) {
		m_AnimationQueue.CurrentAnimation = command;
		m_AnimationQueue.AnimationTime = 0.0f;
	}

	void Monster::UpdateAnimation(Flora::Timestep ts) {
		// finish this animation function (HUUUGE switch case statement)
		//		switch the current animation command and case set the correct animation
		//		once enough time has passed then move on to the next queued animation 
		// revamp death and damaage animations
		// set up some functions for movement and attacking
		// profit (set up goblin for realsies)

	}

}
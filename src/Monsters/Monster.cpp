#include "Monster.h"

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

}
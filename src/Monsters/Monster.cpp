#include "Monster.h"

namespace UCG {

	void Monster::Destroy() {
		m_Context->DestroyEntity(m_Body);
	}

}
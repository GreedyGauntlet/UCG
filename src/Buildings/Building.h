#pragma once
#include "../Monsters/Monster.h"

namespace UCG {
	enum class BuildingType {
		EMPTY,
		FOREST,
		MOUNTAIN,
		NEXUS,
	};

	class Building : public Monster {
	public:
		virtual void Initialize(GameScene* context, Flora::Entity tile) override;
	protected:
		virtual void DrawHealth() override;
	public:
		void SetType(BuildingType type);
		BuildingType Type() { return m_Type; }
	private:
		BuildingType m_Type;
	};
}
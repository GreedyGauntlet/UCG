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
		virtual void Initialize(Flora::Scene* context, Flora::Entity tile);
	protected:
		virtual void DrawHealth() override;
	public:
		void SetType(BuildingType type);
		BuildingType Type() { return m_Type; }
	private:
		bool m_SoftInit = false;
		BuildingType m_Type;
		Flora::Entity m_TileBody;
	};
}

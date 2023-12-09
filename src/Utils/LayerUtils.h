#pragma once
#include "flpch.h"
#include "Flora.h"
#include "Flora/Scene/Scene.h"

namespace UCG {
	enum class ZLayers {
		BACKGROUND = 0,
		TILES      = 1,
		BACKVFX    = 2,
		OBJECT     = 3,
		FRONTVFX   = 4
	};

	class LayerUtils {
	public:
		static void Enable();
		static void Disable();
		static void Update();
		static void Update(Flora::Scene* context);
		static void ClearCache();
		static float LayerValue(ZLayers layer);
		static float LayerTop(ZLayers layer);
	};
}
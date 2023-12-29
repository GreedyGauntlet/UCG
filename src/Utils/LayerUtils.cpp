#include "LayerUtils.h"
#define LAYERSIZE 10.0f

namespace UCG {
	
	static bool s_Enabled = true;
	static std::map<ZLayers, float> s_DepthMap;
	static Flora::Scene* s_Context;

	void LayerUtils::Enable() {
		s_Enabled = true;
		Update();
	}

	void LayerUtils::Disable() {
		s_Enabled = false;
		ClearCache();
	}

	void LayerUtils::Update() {
		Update(s_Context);
	}

	void LayerUtils::Update(Flora::Scene* context) {
		if (context) {
			s_Context = context;
			/*
			context->ForAllComponents<Flora::TransformComponent>([](auto entity, auto component) {
				ZLayers currlayer = (ZLayers)(((int)component.Translation.z) / ((int)LAYERSIZE));
				if (s_DepthMap.find(currlayer) == s_DepthMap.end())
					s_DepthMap[currlayer] = component.Translation.z;
				else {
					s_DepthMap[currlayer] = s_DepthMap[currlayer] < component.Translation.z ? component.Translation.z : s_DepthMap[currlayer];
				}
			});*/
		}
	}

	void LayerUtils::ClearCache() {
		s_DepthMap.clear();
	}

	float LayerUtils::LayerValue(ZLayers layer) {
		return (float)layer * LAYERSIZE;
	}

	float LayerUtils::LayerTop(ZLayers layer) {
		return s_DepthMap.find(layer) != s_DepthMap.end() ? s_DepthMap[layer] + 0.001f : LayerValue(layer);
	}

}
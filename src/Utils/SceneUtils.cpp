#include "SceneUtils.h"
#include "Flora.h"

namespace UCG {

	static Flora::Ref<Flora::SceneCamera> s_Camera = nullptr;

	Flora::Ref<Flora::SceneCamera> SceneUtils::MainCamera() {
		if (s_Camera == nullptr) {
			s_Camera = Flora::CreateRef<Flora::SceneCamera>();
			s_Camera->SetProjectionType(Flora::SceneCamera::ProjectionType::Orthographic);
			s_Camera->SetOrthographic(10.0f, -999.0f, 999.0f);
			s_Camera->SetViewportSize(1920, 1080);
		}
		return s_Camera;
	}

}
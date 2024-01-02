#include "SceneUtils.h"
#include "Flora.h"

namespace UCG {

	static Flora::Ref<Flora::SceneCamera> s_Camera = nullptr;

	Flora::Ref<Flora::SceneCamera> SceneUtils::MainCamera() {
		if (s_Camera == nullptr) {
			s_Camera = Flora::CreateRef<Flora::SceneCamera>();
			s_Camera->SetProjectionType(Flora::SceneCamera::ProjectionType::Orthographic);
			s_Camera->SetBinding(Flora::SceneCamera::OrthographicBinding::Horizontal);
			s_Camera->SetOrthographic(12.0f, -999.0f, 999.0f);
			s_Camera->SetViewportSize(1600, 900);
		}
		return s_Camera;
	}

}

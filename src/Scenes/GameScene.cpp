#include "GameScene.h"

namespace UCG {

	void GameScene::Update(Flora::Timestep ts) {
		GenericUpdate(ts);
	}

	void GameScene::GenericUpdate(Flora::Timestep ts) {
		if (!m_Framebuffer) {
			Flora::FramebufferSpecification fbSpec;
			fbSpec.Attachments = { Flora::FramebufferTextureFormat::RGBA8, Flora::FramebufferTextureFormat::RED_INTEGER, Flora::FramebufferTextureFormat::Depth };
			fbSpec.Width = 1600;
			fbSpec.Height = 900;
			m_Framebuffer = Flora::Framebuffer::Create(fbSpec);
		}

		Flora::Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Flora::RenderCommand::Clear();
		RenderRuntime(Flora::Timestep(ts / 2.0f), m_Camera->GetProjection()); // note: dividing ts by 2 since we are rendering twice, otherwise animations would be twice as fast

		m_HoveredEntity = (int64_t)m_Framebuffer->ReadPixel(1, Flora::Input::GetMouseX(), m_WindowDimensions.y - Flora::Input::GetMouseY());

		m_Framebuffer->Unbind();
		Flora::Renderer2D::ResetStats();
		Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Flora::RenderCommand::Clear();

		OnUpdateRuntime(Flora::Timestep(ts / 2.0f), m_Camera->GetProjection());
		// NOTE: is doing this last going to make the framebuffer lag behind the update? 
		//       (i dont think so, in the generic update call entities are rendered first)
	}

	int64_t GameScene::HoveredEntity() {
		if (EntityExists((uint32_t)m_HoveredEntity) && m_HoveredEntity >= 0) {
			return (int64_t)((uint32_t)m_HoveredEntity);
		} return -1;
	}

	glm::vec2 GameScene::MouseCoordinates() {
		glm::vec2 coords = { 0.0f, 0.0f };
		Flora::SceneCamera::OrthographicBinding binding = m_Camera->GetBinding();
		
		double gamew = 1.0f;
		double gameh = 1.0f;
		if (binding == Flora::SceneCamera::OrthographicBinding::Vertical) {
			gameh = (double)(m_Camera->GetOrthographicSize());
			gamew = gameh * (m_WindowDimensions.x / m_WindowDimensions.y);
		} else {
			gamew = (double)(m_Camera->GetOrthographicSize());
			gameh = gamew * (m_WindowDimensions.y / m_WindowDimensions.x);
		}
		coords.x = (double)(gamew * (((double)Flora::Input::GetMouseX()) / m_WindowDimensions.x) - (gamew / 2.0));
		coords.y = (double)(gameh * (((double)(m_WindowDimensions.y - Flora::Input::GetMouseY())) / m_WindowDimensions.y) - (gameh / 2.0));
		return coords;
	}

	void GameScene::ResizeWindow(uint32_t width, uint32_t height) {
		m_Framebuffer->Resize(width, height);
		m_Camera->SetViewportSize(width, height);
		m_WindowDimensions.x = width;
		m_WindowDimensions.y = height;
	}

}
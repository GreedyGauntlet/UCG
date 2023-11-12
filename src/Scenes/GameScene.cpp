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
		RenderRuntime(ts, m_Camera->GetProjection());

		m_HoveredEntity = (int64_t)m_Framebuffer->ReadPixel(1, 1600 - Flora::Input::GetMouseX(), 900 - Flora::Input::GetMouseY());

		m_Framebuffer->Unbind();
		Flora::Renderer2D::ResetStats();
		Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Flora::RenderCommand::Clear();
		


		OnUpdateRuntime(ts, m_Camera->GetProjection()); 
		// NOTE: is doing this last going to make the framebuffer lag behind the update? 
		//       (i dont think so, in the generic update call entities are rendered first)
	}

	Flora::Entity* GameScene::HoveredEntity() {
		if (EntityExists((uint32_t)m_HoveredEntity) && m_HoveredEntity > 0) {
			return &(GetEntityFromID((uint32_t)m_HoveredEntity));
		} return nullptr;
	}

}
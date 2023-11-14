#include "BattleScene.h"
#include "Flora/Core/MouseCodes.h"
#include "Flora/Core/KeyCodes.h"
#include "../Utils/SceneUtils.h"

namespace UCG {

	static Board test_map = {
		{'F', 'F', 'M', 'D', 'D', 'D', 'D', 'P'},

		{'F', 'F', 'D', 'M', 'D', 'D', 'D', 'D'},

		{'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D'},

		{'D', 'D', 'W', 'W', 'W', 'D', 'D', 'D'},

		{'D', 'D', 'D', 'W', 'W', 'W', 'D', 'D'},

		{'D', 'D', 'D', 'W', 'D', 'D', 'D', 'F'},

		{'D', 'D', 'D', 'M', 'D', 'D', 'D', 'D'},

		{'O', 'D', 'D', 'D', 'M', 'D', 'D', 'D'},
	};

	void BattleScene::Start() {
		OnRuntimeStart();
		CreateUI();
		ResetBoard(test_map);
		DrawHand();
		UpdateHand(false);
		m_Camera = SceneUtils::MainCamera();
	}

	void BattleScene::Update(Flora::Timestep ts) {
		GenericUpdate(ts);
		UpdateBoard();
		UpdateHand();
		UpdateBattleState();
		DevCall();
	}

	void BattleScene::Stop() {

	}

	void BattleScene::ResetBoard(const Board board) {
		DeleteBoard();
		int b_width = board.size();
		int b_height = board[0].size();
		glm::vec3 map_origin = { 0.0f, 2.0f, 0.0f };
		for (int r = 0; r < b_width; r++) {
			std::vector<Flora::Entity> row;
			for (int c = 0; c < b_height; c++) {
				Flora::Entity tile = CreateEntity("Tile");
				tile.AddComponent<Flora::SpriteRendererComponent>();
				tile.GetComponent<Flora::SpriteRendererComponent>().Path = BoardUtils::TilePath(board[r][c]);
				tile.GetComponent<Flora::TransformComponent>().Translation.x = map_origin.x + (-0.5f * r + (0.5f * c));
				tile.GetComponent<Flora::TransformComponent>().Translation.y = map_origin.y + (-0.25f * c - (0.25f * r));
				row.push_back(tile);
			}
			m_BoardEntities.push_back(row);
		}
	}

	void BattleScene::DeleteBoard() {
		for (int i = 0; i < m_BoardEntities.size(); i++)
			for (int j = 0; j < m_BoardEntities.size(); j++)
				DestroyEntity(m_BoardEntities[i][j]);
		m_BoardEntities.clear();
	}

	void BattleScene::UpdateBoard() {
		glm::vec2 tr = MouseCoordinates();
		for (int r = 0; r < m_BoardEntities.size(); r++) {
			for (int c = 0; c < m_BoardEntities.size(); c++) {
				if (TileCollision(m_BoardEntities[r][c], tr))
					m_BoardEntities[r][c].GetComponent<Flora::SpriteRendererComponent>().Color = { 0.5f, 1.0f, 0.5f, 1.0f };
				else m_BoardEntities[r][c].GetComponent<Flora::SpriteRendererComponent>().Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			}
		}
	}

	bool BattleScene::TileCollision(Flora::Entity tile, glm::vec2 translation) {
		glm::vec2 tiletr = tile.GetComponent<Flora::TransformComponent>().Translation;

		if (translation.x > tiletr.x - 0.5f &&
			translation.x < tiletr.x + 0.5f &&
			translation.y > tiletr.y - 0.5f &&
			translation.y < tiletr.y) {
			float rely = translation.y - tiletr.y;
			float relx = translation.x - tiletr.x;
			if (rely > (-0.5f * relx) ||
				rely > (0.5f * relx) ||
				rely < (relx * 0.5f - 0.5f) ||
				rely < (relx * -0.5f - 0.5f)) {
				return false;
			}
			return true;
		}
		return false;
	}

	void BattleScene::DrawHand() {
		m_Hand.clear();
		for (int i = 0; i < 5; i++) DrawCard();
	}

	void BattleScene::UpdateHand(bool fast) {
		// move cards to respective hand pos
		float base_padding = 0.75f;
		if (m_Hand.size() > 6)
			base_padding = (base_padding * 6.0f) / (float)m_Hand.size();
		glm::vec3 origin = { 5.0f - (base_padding * (float)m_Hand.size()), -2.3f, 1.0f };
		float threshold = 0.0001f;
		for (int i = 0; i < m_Hand.size(); i++) {
			Flora::TransformComponent& tc = m_Hand[i].first.GetComponent<Flora::TransformComponent>();
			glm::vec3 dest_tr = { origin.x + (base_padding * (i + 1)), origin.y, origin.z + 0.01f * i };
			glm::vec3 dest_rt = { 0.0f, 0.0f, glm::radians(((float)(m_Hand.size() - (i + 1)) / (float)m_Hand.size()) * 45.0f) };
			if (m_SelectedCard == i) {
				dest_rt = glm::vec3(0.0f);
				dest_tr = { MouseCoordinates().x, MouseCoordinates().y, m_Hand.size() * 0.01f + origin.z };
			}
			if (fast) {
				tc.Translation = dest_tr;
				tc.Rotation = dest_rt;
			} else {
				glm::vec3 curr_tr = tc.Translation;
				glm::vec3 move_tr = {
					std::abs((dest_tr.x - curr_tr.x) / 5.0f) > threshold ? (dest_tr.x - curr_tr.x) / 5.0f : 0.0f,
					std::abs((dest_tr.y - curr_tr.y) / 5.0f) > threshold ? (dest_tr.y - curr_tr.y) / 5.0f : 0.0f,
					0.0f
				};
				tc.Translation += move_tr;
				tc.Translation.z = dest_tr.z;

				glm::vec3 curr_rt = tc.Rotation;
				glm::vec3 move_rt = {
					0.0f,
					0.0f,
					std::abs((dest_rt.z - curr_rt.z) / 10.0f) > threshold ? (dest_rt.z - curr_rt.z) / 10.0f : 0.0f
				};
				tc.Rotation += move_rt;
			}
		}

		// card hover highlighting
		Flora::Entity* he = HoveredEntity();
		if (he) {
			for (int i = 0; i < m_Hand.size(); i++) {
				Flora::Entity card = m_Hand[i].first;
				Flora::SpriteRendererComponent& src = card.GetComponent<Flora::SpriteRendererComponent>();
				if (*he == card)
					src.Color = { 0.5f, 0.5f, 1.0f, 1.0f };
				else src.Color = glm::vec4(1.0f);
			}
		}

		// card dragging
		if (!Flora::Input::IsMouseButtonPressed(Flora::Mouse::Button0)) m_SelectedCard = -1;
		else if (m_SelectedCard < 0)
			if (HoveredEntity())
				for (int i = 0; i < m_Hand.size(); i++)
					if (m_Hand[i].first == *HoveredEntity())
						m_SelectedCard = i;
	}

	void BattleScene::CreateUI() {

		m_UI.PUI.ProfileBorder = CreateEntity("Player Profile Border");
		m_UI.PUI.ProfilePicture = CreateEntity("Player Profile Picture");
		m_UI.PUI.ProfileBorder.AddComponent<Flora::SpriteRendererComponent>().Path = "assets/UI/Profiles/ForestProfile.png";
		m_UI.PUI.ProfilePicture.AddComponent<Flora::SpriteRendererComponent>().Path = "assets/Profile.png";
		m_UI.PUI.ProfileBorder.GetComponent<Flora::TransformComponent>().Translation = { -5.2f, 2.65f, 998.0f };
		m_UI.PUI.ProfilePicture.GetComponent<Flora::TransformComponent>().Translation = { -5.2f, 2.65f, 998.1f };

		for (int i = 0; i < m_Stats.PlayerHealth; i++) {
			Flora::Entity heart = CreateEntity("Health Container");
			heart.AddComponent<Flora::SpriteRendererComponent>().Path = "assets/UI/Battle/Heart.png";
			heart.GetComponent<Flora::TransformComponent>().Translation = m_UI.PUI.ProfileBorder.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(0.25f * (i + 1) + 0.3f, 0.3f, 0.0f);
			heart.GetComponent<Flora::TransformComponent>().Scale = { 0.5f, 0.5f, 1.0f };
			m_UI.PUI.Health.push_back(heart);
		}

		for (int i = 0; i < m_Stats.PlayerMana; i++) {
			Flora::Entity mana = CreateEntity("Mana Container");
			mana.AddComponent<Flora::SpriteRendererComponent>().Path = "assets/UI/Battle/Mana.png";
			mana.GetComponent<Flora::TransformComponent>().Translation = m_UI.PUI.ProfileBorder.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(0.28f * (i + 1) + 0.4f, 0.0f, 0.0f);
			mana.GetComponent<Flora::TransformComponent>().Scale = { 0.5f, 0.5f, 1.0f };
			m_UI.PUI.Mana.push_back(mana);
		}
	}

	void BattleScene::UpdateBattleState() {
		switch (m_State) {
		case BattleState::PREPLAYER:
			DrawCard();
			m_State = BattleState::PLAYER;
			break;
		}
	}

	void BattleScene::DrawCard() {
		std::pair<Flora::Entity, Card> pair = { CreateEntity("Card"), Player::GetDeck()->Draw() };
		pair.first.AddComponent<Flora::SpriteRendererComponent>().Path = "assets/Card.png";
		pair.first.GetComponent<Flora::TransformComponent>().Translation = { 0.0f, 0.0f, 1.0f };
		pair.first.GetComponent<Flora::TransformComponent>().Scale = { 1.0f, 1.5f, 1.0f };
		m_Hand.push_back(pair);
	}

	void BattleScene::DevCall() {
		static bool d_down = Flora::Input::IsKeyPressed(Flora::Key::D);
		if (d_down) {
			if (!Flora::Input::IsKeyPressed(Flora::Key::D)) {
				DrawCard();
				d_down = false;
			} 
		}
		else if (Flora::Input::IsKeyPressed(Flora::Key::D)) {
			d_down = true;
		}
	}

}
#include "BattleScene.h"
#include "Flora/Core/MouseCodes.h"
#include "Flora/Core/KeyCodes.h"
#include "../Utils/SceneUtils.h"
#include "../VFX/VFXCore.h"
#include "../Monsters/MonsterCore.h"

namespace UCG {

	static Board test_map = {
		{'F', 'F', 'M', 'D', 'D', 'D', 'D', 'P'},

		{'F', 'F', 'D', 'M', 'D', 'D', 'D', 'D'},

		{'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D'},

		{'D', 'D', 'W', 'W', 'W', 'D', 'D', 'D'},

		{'D', 'D', 'D', 'W', 'W', 'W', 'D', 'D'},

		{'D', 'D', 'D', 'W', 'D', 'D', 'D', 'F'},

		{'D', 'D', 'D', 'M', 'D', 'D', 'D', 'W'},

		{'O', 'D', 'D', 'D', 'M', 'D', 'W', 'W'},
	};

	template<typename SelectFunction>
	void BattleScene::SelectTile(bool trigger, std::vector<std::string> enabled_tiles, SelectFunction selectfunc) {
		glm::vec2 tr = MouseCoordinates();
		for (int r = 0; r < m_BoardEntities.size(); r++) {
			for (int c = 0; c < m_BoardEntities.size(); c++) {
				bool valid = false;
				for (auto str : enabled_tiles) {
					if (m_BoardEntities[r][c].GetComponent<Flora::TagComponent>().Tag == str) {
						valid = true;
						break;
					}
				}
				if (valid) {
					if (TileCollision(m_BoardEntities[r][c], tr)) {
						m_BoardEntities[r][c].GetComponent<Flora::SpriteRendererComponent>().Color = { 2.0f, 2.0f, 1.0f, 1.0f };
						if (trigger) {
							selectfunc(this, m_BoardEntities[r][c]);
							CleanBoard();
							return;
						}
					}
					else m_BoardEntities[r][c].GetComponent<Flora::SpriteRendererComponent>().Color = { 0.2f, 0.9f, 0.2f, 1.0f };
				}
				else m_BoardEntities[r][c].GetComponent<Flora::SpriteRendererComponent>().Color = { 0.9f, 0.2f, 0.2f, 1.0f };
			}
		}
	}

	void BattleScene::Start() {
		OnRuntimeStart();
		CreateUI();
		ResetBoard(test_map);
		DrawHand();
		m_Camera = SceneUtils::MainCamera();
		m_Monsters.clear();
	}

	void BattleScene::Update(Flora::Timestep ts) {
		GenericUpdate(ts);
		UpdateBoard();
		UpdateHand(ts);
		UpdateUI();
		UpdateBattleState();
		DevCall();
		UpdateSpell();
		UpdateMonsters(ts);
	}

	void BattleScene::Stop() {

	}

	void BattleScene::UpdateMonsters(Flora::Timestep ts) {
		for (int i = 0; i < m_Monsters.size(); i++) {
			if (!m_Monsters[i]->Alive()) {
				delete m_Monsters[i];
				m_Monsters.erase(m_Monsters.begin() + i);
			}
		}
		for (auto& monster : m_Monsters) monster->Update(ts);
	}

	void BattleScene::ResetBoard(const Board board) {
		DeleteBoard();
		int b_width = board.size();
		int b_height = board[0].size();
		glm::vec3 map_origin = { 0.0f, 2.0f, -1.0f };
		for (int r = 0; r < b_width; r++) {
			std::vector<Flora::Entity> row;
			for (int c = 0; c < b_height; c++) {
				std::string tilename = " ";
				tilename[0] = board[r][c];
				Flora::Entity tile = CreateEntity(tilename);
				tile.AddComponent<Flora::SpriteRendererComponent>();
				tile.GetComponent<Flora::SpriteRendererComponent>().Path = BoardUtils::TilePath(board[r][c]);
				tile.GetComponent<Flora::TransformComponent>().Translation = map_origin + glm::vec3(-0.5f * r + (0.5f * c), -0.25f * c - (0.25f * r), 0.001f * (r + c));
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
		/*
		glm::vec2 tr = MouseCoordinates();
		for (int r = 0; r < m_BoardEntities.size(); r++) {
			for (int c = 0; c < m_BoardEntities.size(); c++) {
				if (TileCollision(m_BoardEntities[r][c], tr))
					m_BoardEntities[r][c].GetComponent<Flora::SpriteRendererComponent>().Color = { 0.5f, 1.0f, 0.5f, 1.0f };
				else m_BoardEntities[r][c].GetComponent<Flora::SpriteRendererComponent>().Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			}
		}*/
	}

	bool BattleScene::TileCollision(Flora::Entity tile, glm::vec2 translation) {
		glm::vec2 tiletr = tile.GetComponent<Flora::TransformComponent>().Translation;

		if (translation.x > tiletr.x - 0.5f &&
			translation.x < tiletr.x + 0.5f &&
			translation.y > tiletr.y - 0.25f &&
			translation.y < tiletr.y + 0.25f) {
			float rely = translation.y - tiletr.y;
			float relx = translation.x - tiletr.x;
			if (rely > (-0.5f * relx + 0.25f) ||
				rely > (0.5f * relx + 0.25f) ||
				rely < (relx * 0.5f - 0.25f) ||
				rely < (relx * -0.5f - 0.25f)) {
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

	void BattleScene::UpdateHand(Flora::Timestep ts, bool fast) {
		// move cards to respective hand pos
		float base_padding = 0.75f;
		float card_speed = (float)ts * 60.0f;
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
				glm::vec3 move_tr = card_speed * glm::vec3(
					std::abs((dest_tr.x - curr_tr.x) / 5.0f) > threshold ? (dest_tr.x - curr_tr.x) / 5.0f : 0.0f,
					std::abs((dest_tr.y - curr_tr.y) / 5.0f) > threshold ? (dest_tr.y - curr_tr.y) / 5.0f : 0.0f,
					0.0f
				);
				tc.Translation += move_tr;
				tc.Translation.z = dest_tr.z;

				glm::vec3 curr_rt = tc.Rotation;
				glm::vec3 move_rt = card_speed * glm::vec3(
					0.0f,
					0.0f,
					std::abs((dest_rt.z - curr_rt.z) / 10.0f) > threshold ? (dest_rt.z - curr_rt.z) / 10.0f : 0.0f
				);
				tc.Rotation += move_rt;
			}
		}

		// card hover highlighting
		for (int i = 0; i < m_Hand.size(); i++) {
			Flora::Entity card = m_Hand[i].first;
			Flora::SpriteRendererComponent& src = card.GetComponent<Flora::SpriteRendererComponent>();
			if (CheckHovered(card))
				src.Color = { 0.5f, 0.5f, 1.0f, 1.0f };
			else
				src.Color = glm::vec4(1.0f);
		}

		// card activation
		if (m_SelectedCard >= 0) {
			Flora::Entity card = m_Hand[m_SelectedCard].first;
			if (card.GetComponent<Flora::TransformComponent>().Translation.y > -1.0f) {
				card.GetComponent<Flora::SpriteRendererComponent>().Color = { 0.5f, 1.0f, 0.5f, 1.0f };
				if (!Flora::Input::IsMouseButtonPressed(Flora::Mouse::Button0))
					ActivateCard(m_SelectedCard);
			}
		}

		// card dragging
		if (!Flora::Input::IsMouseButtonPressed(Flora::Mouse::Button0)) m_SelectedCard = -1;
		else if (m_SelectedCard < 0)
			if (HoveredEntity())
				for (int i = 0; i < m_Hand.size(); i++)
					if (CheckHovered(m_Hand[i].first))
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
		case BattleState::PLAYER:
			break;
		}
	}

	void BattleScene::DrawCard() {
		std::pair<Flora::Entity, Card> pair = { CreateEntity("Card"), Player::GetDeck()->Draw() };
		pair.first.AddComponent<Flora::SpriteRendererComponent>().Path = "assets/Card.png";
		pair.first.GetComponent<Flora::TransformComponent>().Translation = { 0.0f, 0.0f, 1.0f };
		pair.first.GetComponent<Flora::TransformComponent>().Scale = { 1.0f, 1.5f, 1.0f };
		
		Flora::ChildComponent& cc = pair.first.AddComponent<Flora::ChildComponent>();
		for (int i = 0; i < pair.second.Cost; i++) {
			Flora::Entity mana_node = CreateEntity("Mana Node");
			mana_node.AddComponent<Flora::SpriteRendererComponent>().Path = "assets/Cards/Decorators/Mana Badge.png";
			mana_node.GetComponent<Flora::TransformComponent>().Translation = { -0.29f + (i%7 * 0.1f), -0.27f - (i/7 * 0.1f), 0.001f };
			mana_node.GetComponent<Flora::TransformComponent>().Scale = { 8.0f/72.0f, 8.0f/108.0f, 1.0f };
			mana_node.AddComponent<Flora::ParentComponent>().Parent = pair.first;
			cc.AddChild(mana_node);
		}

		Flora::Entity splash_art = CreateEntity("Splash Art");
		splash_art.AddComponent<Flora::SpriteRendererComponent>().Path = "assets/Cards/Splash Art/" + pair.second.Name + ".png";
		splash_art.GetComponent<Flora::TransformComponent>().Translation = { 0.0f, 0.0f, 0.002f };
		splash_art.AddComponent<Flora::ParentComponent>().Parent = pair.first;
		cc.AddChild(splash_art);

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

	bool BattleScene::CheckHovered(Flora::Entity entity) {
		Flora::Entity* he = HoveredEntity();
		if (he) {
			if (entity == *he) return true;
			if (entity.HasComponent<Flora::ChildComponent>())
				for (auto child : entity.GetComponent<Flora::ChildComponent>().Children)
					if (child == *he)
						return true;
		}
		return false;
	}

	bool BattleScene::ActivateCard(int cardIndex) {
		Flora::Entity card = m_Hand[cardIndex].first;
		Card info = m_Hand[cardIndex].second;

		if (m_Stats.PlayerMana >= info.Cost) {
			m_Stats.PlayerMana -= info.Cost;
			m_CurrentSpell = info.ID;
			m_Hand.erase(m_Hand.begin() + cardIndex);
			DestroyEntity(card);
			return true;
		}
		return false;
	}

	void BattleScene::UpdateUI() {
		for (int i = 0; i < m_UI.PUI.Mana.size(); i++) {
			Flora::Entity mana = m_UI.PUI.Mana[i];
			if (i + 1 > m_Stats.PlayerMana) {
				m_UI.PUI.Mana.erase(m_UI.PUI.Mana.begin() + i);
				DestroyEntity(mana);
				i--;
				continue;
			}
			mana.GetComponent<Flora::TransformComponent>().Translation = m_UI.PUI.ProfileBorder.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(0.28f * (i + 1) + 0.4f, 0.0f, 0.0f);
		}
	}

	void BattleScene::CleanBoard() {
		for (int r = 0; r < m_BoardEntities.size(); r++) {
			for (int c = 0; c < m_BoardEntities.size(); c++) {
				m_BoardEntities[r][c].GetComponent<Flora::SpriteRendererComponent>().Color = glm::vec4(1.0f);
			}
		}
	}

	void BattleScene::UpdateSpell() {

		static bool mousepressed = Flora::Input::IsMouseButtonPressed(Flora::Mouse::Button0);
		bool mousereleased = false;
		if (mousepressed) {
			if (!Flora::Input::IsMouseButtonPressed(Flora::Mouse::Button0)) {
				mousereleased = true;
				mousepressed = false;
			}
		} else mousepressed = Flora::Input::IsMouseButtonPressed(Flora::Mouse::Button0);

		static bool firstpass = true;
		if (firstpass && m_CurrentSpell != CardID::NONE) {
			if (mousereleased) {
				firstpass = false;
				return;
			}
		}

		static LightningCloud* vfx = nullptr;

		#define ENDSPELL() m_CurrentSpell = CardID::NONE; firstpass = true;
		switch (m_CurrentSpell) {
		case CardID::SMITE:
			if (vfx == nullptr) {
				SelectTile(mousereleased, {"D"}, [](auto scene_context, auto& tile) {
					vfx = new LightningCloud(scene_context, tile);
				});
			} else {
				if (vfx->Activate()) {
					for (auto monster : m_Monsters)
						if (monster->Tile() == vfx->Tile())
							monster->Damage(1);
				}
				if (!vfx->Update()) {
					delete vfx;
					vfx = nullptr;
					ENDSPELL();
				}
			}
			break;
		case CardID::GOBLIN:
			//TODO
			ENDSPELL();
			break;
		case CardID::SLIME:
			SelectTile(mousereleased, { "D" }, [this](auto scene_context, auto& tile) {
				Monster* slime = new Slime();
				slime->Initialize(scene_context, tile);
				m_Monsters.push_back(slime);
				ENDSPELL();
			});
			break;
		}
		#undef ENDSPELL
	}

}

//NEXT:
/*
- no colliding summons
- redo health, mana, player border
- enemy border and health/mana
- meteor card that does what smite does right now, but can burn forests and break mountains
- change smite to select monsters only
- cancel spell 
- moving goblin
- integrate turns
- if needles done:
	- implement needles
- else:
	- just start creating cards and tuning up UI
*/
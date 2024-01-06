#include "BattleScene.h"
#include "Flora/Core/MouseCodes.h"
#include "Flora/Core/KeyCodes.h"
#include "Flora/Renderer/Renderer2D.h"
#include "Flora/Math/Math.h"
#include "../Utils/SceneUtils.h"
#include "../Utils/LayerUtils.h"
#include "../VFX/VFXCore.h"
#include "../Monsters/MonsterCore.h"
#include "../Buildings/BuildingCore.h"
#include "../Utils/FileUtils.h"

namespace UCG {

	static Blueprint test_map = {
		{'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D'},

		{'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D'},

		{'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D'},

		{'D', 'D', 'W', 'W', 'W', 'D', 'D', 'D'},

		{'D', 'D', 'D', 'W', 'W', 'W', 'D', 'D'},

		{'D', 'D', 'D', 'W', 'D', 'D', 'D', 'D'},

		{'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},

		{'D', 'D', 'D', 'D', 'D', 'D', 'W', 'W'},
	};

	static Blueprint test_map_buildings = {
		{'F', 'F', 'M', 'E', 'E', 'E', 'E', 'P'},

		{'F', 'F', 'E', 'M', 'E', 'E', 'E', 'E'},

		{'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},

		{'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},

		{'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'},

		{'E', 'E', 'E', 'E', 'E', 'E', 'E', 'F'},

		{'E', 'E', 'E', 'M', 'E', 'E', 'E', 'E'},

		{'O', 'E', 'E', 'E', 'M', 'E', 'E', 'E'},
	};

	template<typename SelectFunction>
	bool BattleScene::SelectTile(bool trigger, std::vector<TileObj> workingset, SelectFunction selectfunc, TileSelectFlag flags) {
		glm::vec2 tr = MouseCoordinates();
		for (int r = 0; r < (int)m_BoardObjects.Tiles.size(); r++) {
			for (int c = 0; c < (int)m_BoardObjects.Tiles.size(); c++) {
				bool valid = workingset.size() == 0;

				if (!valid) {
					for (TileObj obj : workingset) {
						if ((uint32_t)m_BoardObjects.Tiles[r][c].Contents.Body == (uint32_t)obj.Body) {
							valid = true;
							break;
						}
					}
				}

				if (valid) {
					valid = false;
					if (flags & TileSelectFlags::ALL)
						valid = true;
					else {
						if (flags & TileSelectFlags::OCCUPIED) {
							for (auto monster : m_BoardObjects.Monsters)
								if (monster->GetTile().Coordinates == TileRef{r, c}) {
									valid = true;
									break;
								}
						} else if (flags & TileSelectFlags::UNOCCUPIED) {
							valid = true;
							for (auto monster : m_BoardObjects.Monsters)
								if (monster->GetTile().Coordinates == TileRef{r, c}) {
									valid = false;
									break;
								}
						} else {
							valid = true;
						}
						if (valid) {
							valid = false;
							bool specified = false;

							if (flags & TileSelectFlags::DIRT) {
								specified = true;
								if (m_BoardObjects.Tiles[r][c].Contents.Body.GetComponent<Flora::TagComponent>().Tag[0] == 'D')
									valid = true;
							}

							if (flags & TileSelectFlags::FOREST) {
								specified = true;
								if (m_BoardObjects.Tiles[r][c].Contents.Body.GetComponent<Flora::TagComponent>().Tag[0] == 'F')
									valid = true;
							}

							if (flags & TileSelectFlags::MOUNTAIN) {
								specified = true;
								if (m_BoardObjects.Tiles[r][c].Contents.Body.GetComponent<Flora::TagComponent>().Tag[0] == 'M')
									valid = true;
							}

							if (flags & TileSelectFlags::PLAYER) {
								specified = true;
								if (m_BoardObjects.Tiles[r][c].Contents.Body.GetComponent<Flora::TagComponent>().Tag[0] == 'P')
									valid = true;
							}

							if (flags & TileSelectFlags::OPPONENT) {
								specified = true;
								if (m_BoardObjects.Tiles[r][c].Contents.Body.GetComponent<Flora::TagComponent>().Tag[0] == 'O')
									valid = true;
							}

							if (flags & TileSelectFlags::NEXUS) {
								specified = true;
								if (m_BoardObjects.Tiles[r][c].Contents.Physical->Type() == BuildingType::NEXUS)
									valid = true;
							}

							if (flags & TileSelectFlags::WATER) {
								specified = true;
								if (m_BoardObjects.Tiles[r][c].Contents.Body.GetComponent<Flora::TagComponent>().Tag[0] == 'W')
									valid = true;
							}

							if (!specified) valid = true;
						}
					}
				}

				if (valid) {
					if (TileCollision(m_BoardObjects.Tiles[r][c].Contents.Body, tr)) {
						DeepTint(m_BoardObjects.Tiles[r][c].Contents.Body, { 2.0f, 2.0f, 1.0f, 1.0f });
						if (trigger) {
							TileRef ref = {r, c};
							selectfunc(this, ref);
							CleanBoard();
							return true;
						}
					}
					else DeepTint(m_BoardObjects.Tiles[r][c].Contents.Body, { 0.2f, 0.9f, 0.2f, 1.0f });
				}
				else DeepTint(m_BoardObjects.Tiles[r][c].Contents.Body, { 0.9f, 0.2f, 0.2f, 1.0f });
			}
		}
		if (trigger) return false;
		return true;
	}

	void BattleScene::Start() {
		OnRuntimeStart();
		CreateUI();
		ResetBoard({test_map, test_map_buildings});
		DrawHand();
		m_Camera = SceneUtils::MainCamera();
		m_BoardObjects.Monsters.clear();
	}

	void BattleScene::Update(Flora::Timestep ts) {
		GenericUpdate(ts);
		LayerUtils::Update((Flora::Scene*)this);
		UpdateBoard(ts);
		UpdateHand(ts);
		UpdateBattleState(ts);
		DevCall();
		UpdateSpell();
		UpdateBoardObjects(ts);
		UpdateUI();
	}

	void BattleScene::Stop() {

	}

	void BattleScene::UpdateBoardObjects(Flora::Timestep ts) {
		for (int i = 0; i < (int)m_BoardObjects.Monsters.size(); i++) {
			if (!m_BoardObjects.Monsters[i]->Alive()) {
				delete m_BoardObjects.Monsters[i];
				m_BoardObjects.Monsters.erase(m_BoardObjects.Monsters.begin() + i);
			}
		}
		for (auto& monster : m_BoardObjects.Monsters) monster->Update(ts);
		for (auto& vfx : m_BoardObjects.VFXs) vfx->PersistantUpdate(); 
		// note: we leave generic time updates and vfx destruction to be 
		//done manually via blocking the card activation function or turn 
		//updating because its safer to do there in case time-sensitive 
		//events get overriden or mixed
	}

	Monster* BattleScene::GetMonster(TileRef tile) {
		for (int i = 0; i < (int)m_BoardObjects.Monsters.size(); i++)
			if (m_BoardObjects.Monsters[i]->GetTile().Coordinates == tile)
				return m_BoardObjects.Monsters[i];
		return nullptr;
	}

	TileRef BattleScene::GetTileRef(Flora::Entity tile) {
		for (int r = 0; r < (int)m_BoardObjects.Tiles.size(); r++)
			for (int c = 0; c < (int)m_BoardObjects.Tiles[0].size(); c++)
				if (m_BoardObjects.Tiles[r][c].Contents.Body == tile)
					return {r, c};
		return {-1, -1};
	}

	bool BattleScene::TileOccupied(TileRef tile) {
		if (!ValidBoardCoord(tile)) return false;
		for (int i = 0; i < (int)m_BoardObjects.Monsters.size(); i++) {
			if (m_BoardObjects.Monsters[i]->GetTileRef() == tile)
				return true;
		}
		return false;
	}

	void BattleScene::DeepTint(Flora::Entity tile, glm::vec4 color) {
		if (tile.HasComponent<Flora::SpriteRendererComponent>())
			if (tile.GetComponent<Flora::SpriteRendererComponent>().Color.w != 0.0f)
				tile.GetComponent<Flora::SpriteRendererComponent>().Color = color;
		if (tile.HasComponent<Flora::ChildComponent>()) {
			std::vector<Flora::Entity> children = tile.GetComponent<Flora::ChildComponent>().Children;
			for (auto& child : children) {
				DeepTint(child, color);
			}
		}
	}

	void BattleScene::ResetBoard(const Map map) {
		DeleteBoard();
		int b_width = map.Terrain.size();
		int b_height = map.Terrain[0].size();
		glm::vec3 map_origin = { 0.0f, 2.0f, -1.0f };
		for (int r = 0; r < b_width; r++) {
			std::vector<Tile> row;
			for (int c = 0; c < b_height; c++) {
				Tile tile = MakeTile({r, c}, map);
				tile.Contents.Body.GetComponent<Flora::TransformComponent>().Translation = map_origin + glm::vec3(-0.5f * r + (0.5f * c), -0.25f * c - (0.25f * r), 0.001f * (r + c));
				row.push_back(tile);
				if (map.Buildings[r][c] == 'P') m_BoardObjects.PlayerNexus = tile.Contents.Body;
				else if (map.Buildings[r][c] == 'O') m_BoardObjects.OpponentNexus = tile.Contents.Body;
			}
			m_BoardObjects.Tiles.push_back(row);
		}
	}

	void BattleScene::DeleteBoard() {
		for (int i = 0; i < (int)m_BoardObjects.Tiles.size(); i++) {
			for (int j = 0; j < (int)m_BoardObjects.Tiles[0].size(); j++) {
				DestroyEntity(m_BoardObjects.Tiles[i][j].Contents.Body);
				delete m_BoardObjects.Tiles[i][j].Contents.Physical;
			}
		}
		m_BoardObjects.Tiles.clear();
	}

	void BattleScene::UpdateBoard(Flora::Timestep ts) {
		for (int r = 0; r < (int)m_BoardObjects.Tiles.size(); r++) {
			for (int c = 0; c < (int)m_BoardObjects.Tiles[0].size(); c++) {
				m_BoardObjects.Tiles[r][c].Contents.Physical->Update(ts);
			}
		}
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

	Tile BattleScene::MakeTile(TileRef coordinates, Map map) {
		std::string tilename = " ";
		tilename[0] = map.Buildings[coordinates.r][coordinates.c] == 'E' ? map.Terrain[coordinates.r][coordinates.c] : map.Buildings[coordinates.r][coordinates.c];
		Flora::Entity tile = CreateEntity(tilename);
		Building* building = new Building();
		building->SetType(BuildingType::EMPTY);
		Flora::SpriteRendererComponent& src = tile.AddComponent<Flora::SpriteRendererComponent>();
		TileTypes type;
		switch (map.Terrain[coordinates.r][coordinates.c]) {
		case 'D':
			src.Path = UCG::FileUtils::Path("assets/Tiles/Dirt.png");
			type = TileTypes::DIRT;
			break;
		case 'W':
			src.Path = UCG::FileUtils::Path("assets/Tiles/Water.png");
			type = TileTypes::WATER;
			break;
		default:
			src.Path = "invalid.png";
			type = TileTypes::DIRT;
			break;
		}
		switch (map.Buildings[coordinates.r][coordinates.c]) {
		case 'P':
			src.Path = UCG::FileUtils::Path("assets/Tiles/Player.png");
			building->SetType(BuildingType::NEXUS);
			break;
		case 'O':
			src.Path = UCG::FileUtils::Path("assets/Tiles/Opponent.png");
			building->SetType(BuildingType::NEXUS);
			break;
		case 'F':
			building = new Forest();
			break;
		case 'M':
			src.Path = UCG::FileUtils::Path("assets/Tiles/Mountain.png");
			building->SetType(BuildingType::MOUNTAIN);
			break;
		default:
			break;
		}
		building->Initialize(this, tile);
		TileObj contents = { building, tile};
		return { coordinates, contents, type };
	}

	void BattleScene::DrawHand() {
		m_Hand.Cards.clear();
		for (int i = 0; i < 5; i++) DrawCard();
	}

	void BattleScene::UpdateHand(Flora::Timestep ts, bool fast) {
		// move cards to respective hand pos
		float base_padding = 0.75f;
		float card_speed = (float)ts * 60.0f;
		if (m_Hand.Cards.size() > 6)
			base_padding = (base_padding * 6.0f) / (float)m_Hand.Cards.size();
		glm::vec3 origin = { 5.0f - (base_padding * (float)m_Hand.Cards.size()), -2.3f, 1.0f };
		float threshold = 0.0001f;
		for (int i = 0; i < (int)m_Hand.Cards.size(); i++) {
			Flora::TransformComponent& tc = m_Hand.Cards[i].first.GetComponent<Flora::TransformComponent>();
			glm::vec3 dest_tr = { origin.x + (base_padding * (i + 1)), origin.y, origin.z + 0.01f * i };
			glm::vec3 dest_rt = { 0.0f, 0.0f, glm::radians(((float)(m_Hand.Cards.size() - (i + 1)) / (float)m_Hand.Cards.size()) * 45.0f) };
			if (m_Hand.SelectedCard == i) {
				dest_rt = glm::vec3(0.0f);
				dest_tr = { MouseCoordinates().x, MouseCoordinates().y, m_Hand.Cards.size() * 0.01f + origin.z };
			}
			if (m_Hand.CurrentSpellCardIndex == i) {
				dest_rt = glm::vec3(0.0f);
				dest_tr = { 5.0f, 0.0f, 1.0f };
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
		for (int i = 0; i < (int)m_Hand.Cards.size(); i++) {
			Flora::Entity card = m_Hand.Cards[i].first;
			Flora::SpriteRendererComponent& src = card.GetComponent<Flora::SpriteRendererComponent>();
			if (CheckHovered(card))
				src.Color = { 0.5f, 0.5f, 1.0f, 1.0f };
			else
				src.Color = glm::vec4(1.0f);
		}

		// card activation
		if (m_Hand.SelectedCard >= 0 && m_Hand.CurrentSpellCardIndex < 0) {
			Flora::Entity card = m_Hand.Cards[m_Hand.SelectedCard].first;
			if (card.GetComponent<Flora::TransformComponent>().Translation.y > -1.0f) {
				card.GetComponent<Flora::SpriteRendererComponent>().Color = { 0.5f, 1.0f, 0.5f, 1.0f };
				if (!Flora::Input::IsMouseButtonPressed(Flora::Mouse::Button0))
					ActivateCard(m_Hand.SelectedCard);
			}
		}

		// card dragging
		if (!Flora::Input::IsMouseButtonPressed(Flora::Mouse::Button0)) m_Hand.SelectedCard = -1;
		else if (m_Hand.SelectedCard < 0 && m_Hand.CurrentSpellCardIndex < 0)
			if (HoveredEntity() >= 0)
				for (int i = 0; i < (int)m_Hand.Cards.size(); i++)
					if (CheckHovered(m_Hand.Cards[i].first)) {
						m_Hand.SelectedCard = i;
					}
	}

	void BattleScene::CreateUI() {

		m_UI.PUI.ProfileBorder = CreateEntity("Player Profile Border");
		m_UI.PUI.ProfilePicture = CreateEntity("Player Profile Picture");
		m_UI.PUI.ProfileBorder.AddComponent<Flora::SpriteRendererComponent>().Path = UCG::FileUtils::Path("assets/UI/Profiles/ForestProfile.png");
		m_UI.PUI.ProfilePicture.AddComponent<Flora::SpriteRendererComponent>().Path = UCG::FileUtils::Path("assets/Profile.png");
		m_UI.PUI.ProfileBorder.GetComponent<Flora::TransformComponent>().Translation = { -5.2f, 2.65f, 998.0f };
		m_UI.PUI.ProfilePicture.GetComponent<Flora::TransformComponent>().Translation = { -5.2f, 2.65f, 998.1f };

		// health vessel top/bottm
		Flora::Entity health_vessel_top = CreateEntity("Health Vessel Top");
		Flora::Entity health_vessel_bot = CreateEntity("Health Vessel Bottom");
		Flora::SpriteRendererComponent& h_top_src = health_vessel_top.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& h_top_tc = health_vessel_top.GetComponent<Flora::TransformComponent>();
		Flora::SpriteRendererComponent& h_bot_src = health_vessel_bot.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& h_bot_tc = health_vessel_bot.GetComponent<Flora::TransformComponent>();
		h_top_src.Path = h_bot_src.Path = UCG::FileUtils::Path("assets/UI/Battle/Health Vessel.png");
		h_top_src.Type = h_bot_src.Type = Flora::SpriteRendererComponent::SpriteType::SUBTEXTURE;
		h_top_src.Columns = h_bot_src.Columns = 1;
		h_top_src.Rows = h_bot_src.Rows = 4;
		h_top_src.ColumnCoordinate = h_bot_src.ColumnCoordinate = 1;
		h_top_src.RowCoordinate = 1;
		h_bot_src.RowCoordinate = 4;
		h_top_tc.Translation = m_UI.PUI.ProfileBorder.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(-0.25f, (-0.7f) - (-1.0f * 0.125f), 0.0f);
		h_bot_tc.Translation = m_UI.PUI.ProfileBorder.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(-0.25f, (-0.7f) - (m_Stats.PlayerHealth * 0.125f), 0.0f);
		h_top_tc.Scale = h_bot_tc.Scale = { 0.5f, 0.125f, 1.0f };

		// mana vessel top/bottm
		Flora::Entity mana_vessel_top = CreateEntity("Mana Vessel Top");
		Flora::Entity mana_vessel_bot = CreateEntity("Mana Vessel Bottom");
		Flora::SpriteRendererComponent& m_top_src = mana_vessel_top.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& m_top_tc = mana_vessel_top.GetComponent<Flora::TransformComponent>();
		Flora::SpriteRendererComponent& m_bot_src = mana_vessel_bot.AddComponent<Flora::SpriteRendererComponent>();
		Flora::TransformComponent& m_bot_tc = mana_vessel_bot.GetComponent<Flora::TransformComponent>();
		m_top_src.Path = m_bot_src.Path = UCG::FileUtils::Path("assets/UI/Battle/Mana Vessel.png");
		m_top_src.Type = m_bot_src.Type = Flora::SpriteRendererComponent::SpriteType::SUBTEXTURE;
		m_top_src.Columns = m_bot_src.Columns = 1;
		m_top_src.Rows = m_bot_src.Rows = 4;
		m_top_src.ColumnCoordinate = m_bot_src.ColumnCoordinate = 1;
		m_top_src.RowCoordinate = 1;
		m_bot_src.RowCoordinate = 4;
		m_top_tc.Translation = m_UI.PUI.ProfileBorder.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(0.25f, (-0.7f) - (-1.0f * 0.125f), 0.0f);
		m_bot_tc.Translation = m_UI.PUI.ProfileBorder.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(0.25f, (-0.7f) - (m_Stats.PlayerMana * 0.125f), 0.0f);
		m_top_tc.Scale = m_bot_tc.Scale = { 0.5f, 0.125f, 1.0f };

		for (int i = 0; i < m_Stats.PlayerHealth; i++) {
			Flora::Entity vessel = CreateEntity("Health Vessel");
			Flora::SpriteRendererComponent& src = vessel.AddComponent<Flora::SpriteRendererComponent>();
			Flora::TransformComponent& tc = vessel.GetComponent<Flora::TransformComponent>();
			src.Path = UCG::FileUtils::Path("assets/UI/Battle/Health Vessel.png");
			src.Type = Flora::SpriteRendererComponent::SpriteType::SUBTEXTURE;
			src.Columns = 1;
			src.Rows = 4;
			src.ColumnCoordinate = 1;
			src.RowCoordinate = 2;
			tc.Translation = m_UI.PUI.ProfileBorder.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(-0.25f, (-0.7f) - (i * 0.125f), 0.0f);
			tc.Scale = { 0.5f, 0.125f, 1.0f };
			m_UI.PUI.HealthVessel.push_back(vessel);
		}

		for (int i = 0; i < m_Stats.PlayerMana; i++) {
			Flora::Entity vessel = CreateEntity("Mana Vessel");
			Flora::SpriteRendererComponent& src = vessel.AddComponent<Flora::SpriteRendererComponent>();
			Flora::TransformComponent& tc = vessel.GetComponent<Flora::TransformComponent>();
			src.Path = UCG::FileUtils::Path("assets/UI/Battle/Mana Vessel.png");
			src.Type = Flora::SpriteRendererComponent::SpriteType::SUBTEXTURE;
			src.Columns = 1;
			src.Rows = 4;
			src.ColumnCoordinate = 1;
			src.RowCoordinate = 2;
			tc.Translation = m_UI.PUI.ProfileBorder.GetComponent<Flora::TransformComponent>().Translation + glm::vec3(0.25f, (-0.7f) - (i * 0.125f), 0.0f);
			tc.Scale = { 0.5f, 0.125f, 1.0f };
			m_UI.PUI.ManaVessel.push_back(vessel);
		}
	}

	void BattleScene::UpdateBattleState(Flora::Timestep ts) {
		bool fieldaction = false;

		switch (m_Stats.State) {
		case BattleState::PREPLAYER:
			for (size_t i = 0; i < m_BoardObjects.VFXs.size(); i++) {
				if (!m_BoardObjects.VFXs[i]->TurnUpdate()) {
					m_BoardObjects.VFXs[i]->Kill();
					delete m_BoardObjects.VFXs[i];
					m_BoardObjects.VFXs.erase(m_BoardObjects.VFXs.begin() + i);
				}
			}
			DrawCard();
			m_Stats.State = BattleState::PLAYER;
			break;
		case BattleState::PLAYER:
			break;
		case BattleState::ENDPLAYER:
			for (auto monster : m_BoardObjects.Monsters)
				monster->Prime();
			m_Stats.State = BattleState::POSTPLAYER;
			break;
		case BattleState::POSTPLAYER:
			fieldaction = false;
			for (auto monster : m_BoardObjects.Monsters) {
				if (monster->InAction()) {
					fieldaction = true;
					monster->UpdateActions(ts);
					break;
				}
			}
			if (!fieldaction) m_Stats.State = BattleState::PREOPPONENT;
			break;
		case BattleState::PREOPPONENT:
			m_Stats.State = BattleState::PREPLAYER;
			break;
		default:
			break;
		}
	}

	void BattleScene::DrawCard() {
		std::pair<Flora::Entity, Card> pair = { CreateEntity("Card"), Player::GetDeck()->Draw() };
		pair.first.AddComponent<Flora::SpriteRendererComponent>().Path = UCG::FileUtils::Path("assets/Card.png");
		pair.first.GetComponent<Flora::TransformComponent>().Translation = { 0.0f, 0.0f, 1.0f };
		pair.first.GetComponent<Flora::TransformComponent>().Scale = { 1.0f, 1.5f, 1.0f };
		
		Flora::ChildComponent& cc = pair.first.AddComponent<Flora::ChildComponent>();
		for (int i = 0; i < pair.second.Cost; i++) {
			Flora::Entity mana_node = CreateEntity("Mana Node");
			mana_node.AddComponent<Flora::SpriteRendererComponent>().Path = UCG::FileUtils::Path("assets/Cards/Decorators/Mana Badge.png");
			mana_node.GetComponent<Flora::TransformComponent>().Translation = { -0.29f + (i%7 * 0.1f), -0.27f - (i/7 * 0.1f), 0.001f };
			mana_node.GetComponent<Flora::TransformComponent>().Scale = { 8.0f/72.0f, 8.0f/108.0f, 1.0f };
			mana_node.AddComponent<Flora::ParentComponent>().Parent = pair.first;
			cc.AddChild(mana_node);
		}

		Flora::Entity splash_art = CreateEntity("Splash Art");
		splash_art.AddComponent<Flora::SpriteRendererComponent>().Path = UCG::FileUtils::Path("assets/Cards/Splash Art/" + pair.second.Name + ".png");
		splash_art.GetComponent<Flora::TransformComponent>().Translation = { 0.0f, 0.0f, 0.002f };
		splash_art.AddComponent<Flora::ParentComponent>().Parent = pair.first;
		cc.AddChild(splash_art);

		m_Hand.Cards.push_back(pair);
	}

	void BattleScene::DevCall() {
		if (false) {
			if (HoveredEntity() >= 0) {
				for (int r = 0; r < (int)m_BoardObjects.Tiles.size(); r++)
					for (int c = 0; c < (int)m_BoardObjects.Tiles[0].size(); c++)
						if ((uint32_t)HoveredEntity() == (uint32_t)(m_BoardObjects.Tiles[r][c].Contents.Body))
							FL_CORE_INFO("r: {}, c: {}", r, c);
			}
		}
		static bool d_down = Flora::Input::IsKeyPressed(Flora::Key::D);
		if (d_down) {
			if (!Flora::Input::IsKeyPressed(Flora::Key::D)) {
				if (Flora::Input::IsKeyPressed(Flora::Key::T)) {
					m_Stats.State = BattleState::ENDPLAYER;
				}
				else {
					DrawCard();
				}
				d_down = false;
			} 
		}
		else if (Flora::Input::IsKeyPressed(Flora::Key::D)) {
			d_down = true;
		}
	}

	bool BattleScene::CheckHovered(Flora::Entity entity) {
		int64_t he = HoveredEntity();
		if (he >= 0) {
			if ((uint32_t)entity == (uint32_t)he) return true;
			if (entity.HasComponent<Flora::ChildComponent>())
				for (auto child : entity.GetComponent<Flora::ChildComponent>().Children)
					if ((uint32_t)child == (uint32_t)he)
						return true;
		}
		return false;
	}

	bool BattleScene::ActivateCard(int cardIndex) {
		Card info = m_Hand.Cards[cardIndex].second;
		if (m_Stats.PlayerMana >= info.Cost) {
			m_Hand.CurrentSpell = info.ID;
			m_Hand.CurrentSpellCardIndex = cardIndex;
			return true;
		}
		return false;
	}

	void BattleScene::ConsumeCard() {
		Flora::Entity card = m_Hand.Cards[m_Hand.CurrentSpellCardIndex].first;
		Card info = m_Hand.Cards[m_Hand.CurrentSpellCardIndex].second;
		m_Stats.PlayerMana -= info.Cost;
		m_Hand.Cards.erase(m_Hand.Cards.begin() + m_Hand.CurrentSpellCardIndex);
		m_Hand.CurrentSpellCardIndex = -1;
		DestroyEntity(card);
	}

	void BattleScene::UpdateUI() {
		// Player Health
		for (int i = 0; i < (int)m_UI.PUI.HealthVessel.size(); i++) {
			m_UI.PUI.HealthVessel[m_UI.PUI.HealthVessel.size() - 1 - i].GetComponent<Flora::SpriteRendererComponent>().RowCoordinate = (i + 1) <= m_Stats.PlayerHealth ? 3 : 2;
		}

		// Player Mana
		for (int i = 0; i < (int)m_UI.PUI.ManaVessel.size(); i++) {
			m_UI.PUI.ManaVessel[m_UI.PUI.ManaVessel.size() - 1 - i].GetComponent<Flora::SpriteRendererComponent>().RowCoordinate = (i + 1) <= m_Stats.PlayerMana ? 3 : 2;
		}
	}

	void BattleScene::CleanBoard() {
		for (int r = 0; r < (int)m_BoardObjects.Tiles.size(); r++) {
			for (int c = 0; c < (int)m_BoardObjects.Tiles.size(); c++) {
				DeepTint(m_BoardObjects.Tiles[r][c].Contents.Body, glm::vec4(1.0f));
			}
		}
	}

	bool BattleScene::ValidBoardCoord(TileRef tile) {
		if (tile.r >= (int)m_BoardObjects.Tiles.size()) return false;
		if (tile.r < 0) return false;
		if (tile.c >= (int)m_BoardObjects.Tiles[0].size()) return false;
		if (tile.c < 0) return false;
		return true;
	}

	std::vector<TileObj> BattleScene::NexusWorkingSet(bool playernexus, int radius) {
		std::string tilestr = playernexus ? "P" : "O";
		int row = 0;
		int col = 0;
		for (int r = 0; r < (int)m_BoardObjects.Tiles.size(); r++) {
			for (int c = 0; c < (int)m_BoardObjects.Tiles[0].size(); c++) {
				if (m_BoardObjects.Tiles[r][c].Contents.Body.GetComponent<Flora::TagComponent>().Tag == tilestr) {
					row = r;
					col = c;
					break;
				}
			}
		}
		std::vector<TileObj> workingset;
		for (int r = -1 * radius; r <= radius; r++) {
			for (int c = -1 * radius; c <= radius; c++) {
				if (ValidBoardCoord(TileRef{row + r, col + c})) {
					workingset.push_back(m_BoardObjects.Tiles[row + r][col + c].Contents);
				}
			}
		}
		return workingset;
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
		if (firstpass && m_Hand.CurrentSpell != CardID::NONE) {
			if (mousereleased) {
				firstpass = false;
				return;
			}
		}

		static VFX* vfx = nullptr;

		#define ENDSPELL() {m_Hand.CurrentSpell = CardID::NONE; firstpass = true; m_Hand.CurrentSpellCardIndex = -1; CleanBoard();}
		switch (m_Hand.CurrentSpell) {
		case CardID::SMITE:
			if (vfx == nullptr) {
				if (!SelectTile(mousereleased, std::vector<TileObj>(), [this](auto scene_context, auto& tile) {
					vfx = new LightningCloud();
					vfx->Initialize(scene_context, tile);
					ConsumeCard();
				}, (TileSelectFlag)TileSelectFlags::OCCUPIED)) ENDSPELL();
			} else {
				if (vfx->Activate()) {
					Monster* occupied_mon = GetMonster(vfx->GetTile());
					if (occupied_mon) occupied_mon->Damage(1);
				}
				if (!vfx->Update()) {
					delete vfx;
					vfx = nullptr;
					ENDSPELL();
				}
			}
			break;
		case CardID::GOBLIN:
			if (!SelectTile(mousereleased, NexusWorkingSet(), [this](auto scene_context, auto& tile) {
				Monster* goblin = new Goblin();
				goblin->Initialize(scene_context, tile);
				m_BoardObjects.Monsters.push_back(goblin);
				ConsumeCard();
				ENDSPELL();
			}, (TileSelectFlag)(TileSelectFlags::DIRT | TileSelectFlags::UNOCCUPIED))) ENDSPELL();
			break;
		case CardID::SLIME:
			if (!SelectTile(mousereleased, std::vector<TileObj>(), [this](auto scene_context, auto& tile) {
				Monster* slime = new Slime();
				slime->Initialize(scene_context, tile);
				m_BoardObjects.Monsters.push_back(slime);
				ConsumeCard();
				ENDSPELL();
			}, (TileSelectFlag)(TileSelectFlags::DIRT | TileSelectFlags::UNOCCUPIED))) ENDSPELL();
			break;
		case CardID::METEOR:
			if (vfx == nullptr) {
				if (!SelectTile(mousereleased, std::vector<TileObj>(), [this](auto scene_context, auto& tile) {
					vfx = new Meteor();
					vfx->Initialize(scene_context, tile);
					ConsumeCard();
				}, (TileSelectFlag)(TileSelectFlags::DIRT | TileSelectFlags::FOREST | TileSelectFlags::MOUNTAIN))) ENDSPELL();
			}
			else {
				if (vfx->Activate()) {	
					Monster* occupied_mon = GetMonster(vfx->GetTile());
					if (occupied_mon) occupied_mon->Damage(3);
					else if (vfx->GetTile().Contents.Physical->Type() != BuildingType::EMPTY) vfx->GetTile().Contents.Physical->Damage(1);
				}
				if (!vfx->Update()) {
					delete vfx;
					vfx = nullptr;
					ENDSPELL();
				}
			}
			break;
		case CardID::SANCTUARY:
			if (!SelectTile(mousereleased, std::vector<TileObj>(), [this](auto scene_context, auto& tile) {
				m_BoardObjects.VFXs.push_back(new Sanctuary());
				m_BoardObjects.VFXs[m_BoardObjects.VFXs.size() - 1]->Initialize(scene_context, tile);
				ConsumeCard();
				ENDSPELL();
			}, (TileSelectFlag)(TileSelectFlags::DIRT | TileSelectFlags::FOREST | TileSelectFlags::MOUNTAIN | TileSelectFlags::WATER))) ENDSPELL();
			break;
		default:
			break;
		}
		#undef ENDSPELL
	}

}

//NEXT:
/*
- meteor card that does what smite does right now, but can burn forests and break mountains (implement objects (not monsters but damageable)
- integrate monsters killing nexus (nexus is technically a monster object!)
- redo health, mana, player border
- enemy border and health/mana
- integrate turns
- if needles done:
	- implement needles
	- implem,ent multiplayer ui
- else:
	- just start creating cards and tuning up UI
*/

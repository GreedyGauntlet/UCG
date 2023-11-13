#include "BattleScene.h"
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
		ResetBoard(test_map);
		m_Camera = SceneUtils::MainCamera();
		m_Cursor = CreateEntity("Cursor");
		m_Cursor.AddComponent<Flora::CircleRendererComponent>();
		m_Cursor.GetComponent<Flora::TransformComponent>().Translation.z = 998.0f;
		m_Cursor.GetComponent<Flora::TransformComponent>().Scale = { 0.1f, 0.1f, 0.1f };
	}

	void BattleScene::Update(Flora::Timestep ts) {
		m_Cursor.GetComponent<Flora::TransformComponent>().Translation.x = MouseCoordinates().x;
		m_Cursor.GetComponent<Flora::TransformComponent>().Translation.y = MouseCoordinates().y;
		GenericUpdate(ts);
		UpdateBoard();
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

		//FL_CORE_INFO("{}, {}", translation.x, translation.y);
		if (translation.x > tiletr.x - 0.5f &&
			translation.x < tiletr.x + 0.5f &&
			translation.y > tiletr.y - 0.5f &&
			translation.y < tiletr.y) {
			float rely = translation.y - tiletr.y;
			float relx = translation.x - tiletr.x;
			FL_CORE_INFO("{}, {}", relx, rely);
			return true;
		}
		return false;
	}

}
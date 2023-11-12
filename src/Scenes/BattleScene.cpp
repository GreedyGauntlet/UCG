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
	}

	void BattleScene::Stop() {

	}

	void BattleScene::ResetBoard(const Board board) {
		int b_width = board.size();
		int b_height = board[0].size();
		for (int r = 0; r < b_width; r++) {
			for (int c = 0; c < b_height; c++) {
				Flora::Entity tile = CreateEntity("Tile");
				tile.AddComponent<Flora::SpriteRendererComponent>();
				tile.GetComponent<Flora::SpriteRendererComponent>().Path = BoardUtils::TilePath(board[r][c]);
				tile.GetComponent<Flora::TransformComponent>().Translation.x = -0.5f * r + (0.5f * c);
				tile.GetComponent<Flora::TransformComponent>().Translation.y = -0.25f * c - (0.25f * r);
			}
		}
	}

}
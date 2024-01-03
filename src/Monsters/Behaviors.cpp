#include "Behaviors.h"
#include "limits.h"

namespace UCG {
  
	TileSet Behaviors::Target(BattleScene* context, Monster& subject, ObjectSelectFlags flags, uint32_t numTargets, const TileSet& included) {
		TileRef subject_tr = subject.GetTileRef();
		TileSet targets;
		TileSet workingset;
		if (included.size() <= 0) {
			int rows = (int)(context->GetBoardTiles().size());
			int cols = (int)(context->GetBoardTiles()[0].size());
			for (int r = 0; r < rows; r++)
				for (int c = 0; c < cols; c++)
					workingset.push_back({r, c});
		} else workingset = included;

		std::set<TileRef> excluded;
		excluded.insert(subject_tr);

		while (targets.size() < numTargets) {
			TileSet eval_targets;
			for (auto tile : workingset) {
				if (excluded.find(tile) != excluded.end()) continue;
				
				if (!(flags & ObjectSelectFlags::EMPTY)) {
					bool viable = context->TileOccupied(tile);
					if (!viable) continue;

					Monster* occupant = context->GetMonster(tile);
					viable = ((flags & ObjectSelectFlags::FRIENDLYFIRE) || subject.GetAllegiance() == Allegiances::NONE) ? true : subject.GetAllegiance() != occupant->GetAllegiance();
					if (!viable) continue;
				}

				if (flags & ObjectSelectFlags::LINEAR) {
					if (flags & ObjectSelectFlags::DIRECTIONAL) {
						switch (subject.Status().Direction) {
						case Orientation::DR:
							if (tile.r == subject_tr.r && tile.c > subject_tr.c) {
								eval_targets.push_back(tile);
								continue;
							}
							break;
						case Orientation::UR:
							if (tile.r < subject_tr.r && tile.c == subject_tr.c) {
								eval_targets.push_back(tile);
								continue;
							}
							break;
						case Orientation::UL:
							if (tile.r == subject_tr.r && tile.c < subject_tr.c) {
								eval_targets.push_back(tile);
								continue;
							}
							break;
						default:
							if (tile.r > subject_tr.r && tile.c == subject_tr.c) {
								eval_targets.push_back(tile);
								continue;
							}
							break;	
						}
					} else {
						if (subject_tr.r == tile.r || subject_tr.c == tile.c) {
							eval_targets.push_back(tile);	
							continue;
						}
					}
				}
				
				if (flags & ObjectSelectFlags::DIAGONAL) {
					bool valid_diag = (tile.r - tile.c == subject_tr.r - subject_tr.c) || (tile.r + tile.c == subject_tr.r + subject_tr.c);
					if (flags & ObjectSelectFlags::DIRECTIONAL) {
						switch (subject.Status().Direction) {
						case Orientation::DR:
							if (tile.c > subject_tr.c && valid_diag) {
								eval_targets.push_back(tile);
								continue;
							}
							break;
						case Orientation::UR:
							if (tile.r < subject_tr.r && valid_diag) {
								eval_targets.push_back(tile);
								continue;
							}
							break;
						case Orientation::UL:
							if (tile.c < subject_tr.c && valid_diag) {
								eval_targets.push_back(tile);
								continue;
							}
							break;
						default:
							if (tile.r > subject_tr.r && valid_diag) {
								eval_targets.push_back(tile);
								continue;
							}
							break;
						}
					} else {
						if (valid_diag) {
							eval_targets.push_back(tile);
							continue;
						}
					}
				}
				
				if (!((flags & ObjectSelectFlags::LINEAR) || (flags & ObjectSelectFlags::DIAGONAL))) eval_targets.push_back(tile);
			}

			if (eval_targets.size() > 0) {
				if (flags & ObjectSelectFlags::NEAR || flags & ObjectSelectFlags::FAR) {
					TileRef nearest_tile;
					TileRef farthest_tile;
					int near_val = INT_MAX;
					int far_val = -1;
					for (auto tile : eval_targets) {
						int a = subject_tr.r - tile.r;
						int b = subject_tr.c - tile.c;
						float distance = std::sqrt((a*a) + (b*b));
						if (distance < near_val) {
							near_val = distance;
							nearest_tile = tile;
						}
						if (distance > far_val) {
							far_val = distance;
							farthest_tile = tile;
						}
					}
					TileRef finaltile = flags & ObjectSelectFlags::NEAR ? nearest_tile : farthest_tile;
					excluded.insert(finaltile);
					targets.push_back(finaltile);
				} else {
					excluded.insert(eval_targets[0]);
					targets.push_back(eval_targets[0]);
				}
			} else break;
		}
		return targets;
	}

}

#include "Behaviors.h"

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
			bool evaluate = (flags & ObjectSelectFlags::NEAR) || (flags & ObjectSelectFlags::FAR);
			TileSet eval_targets;
			for (auto tile : workingset) {
				if (excluded.find(tile) != excluded.end()) continue;
				// is it occupied?
				// are they targetable (friendly fire or enemy)
				// are they in our LOS
				bool viable = !context->TileOccupied(tile);
				if (!viable) continue;

				Monster* occupant = context->GetMonster(tile);
				viable = (flags & ObjectSelectFlags::FRIENDLYFIRE) ? true : subject.GetAllegiance() != occupant->GetAllegiance();
				if (!viable) continue;

				if (flags & ObjectSelectFlags::LINEAR) {
					if (flags & ObjectSelectFlags::DIRECTIONAL) {
						bool valid
						switch (subject.Status().Direction) {
						case Orientation::DR:
							if (tile.first == subject_tr.first && tile.second > subject_tr.second) {
								eval_targets.push_back(tile);
								continue;
							}
							break;
						case Orientation::UR:
							if (tile.first < subject_tr.first && tile.second == subject_tr.second) {
								eval_targets.push_back(tile);
								continue;
							}
							break;
						case Orientation::UL:
							if (tile.first == subject_tr.first && tile.second < subject_tr.second) {
								eval_targets.push_back(tile);
								continue;
							}
							break;
						default:
							if (tile.first > subject_tr.first && tile.second == subject_tr.second) {
								eval_targets.push_back(tile);
								continue;
							}
							break;	
						}
					} else {
						if (subject_tr.first == tile.first || subject_tr.second == tile.second) {
							eval_targets.push_back(tile);	
							continue;
						}
					}
				}
				
				if (flags & ObjectSelectFlags::DIAGONAL) {
						
				}
				
				if (!((flags & ObjectSelectFlags::LINEAR) || (flags & ObjectSelectFlags::DIAGONAL))) eval_targets.push_back(tile);
			}

			if (eval_targets.size() > 0) {
				if (flags & ObjectSelectFlags::NEAR || flags & ObjectSelectFlags::FAR) {
					// dont forget to TODO lol	
				} else {
					excluded.insert(eval_targets[0]);
					targets.push_back(eval_targets[0]);
				}
			} else break;
		}
		return targets;
	}

}

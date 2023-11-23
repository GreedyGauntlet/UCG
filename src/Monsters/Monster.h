#pragma once
#include "Flora.h"
#include "Flora/Scene/Scene.h"
#include "../Scenes/GameScene.h"

namespace UCG {
	enum class Orientation {
		NONE,
		DL,
		DR,
		UL,
		UR
	};

	enum class AnimationState {
		INVALID,
		SPAWN,
		DEATH,
		IDLE,
		ATTACK,
		MOVE,
		ROTATE
	};

	enum class Action {
		ATTACK,
		IDLE,
		MOVE,
		ROTATE_R,
		ROTATE_L
	};

	typedef std::tuple<int, int, int> Animation; // start frame, end frame, fps
	typedef std::tuple<AnimationState, Orientation> AnimationCommand;
	typedef std::pair<int, int> TileRef; // row, column

	struct AnimationQueue {
		std::vector<AnimationCommand> Queue;
		AnimationCommand CurrentAnimation = { AnimationState::INVALID, Orientation::NONE };
		float AnimationTime = 0.0f;
	};

	struct ActionQueue {
		std::vector<Action> Queue;
		Action CurrentAction = Action::IDLE;
		float ActionTime = 0.0f;
		float TimeThreshold = 0.0f;
	};

	struct AnimationMap {
		Animation Spawn        = { 0, 0, 0 }; // spawn in (default facing DL)
		Animation DeathDown    = { 0, 0, 0 }; // death (default facing DL)
		Animation DeathUp      = { 0, 0, 0 }; // death (default facing UL)
		Animation IdleDown     = { 0, 0, 0 }; // idle (default facing DL)
		Animation IdleUp       = { 0, 0, 0 }; // idle (default facing UL)
		Animation AttackUp     = { 0, 0, 0 }; // attack (default facing UL)
		Animation AttackDown   = { 0, 0, 0 }; // attack (default facing DL)
		Animation MoveUp       = { 0, 0, 0 }; // move (center to UL)
		Animation MoveDown     = { 0, 0, 0 }; // move (center to DL)
		Animation RotateUp     = { 0, 0, 0 }; // rotate (DL to UL)
		Animation RotateDown   = { 0, 0, 0 }; // rotate (UL to DL)
		Animation RotateTop    = { 0, 0, 0 }; // rotate (UR to UL)
		Animation RotateBottom = { 0, 0, 0 }; // rotate (DR to DL)
	};

	struct MonsterStatus {
		float RelativeTime = 0.0f;
		int Health = 1;
		int MaxHealth = 1;
		bool Damaged = false;
		bool Dead = false;
		Orientation Orientation = Orientation::DL;
	};

	class Monster {
	public:
		virtual ~Monster() = default;
		virtual void Initialize(GameScene* context, Flora::Entity tile) = 0;
		virtual void Update(Flora::Timestep ts);
		virtual void Destroy();
		virtual void Damage(int damage);
	public:
		Flora::Entity Body() { return m_Body; }
		bool Alive() { return !m_Status.Dead; }
		Flora::Entity Tile();
		TileRef GetTileRef(Flora::Entity tile);
		TileRef GetTileRef() { return m_Tile; }
	protected:
		virtual void DamageAnim(Flora::Timestep ts);
		virtual void DeathAnim(Flora::Timestep ts);
		virtual void DrawHealth();
	protected:
		void QueueAnimation(AnimationCommand command);
		void OverrideAnimation(AnimationCommand command);
		void UpdateAnimation(Flora::Timestep ts);
		Animation GetAnimation(AnimationCommand command);
		float GetAnimationTime(Animation anim);
	private:
		bool ValidAnimation(Animation anim);
	public:
		virtual void StartTurn();
		virtual void UpdateActions(Flora::Timestep ts);
	protected:
		virtual bool Attack(Flora::Timestep ts);
		virtual bool Move(Flora::Timestep ts);
		virtual bool RotateRight(Flora::Timestep ts);
		virtual bool RotateLeft(Flora::Timestep ts);
	protected:
		void PushAction(Action action);
	public:
		bool InAction() { return m_ActionQueue.Queue.size() > 0; }
	protected:
		GameScene* m_Context;
		Flora::Entity m_Body;
		TileRef m_Tile;
	protected:
		AnimationMap m_Animations;
		AnimationQueue m_AnimationQueue;
		ActionQueue m_ActionQueue;
		MonsterStatus m_Status;
	};
}
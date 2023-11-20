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
		SPAWN,
		DEATH,
		IDLE,
		ATTACK,
		MOVE,
		ROTATE
	};

	typedef std::tuple<int, int, int> Animation; // start frame, end frame, fps
	typedef std::tuple<AnimationState, Orientation> AnimationCommand;

	struct AnimationQueue {
		std::vector<AnimationCommand> Queue;
		AnimationCommand CurrentAnimation = { AnimationState::SPAWN, Orientation::NONE };
		float AnimationTime = 0.0f;
	};

	struct AnimationMap {
		Animation Spawn        = { 0, 0, 0 }; // spawn in (default facing DL)
		Animation Death        = { 0, 0, 0 }; // death (default facing DL)
		Animation IdleDown     = { 0, 0, 0 }; // idle (default facing DL)
		Animation IdleUp       = { 0, 0, 0 }; // idle (default facing UL)
		Animation AttackUp     = { 0, 0, 0 }; // attack (default facing UL)
		Animation AttackDown   = { 0, 0, 0 }; // attack (default facing DL)
		Animation MoveUp       = { 0, 0, 0 }; // move (center to UL)
		Animation MoveDown     = { 0, 0, 0 }; // move (center to DL)
		Animation RotateUp     = { 0, 0, 0 }; // rotate (DL to UL)
		Animation RotateDown   = { 0, 0, 0 }; // rotate (UL to DL)
		Animation RotateTop    = { 0, 0, 0 }; // rotate (UL to UR)
		Animation RotateBottom = { 0, 0, 0 }; // rotate (DL to DR)
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
		virtual void Update(Flora::Timestep ts) = 0;
		virtual void Destroy();
		virtual void Damage(int damage);
	public:
		Flora::Entity Body() { return m_Body; }
		Flora::Entity Tile() { return m_Tile; }
		bool Alive() { return !m_Status.Dead; }
	protected:
		virtual void DamageAnim(Flora::Timestep ts);
		virtual void DeathAnim(Flora::Timestep ts);
		virtual void DrawHealth();
	protected:
		virtual void QueueAnimation(AnimationCommand command);
		virtual void OverrideAnimation(AnimationCommand command);
		virtual void UpdateAnimation(Flora::Timestep ts);
	protected:
		GameScene* m_Context;
		Flora::Entity m_Body;
		Flora::Entity m_Tile;
	protected:
		AnimationMap m_Animations;
		AnimationQueue m_AnimationQueue;
		MonsterStatus m_Status;
	};
}
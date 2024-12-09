//-----------------------------------------------------------------------------------------------
// Game.hpp
//

//-----------------------------------------------------------------------------------------------
#pragma once
#include <Engine/Math/Vec2.hpp>
#include "Game/GameCommon.hpp"
#include "Game/Asteroid.hpp"
#include "Game/Beetle.hpp"
#include "Game/Box.hpp"
#include "Game/Bullet.hpp"
#include "Game/Debris.hpp"
#include "Game/PlayerShip.hpp"
#include "Game/Wasp.hpp"

//-----------------------------------------------------------------------------------------------
class Camera;
class ScoreBoardHandler;
class UIHandler;
// class Asteroid;
// class Beetle;
// class Box;
// class Bullet;
// class Debris;
// class PlayerShip;
// class Wasp;

//-----------------------------------------------------------------------------------------------
class Game
{
public:
	Game();
	~Game();
	//-----------------------------------------------------------------------------------------------
	void Update(float deltaSeconds);
	void Render();
	void DebugRender() const;
	void ResetData();
	//-----------------------------------------------------------------------------------------------
	// high-level game mechanics(e.g.levels / waves, spawning)
	void         SpawnBullet(Vec2 const& position, float orientationDegrees);
	PlayerShip*  GetPlayerShip() const;
	void         MarkAllEntityAsDeadAndGarbage();
	void         SetAttractMode(bool isAttractMode);
	bool         IsAttractMode() const;
	void         SetPlayerNameInputMode(bool isPlayerNameInputMode);
	void         SetPlayerShipIsReadyToSpawnBullet(bool isReadyToSpawnBullet) const;
	bool         IsPlayerNameInputMode() const;
	int          GetHighScore() const;

private:
	void SpawnPlayerShip();
	void SpawnBeetle(Vec2 const& position);
	void SpawnWasp(Vec2 const& position);
	void SpawnAsteroid(Vec2 const& position);
	void SpawnDebris(Vec2 const& position, Vec2 const& velocity, float radius, Rgba8 color);
	void SpawnDebrisCluster(Vec2 const& position, Vec2 const& velocity, int numDebris, float radius, Rgba8 color);
	void SpawnBox(Vec2 const& position);
	void SpawnBoxCluster();
	void SpawnRandomEnemy(int boxIndex);
	void UpdateEntities(float deltaSeconds);
	void UpdateFromKeyBoard();
	void UpdateFromController();
	void RenderEntities() const;
	void DebugRenderEntities() const;

	// entity-vs-entity interactions (e.g. physics, damage)
	void HandleEntityCollision();
	void HandleCollisionBetweenPlayerShipAndBox();
	void HandleEntityIsOffScreen() const;

	Vec2 GetOffScreenPosition(float entityCosmeticRadius) const;

	void DeleteGarbageEntities();
	void SpawnEnemiesForCurrentWave();
	bool AreAllEnemiesDead() const;
	void DoShakeCamera(float deltaSeconds);
	void ResetCamera() const;
	// bool IsAlive(Entity* entity);
	// void CheckBulletVsEnemy(Bullet& bullet, entity& enemy);
	// void CheckBulletVsEnemyList(Bullet* bullet, int listMaxSize, Entity** enemyList)

	// Entity* RaycastSwampDiscVsEnemies(Vec2 const& startPos, Vec2 const& fwdNormal, float maxLength, float discRadius);

	// #TODO: Entity* Lists / dynamic_cast<Asteroid*>(m_asteroid[asteroidIndex])
	PlayerShip*        m_playerShip                   = nullptr; // Just one player ship (for now...)
	Bullet*            m_bullets[MAX_BULLETS_NUM]     = {};      // Fixed number of asteroid �slots�; nullptr if unused.
	Asteroid*          m_asteroids[MAX_ASTEROIDS_NUM] = {};      // The �= {};� syntax initializes the array to zeros.
	Beetle*            m_beetle[MAX_BEETLE_NUM]       = {};
	Wasp*              m_wasp[MAX_WASP_NUM]           = {};
	Debris*            m_debris[MAX_DEBRIS_NUM]       = {};
	Box*               m_boxes[MAX_BOX_NUM]           = {};
	Camera*            m_worldCamera                  = nullptr;
	Camera*            m_screenCamera                 = nullptr;
	int                m_currentWave                  = 0;
	float              m_timeSinceDeath               = 0.f;
	int                m_playerShipHealth             = MAX_PLAYER_SHIP_HEALTH;
	bool               m_isAttractMode                = true;
	bool               m_isPlayerNameInputMode        = false;
	bool               m_isHighScoreboardMode         = false;
	bool               m_isDebugRendering             = false;
	UIHandler*         m_theUIHandler                 = nullptr;
	float              m_shakeIntensity               = 5.f; // Current intensity of the shake
	float              m_shakeDuration                = 20.f;  // Time remaining for the shake
	Vec2               m_baseCameraPos                = Vec2::ZERO;
	float              m_accumulatedTime              = 0.f;
	ScoreBoardHandler* m_theScoreBoardHandler         = nullptr;
	float              m_debrisVelocityRate = 0.5f;
	int                m_highScore = 0;
};

//-----------------------------------------------------------------------------------------------
// Game.hpp
//

//-----------------------------------------------------------------------------------------------
#pragma once
#include <Engine/Math/Vec2.hpp>
#include "EventHandler.hpp"
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
	EventHandler OnCustomEvent;

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
	Bullet*            m_bullets[MAX_BULLETS_NUM]     = {};      // Fixed number of asteroid “slots”; nullptr if unused.
	Asteroid*          m_asteroids[MAX_ASTEROIDS_NUM] = {};      // The “= {};” syntax initializes the array to zeros.
	Beetle*            m_beetle[MAX_BEETLE_NUM]       = {};
	Wasp*              m_wasp[MAX_WASP_NUM]           = {};
	Debris*            m_debris[MAX_DEBRIS_NUM]       = {};
	Box*               m_boxes[MAX_BOX_NUM]           = {};
	Camera*            m_worldCamera;
	Camera*            m_screenCamera;
	int                m_currentWave;
	float              m_timeSinceDeath;
	int                m_playerShipHealth;
	bool               m_isAttractMode;
	bool               m_isPlayerNameInputMode;
	bool               m_isHighScoreboardMode;
	bool               m_isDebugRendering;
	UIHandler*         m_theUIHandler;
	float              m_shakeIntensity; // Current intensity of the shake
	float              m_shakeDuration;  // Time remaining for the shake
	Vec2               m_baseCameraPos;
	float              m_accumulatedTime;
	ScoreBoardHandler* m_theScoreBoardHandler;
	float              m_debrisVelocityRate;
	int                m_highScore;
};

#pragma once
#include <string>

#include "Game/GameCommon.hpp"
#include "Game/Entities/Entity.hpp"

struct PlayerScore;

//-----------------------------------------------------------------------------------------------
class PlayerShip : public Entity
{
public:
	PlayerShip(Game* game, const Vec2& position, float orientationDegrees, int health, bool isReadyToSpawnBullet);
	~PlayerShip() override;

	void Update(float deltaSeconds) override;
	void Render() const override;
	void DebugRender() const override;

	bool  IsReadyToSpawnBullet(bool isReady);
	void  UpdateFromController();
	Vec2& GetPositionAndSet();
	Vec2& GetVelocityAndSet();
	void  SetPosition(const Vec2& targetPosition);
	int   GetPlayerScore() const;
	void  SetPlayerScore(int delta);

private:
	void BounceOffWall();
	void InitializeLocalVerts() override;
	// #TODO: static void InitializeLocalVerts(Vertex_PCU* vertsToFillIn, Rgba8 const& color)
	void UpdateFromKeyBoard();


	Vertex_PCU m_localVerts[PLAYER_SHIP_VERTS_NUM];

	bool  m_isTurningLeft;
	bool  m_isTurningRight;
	bool  m_isThrusting;
	bool  m_isReadyToSpawnBullet;
	float m_thrustRate;
	int   m_playerScore;
};

//----------------------------------------------------------------------------------------------------
// PlayerShip.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
class PlayerShip : public Entity
{
public:
    PlayerShip(Vec2 const& position, float orientationDegrees, int health, bool isReadyToSpawnBullet);

    void Update(float deltaSeconds) override;
    void Render() const override;
    void DebugRender() const override;

    bool  IsReadyToSpawnBullet(bool isReady);
    void  UpdateFromController();
    Vec2& GetPositionAndSet();
    Vec2& GetVelocityAndSet();
    void  SetPosition(Vec2 const& targetPosition);
    int   m_score = 0;

private:
    void BounceOffWall();
    void InitializeLocalVerts() override;
    void UpdateFromKeyBoard();

    Vertex_PCU m_localVerts[PLAYER_SHIP_VERTS_NUM];

    bool  m_isTurningLeft        = false;
    bool  m_isTurningRight       = false;
    bool  m_isThrusting          = false;
    bool  m_isReadyToSpawnBullet = false;
    float m_thrustRate           = 0.f;
};

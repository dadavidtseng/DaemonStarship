//----------------------------------------------------------------------------------------------------
// Asteroid.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
class Game;

//----------------------------------------------------------------------------------------------------
class Asteroid : public Entity
{
public:
    Asteroid(Game* game, Vec2 const& position, float orientationDegrees);

    void Update(float deltaSeconds) override;
    void Render() const override;
    void DebugRender() const override;

private:
    void InitializeLocalVerts() override;

    Vertex_PCU m_localVerts[ASTEROID_VERTS_NUM];
};

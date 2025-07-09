//----------------------------------------------------------------------------------------------------
// Debris.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
class Debris final : public Entity
{
public:
    Debris(Vec2 const& position, Vec2 const& velocity, float radius, Rgba8 color);

    void Update(float deltaSeconds) override;
    void Render() const override;
    void DebugRender() const override;

private:
    void InitializeLocalVerts() override;

    Vertex_PCU m_localVerts[DEBRIS_VERTS_NUM];
    float      m_lifetime;
    float      m_initialLifetime;
};

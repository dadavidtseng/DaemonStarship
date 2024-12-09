//----------------------------------------------------------------------------------------------------
// Wasp.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
class Wasp : public Entity
{
public:
    Wasp(Game* game, Vec2 const& position, float orientationDegrees);

    void Update(float deltaSeconds) override;
    void Render() const override;
    void DebugRender() const override;

private:
    void InitializeLocalVerts() override;

    Vertex_PCU m_localVerts[WASP_VERTS_NUM];
};

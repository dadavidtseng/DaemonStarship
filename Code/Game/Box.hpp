//----------------------------------------------------------------------------------------------------
// Box.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
class Box : public Entity
{
public:
    Box(Game* game, Vec2 const& position, float orientationDegrees);

    void  Update(float deltaSeconds) override;
    void  Render() const override;
    void  DebugRender() const override;
    AABB2 GetBoxCollider();
    void  SetPosition(const Vec2& targetPosition);

private:
    void InitializeLocalVerts() override;

    Vertex_PCU m_localVerts[BOX_VERTS_NUM];

    AABB2 m_boxCollider;
    float m_accumulatedTime;
    Vec2  m_targetPosition;
};

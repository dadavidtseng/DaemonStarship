//-----------------------------------------------------------------------------------------------
// Box.cpp
//

//-----------------------------------------------------------------------------------------------
#include "Game/Box.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Game.hpp"

//-----------------------------------------------------------------------------------------------
Box::Box(Game* game, Vec2 const& position, float const orientationDegrees)
    : Entity(game, position, orientationDegrees, Rgba8(255, 255, 255, 200)),
      m_boxCollider(position, position + Vec2(BOX_SIDE_LENGTH, BOX_SIDE_LENGTH)),
      m_accumulatedTime(0.f),
      m_targetPosition(position - Vec2(BOX_SIDE_LENGTH * 1.1f, 0.0f))
{
    m_health = 5;
    m_boxCollider.SetCenter(position + Vec2(BOX_SIDE_LENGTH / 2.f, BOX_SIDE_LENGTH / 2.f));
    Box::InitializeLocalVerts();
}

//-----------------------------------------------------------------------------------------------
Box::~Box() = default;

//-----------------------------------------------------------------------------------------------
void Box::Update(float deltaSeconds)
{
    if (m_isDead)
        return;

    m_accumulatedTime += deltaSeconds;


    float t = m_accumulatedTime / 0.5f;
    t       = GetClamped(t, 0.f, 1.f);


    m_position.x = Interpolate(m_position.x, m_targetPosition.x, t);


    m_boxCollider.SetCenter(m_position + Vec2(BOX_SIDE_LENGTH / 2.f, BOX_SIDE_LENGTH / 2.f));


    if (m_accumulatedTime >= 1.0f)
    {
        m_accumulatedTime = 0.0f;


        m_targetPosition = m_position - Vec2(BOX_SIDE_LENGTH * 1.1f, 0.0f);
    }

    // WrapPosition();
}

//-----------------------------------------------------------------------------------------------
void Box::Render() const
{
    if (m_isDead)
        return;

    Vertex_PCU tempWorldVerts[BOX_VERTS_NUM];

    for (int vertIndex = 0; vertIndex < BOX_VERTS_NUM; vertIndex++)
    {
        tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
    }

    TransformVertexArrayXY3D(BOX_VERTS_NUM, tempWorldVerts, 1.f, m_orientationDegrees, m_position);

    g_theRenderer->DrawVertexArray(BOX_VERTS_NUM, tempWorldVerts);
}

//-----------------------------------------------------------------------------------------------
void Box::DebugRender() const
{
    DebugDrawBoxRing(m_boxCollider.GetCenter(), BOX_SIDE_LENGTH / 2.f, 0.2f, DEBUG_RENDER_RED);
}

AABB2 Box::GetBoxCollider()
{
    return m_boxCollider;
}

void Box::SetPosition(const Vec2& targetPosition)
{
    m_position += targetPosition;
}

//-----------------------------------------------------------------------------------------------
void Box::InitializeLocalVerts()
{
    m_localVerts[0].m_position = Vec3(0.f, 0.f, 0);
    m_localVerts[1].m_position = Vec3(BOX_SIDE_LENGTH, 0.f, 0);
    m_localVerts[2].m_position = Vec3(0.f, BOX_SIDE_LENGTH, 0);

    m_localVerts[3].m_position = Vec3(0.f, BOX_SIDE_LENGTH, 0);
    m_localVerts[4].m_position = Vec3(BOX_SIDE_LENGTH, 0.f, 0);
    m_localVerts[5].m_position = Vec3(BOX_SIDE_LENGTH, BOX_SIDE_LENGTH, 0);

    for (Vertex_PCU& m_localVert : m_localVerts)
    {
        m_localVert.m_color = m_color;
    }
}

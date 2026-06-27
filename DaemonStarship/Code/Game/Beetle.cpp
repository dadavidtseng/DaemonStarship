//----------------------------------------------------------------------------------------------------
// Beetle.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Beetle.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Game.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/VertexUtils.hpp"

//----------------------------------------------------------------------------------------------------
Beetle::Beetle(Vec2 const& position, float const orientationDegrees)
    : Entity(position, orientationDegrees, BEETLE_COLOR)
{
    m_health         = 3;
    m_physicsRadius  = BEETLE_PHYSICS_RADIUS;
    m_cosmeticRadius = BEETLE_COSMETIC_RADIUS;
    Beetle::InitializeLocalVerts();
}

//----------------------------------------------------------------------------------------------------
void Beetle::Update(float const deltaSeconds)
{
    if (m_isDead) return;

    if (g_game->GetPlayerShip() &&
        !g_game->GetPlayerShip()->IsDead())
    {
        Vec2 playerShipPos     = g_game->GetPlayerShip()->GetPosition();
        Vec2 directionToPlayer = (playerShipPos - m_position).GetNormalized();
        m_orientationDegrees   = directionToPlayer.GetOrientationDegrees();
    }

    float const beetleSpeed = g_rng->RollRandomFloatInRange(5.f, 12.f);
    m_velocity              = Vec2::MakeFromPolarDegrees(m_orientationDegrees, beetleSpeed);
    m_position              += m_velocity * deltaSeconds;
}

//----------------------------------------------------------------------------------------------------
void Beetle::Render() const
{
    if (m_isDead) return;

    Vertex_PCU tempWorldVerts[BEETLE_VERTS_NUM];

    for (int vertIndex = 0; vertIndex < BEETLE_VERTS_NUM; vertIndex++)
    {
        tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
    }

    TransformVertexArrayXY3D(BEETLE_VERTS_NUM, tempWorldVerts, 1.f, m_orientationDegrees, m_position);

    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->BindTexture(nullptr);
    g_renderer->DrawVertexArray(BEETLE_VERTS_NUM, tempWorldVerts);
}

//----------------------------------------------------------------------------------------------------
void Beetle::DebugRender() const
{
    Vec2 const playerShipPos = g_game->GetPlayerShip()->GetPosition();
    Vec2 const offset        = Vec2(-0.5f, 0.f);

    DebugDrawLine(playerShipPos,
                  m_position,
                  0.2f,
                  DEBUG_RENDER_GREY);
    DebugDrawLine(m_position,
                  m_position + GetForwardNormal() * BEETLE_COSMETIC_RADIUS,
                  0.2f,
                  DEBUG_RENDER_RED);
    DebugDrawLine(m_position,
                  m_position + GetForwardNormal().GetRotated90Degrees() * BEETLE_COSMETIC_RADIUS,
                  0.2f,
                  DEBUG_RENDER_GREEN);
    DebugDrawRing(m_position + offset,
                  BEETLE_COSMETIC_RADIUS,
                  0.2f,
                  DEBUG_RENDER_MAGENTA);
    DebugDrawRing(m_position,
                  BEETLE_PHYSICS_RADIUS,
                  0.2f,
                  DEBUG_RENDER_CYAN);
    DebugDrawLine(m_position,
                  m_position + m_velocity,
                  0.2f,
                  DEBUG_RENDER_YELLOW);
}

//----------------------------------------------------------------------------------------------------
void Beetle::InitializeLocalVerts()
{
    m_localVerts[0].m_position = Vec3(1.5f, 1.f, 0.f);
    m_localVerts[1].m_position = Vec3(-1.5f, 2.f, 0.f);
    m_localVerts[2].m_position = Vec3(1.5f, -1.f, 0.f);

    m_localVerts[3].m_position = Vec3(1.5f, -1.f, 0.f);
    m_localVerts[4].m_position = Vec3(-1.5f, 2.f, 0.f);
    m_localVerts[5].m_position = Vec3(-1.5f, -2.f, 0.f);

    for (Vertex_PCU& m_localVert : m_localVerts)
    {
        m_localVert.m_color = m_color;
    }
}

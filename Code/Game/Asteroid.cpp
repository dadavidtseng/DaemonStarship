//----------------------------------------------------------------------------------------------------
// Asteroid.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Asteroid.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Game.hpp"

//----------------------------------------------------------------------------------------------------
Asteroid::Asteroid( Vec2 const& position, float const orientationDegrees)
    : Entity( position, orientationDegrees, ASTEROID_COLOR)
{
    m_health          = 3;
    m_physicsRadius   = ASTEROID_PHYSICS_RADIUS;
    m_cosmeticRadius  = ASTEROID_COSMETIC_RADIUS;
    m_angularVelocity = g_rng->RollRandomFloatInRange(-200.f, 200.f);

    float const rangeX = g_rng->RollRandomFloatInRange(-ASTEROID_SPEED, ASTEROID_SPEED);
    float const rangeY = ASTEROID_SPEED - rangeX;

    m_velocity = Vec2(rangeX, rangeY);

    Asteroid::InitializeLocalVerts();
}

//----------------------------------------------------------------------------------------------------
void Asteroid::Update(float const deltaSeconds)
{
    if (m_isDead)
        return;

    m_position += m_velocity * deltaSeconds;
    m_orientationDegrees += m_angularVelocity * deltaSeconds;

    WrapPosition();
}

//----------------------------------------------------------------------------------------------------
void Asteroid::Render() const
{
    if (m_isDead)
        return;

    Vertex_PCU tempWorldVerts[ASTEROID_VERTS_NUM];

    for (int vertIndex = 0; vertIndex < ASTEROID_VERTS_NUM; vertIndex++)
    {
        tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
    }

    TransformVertexArrayXY3D(ASTEROID_VERTS_NUM, tempWorldVerts, 1.f, m_orientationDegrees, m_position);
    g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_renderer->BindTexture(nullptr);
    g_renderer->DrawVertexArray(ASTEROID_VERTS_NUM, tempWorldVerts);
}

//----------------------------------------------------------------------------------------------------
void Asteroid::DebugRender() const
{
    Vec2 const playerShipPos = g_game->GetPlayerShip()->GetPosition();

    DebugDrawLine(playerShipPos,
                  m_position,
                  0.2f,
                  DEBUG_RENDER_GREY);
    DebugDrawLine(m_position,
                  m_position + GetForwardNormal() * BULLET_COSMETIC_RADIUS,
                  0.2f,
                  DEBUG_RENDER_RED);
    DebugDrawLine(m_position,
                  m_position + GetForwardNormal().GetRotated90Degrees() * BULLET_COSMETIC_RADIUS,
                  0.2f,
                  DEBUG_RENDER_GREEN);
    DebugDrawRing(m_position,
                  ASTEROID_COSMETIC_RADIUS,
                  0.2f,
                  DEBUG_RENDER_MAGENTA);
    DebugDrawRing(m_position,
                  ASTEROID_PHYSICS_RADIUS,
                  0.2f,
                  DEBUG_RENDER_CYAN);
    DebugDrawLine(m_position,
                  m_position + m_velocity,
                  0.2f,
                  DEBUG_RENDER_YELLOW);
}

//----------------------------------------------------------------------------------------------------
void Asteroid::InitializeLocalVerts()
{
    float           radius[ASTEROID_TRIS_NUM]             = {};
    constexpr float degreesPerSide                        = 360.0f / static_cast<float>(ASTEROID_TRIS_NUM);
    Vec2            localVertPositions[ASTEROID_TRIS_NUM] = {};

    for (int sideIndex = 0; sideIndex < ASTEROID_TRIS_NUM; ++sideIndex)
    {
        // Pre-generate random radius for every triangle
        radius[sideIndex] = g_rng->RollRandomFloatInRange(m_physicsRadius, m_cosmeticRadius);

        // Apply radius to vert positions
        const float degrees = degreesPerSide * static_cast<float>(sideIndex);

        localVertPositions[sideIndex].x = radius[sideIndex] * CosDegrees(degrees);
        localVertPositions[sideIndex].y = radius[sideIndex] * SinDegrees(degrees);
    }

    // Build triangles
    for (int sideIndex = 0; sideIndex < ASTEROID_TRIS_NUM; ++sideIndex)
    {
        const int currentRadiusIndex = sideIndex;
        const int nextRadiusIndex    = (sideIndex + 1) % ASTEROID_TRIS_NUM;

        const int firstVertIndex  = sideIndex * 3 + 0;
        const int secondVertIndex = sideIndex * 3 + 1;
        const int thirdVertIndex  = sideIndex * 3 + 2;

        const Vec2 secondVert = localVertPositions[currentRadiusIndex];
        const Vec2 thirdVert  = localVertPositions[nextRadiusIndex];

        m_localVerts[firstVertIndex].m_position  = Vec3(0.f, 0.f, 0.f);
        m_localVerts[secondVertIndex].m_position = Vec3(secondVert.x, secondVert.y, 0.f);
        m_localVerts[thirdVertIndex].m_position  = Vec3(thirdVert.x, thirdVert.y, 0.f);
    }

    for (Vertex_PCU& m_localVert : m_localVerts)
    {
        m_localVert.m_color = m_color;
    }
}

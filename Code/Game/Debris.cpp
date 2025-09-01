//-----------------------------------------------------------------------------------------------
// Debris.cpp
//

//-----------------------------------------------------------------------------------------------
#include "Game/Debris.hpp"

#include "Engine/Renderer/VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Game.hpp"

//-----------------------------------------------------------------------------------------------
Debris::Debris( Vec2 const& position, Vec2 const& velocity, float const radius, Rgba8 const color)
    : Entity( position, g_theRNG->RollRandomFloatInRange(0.f, 360.f), color),
      m_lifetime(2.f),
      m_initialLifetime(2.f)
{
    m_physicsRadius  = radius * 0.5f;
    m_cosmeticRadius = radius * 1.5f;

    m_velocity        = velocity;
    m_angularVelocity = g_theRNG->RollRandomFloatInRange(-200.f, 200.f);

    m_color.a = 127;

    Debris::InitializeLocalVerts();
}

//-----------------------------------------------------------------------------------------------
void Debris::Update(float const deltaSeconds)
{
    if (m_isDead)
        return;

    m_position += m_velocity * deltaSeconds;
    m_orientationDegrees += m_angularVelocity * deltaSeconds;

    m_lifetime -= deltaSeconds;

    if (m_lifetime <= 0.f)
    {
        m_isDead    = true;
        m_isGarbage = true;
    }
}

//-----------------------------------------------------------------------------------------------
void Debris::Render() const
{
    if (m_isDead)
        return;

    Vertex_PCU tempWorldVerts[DEBRIS_VERTS_NUM];

    for (int vertIndex = 0; vertIndex < DEBRIS_VERTS_NUM; vertIndex++)
    {
        tempWorldVerts[vertIndex] = m_localVerts[vertIndex];

        tempWorldVerts[vertIndex].m_color.a = static_cast<unsigned char>(static_cast<float>(m_color.a) * (m_lifetime /
            m_initialLifetime));
    }

    TransformVertexArrayXY3D(DEBRIS_VERTS_NUM, tempWorldVerts, 1.f, m_orientationDegrees, m_position);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->DrawVertexArray(DEBRIS_VERTS_NUM, tempWorldVerts);
}

//-----------------------------------------------------------------------------------------------
void Debris::DebugRender() const
{
    if (m_isDead)
        return;

    Vec2 const playerShipPos = g_theGame->GetPlayerShip()->GetPosition();

    DebugDrawLine(playerShipPos,
                  m_position,
                  0.2f,
                  DEBUG_RENDER_GREY);
    DebugDrawLine(m_position,
                  m_position + GetForwardNormal() * m_cosmeticRadius,
                  0.2f,
                  DEBUG_RENDER_RED);
    DebugDrawLine(m_position,
                  m_position + GetForwardNormal().GetRotated90Degrees() * m_cosmeticRadius,
                  0.2f,
                  DEBUG_RENDER_GREEN);
    DebugDrawRing(m_position,
                  m_cosmeticRadius,
                  0.2f,
                  DEBUG_RENDER_MAGENTA);
    DebugDrawLine(m_position,
                  m_position + m_velocity,
                  0.2f,
                  DEBUG_RENDER_YELLOW);
}

//-----------------------------------------------------------------------------------------------
void Debris::InitializeLocalVerts()
{
    float* radius             = new float[DEBRIS_TRI_NUM];
    float  degreesPerSide     = 360.0f / static_cast<float>(DEBRIS_TRI_NUM);
    Vec2*  localVertPositions = new Vec2[DEBRIS_TRI_NUM];

    for (int sideIndex = 0; sideIndex < DEBRIS_TRI_NUM; ++sideIndex)
    {
        radius[sideIndex] = g_theRNG->RollRandomFloatInRange(m_physicsRadius * 0.5f, m_cosmeticRadius * 0.75f);

        const float degrees = degreesPerSide * static_cast<float>(sideIndex);

        localVertPositions[sideIndex].x = radius[sideIndex] * CosDegrees(degrees);
        localVertPositions[sideIndex].y = radius[sideIndex] * SinDegrees(degrees);
    }

    for (int sideIndex = 0; sideIndex < DEBRIS_TRI_NUM; ++sideIndex)
    {
        int const currentRadiusIndex = sideIndex;
        int const nextRadiusIndex    = (sideIndex + 1) % DEBRIS_TRI_NUM;

        int const firstVertIndex  = sideIndex * 3 + 0;
        int const secondVertIndex = sideIndex * 3 + 1;
        int const thirdVertIndex  = sideIndex * 3 + 2;

        Vec2 const secondVert = localVertPositions[currentRadiusIndex];
        Vec2 const thirdVert  = localVertPositions[nextRadiusIndex];

        m_localVerts[firstVertIndex].m_position  = Vec3(0.f, 0.f, 0.f);
        m_localVerts[secondVertIndex].m_position = Vec3(secondVert.x, secondVert.y, 0.f);
        m_localVerts[thirdVertIndex].m_position  = Vec3(thirdVert.x, thirdVert.y, 0.f);
    }

    for (Vertex_PCU& m_localVert : m_localVerts)
    {
        m_localVert.m_color = m_color;
    }
}

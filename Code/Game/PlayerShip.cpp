//----------------------------------------------------------------------------------------------------
// PlayerShip.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/PlayerShip.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Game.hpp"

//----------------------------------------------------------------------------------------------------
PlayerShip::PlayerShip(Vec2 const& position, float const orientationDegrees, int const health, bool const isReadyToSpawnBullet)
    : Entity(position, orientationDegrees, PLAYER_SHIP_COLOR)
{
    m_health               = health;
    m_physicsRadius        = PLAYER_SHIP_PHYSICS_RADIUS;
    m_cosmeticRadius       = PLAYER_SHIP_COSMETIC_RADIUS;
    m_orientationDegrees   = orientationDegrees;
    m_isReadyToSpawnBullet = isReadyToSpawnBullet;

    PlayerShip::InitializeLocalVerts();
}

//----------------------------------------------------------------------------------------------------
void PlayerShip::Update(float const deltaSeconds)
{
    UpdateFromKeyBoard();
    UpdateFromController();

    if (m_isDead) return;

    if (m_isThrusting)
    {
        Vec2 const fwdNormal    = Vec2::MakeFromPolarDegrees(m_orientationDegrees);
        Vec2 const acceleration = fwdNormal * PLAYER_SHIP_ACCELERATION * m_thrustRate;

        m_velocity += acceleration * deltaSeconds;
    }

    if (m_isTurningLeft) m_orientationDegrees += PLAYER_SHIP_TURN_SPEED * deltaSeconds;

    if (m_isTurningRight) m_orientationDegrees -= PLAYER_SHIP_TURN_SPEED * deltaSeconds;

    m_position += m_velocity * deltaSeconds;

    BounceOffWall();
}

//----------------------------------------------------------------------------------------------------
void PlayerShip::Render() const
{
    if (m_isDead) return;


    Vertex_PCU tempWorldVerts[PLAYER_SHIP_VERTS_NUM];

    for (int vertIndex = 0; vertIndex < PLAYER_SHIP_VERTS_NUM; vertIndex++)
    {
        tempWorldVerts[vertIndex]         = m_localVerts[vertIndex];
        tempWorldVerts[vertIndex].m_color = m_color;
    }

    TransformVertexArrayXY3D(PLAYER_SHIP_VERTS_NUM, tempWorldVerts, 1.f, m_orientationDegrees, m_position);

    // DebugDrawGlowCircle(m_position, 5.f, WASP_COLOR, 0.0001f);
    g_theRenderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_BACK);
    g_theRenderer->BindTexture(nullptr);
    g_theRenderer->DrawVertexArray(PLAYER_SHIP_VERTS_NUM, tempWorldVerts);
}

//----------------------------------------------------------------------------------------------------
void PlayerShip::DebugRender() const
{
    DebugDrawLine(m_position,
                  m_position + GetForwardNormal() * PLAYER_SHIP_COSMETIC_RADIUS,
                  0.2f,
                  DEBUG_RENDER_RED);
    DebugDrawLine(m_position,
                  m_position + GetForwardNormal().GetRotated90Degrees() * PLAYER_SHIP_COSMETIC_RADIUS,
                  0.2f,
                  DEBUG_RENDER_GREEN);
    DebugDrawRing(m_position,
                  PLAYER_SHIP_COSMETIC_RADIUS,
                  0.2f,
                  DEBUG_RENDER_MAGENTA);
    DebugDrawRing(m_position,
                  PLAYER_SHIP_PHYSICS_RADIUS,
                  0.2f,
                  DEBUG_RENDER_CYAN);
    DebugDrawLine(m_position,
                  m_position + m_velocity,
                  0.2f,
                  DEBUG_RENDER_YELLOW);
}

//----------------------------------------------------------------------------------------------------
bool PlayerShip::IsReadyToSpawnBullet(bool const isReady)
{
    return m_isReadyToSpawnBullet = isReady;
}

//----------------------------------------------------------------------------------------------------
void PlayerShip::UpdateFromController()
{
    XboxController const& controller = g_theInput->GetController(0); // #TODO: support multiple players

    // Shoot
    if (controller.WasButtonJustPressed(XBOX_BUTTON_A))
    {
        g_theGame->SpawnBullet(m_position + GetForwardNormal(), m_orientationDegrees);
    }

    if (g_theInput->IsKeyDown(KEYCODE_W) || controller.GetLeftStick().GetMagnitude() > 0.f)
    {
        if (g_theInput->IsKeyDown(KEYCODE_W))
        {
            m_isThrusting = true;
            m_thrustRate  = 1;
            return;
        }
        if (controller.GetLeftStick().GetMagnitude() > 0.f)
        {
            m_isThrusting        = true;
            m_thrustRate         = controller.GetLeftStick().GetMagnitude();
            m_orientationDegrees = controller.GetLeftStick().GetOrientationDegrees();
        }
    }
    else
    {
        m_isThrusting = false;
        m_thrustRate  = 0;
    }
}

Vec2& PlayerShip::GetPositionAndSet()
{
    return m_position;
}

Vec2& PlayerShip::GetVelocityAndSet()
{
    return m_velocity;
}

void PlayerShip::SetPosition(const Vec2& targetPosition)
{
    m_position = targetPosition;
}

int PlayerShip::GetPlayerScore() const
{
    return m_playerScore;
}

void PlayerShip::SetPlayerScore(int delta)
{
    m_playerScore += delta;
}


//-----------------------------------------------------------------------------------------------
void PlayerShip::InitializeLocalVerts()
{
    m_localVerts[0].m_position = Vec3(-2.f, 1.f, 0.f);
    m_localVerts[1].m_position = Vec3(2.f, 1.f, 0.f);
    m_localVerts[2].m_position = Vec3(0.f, 2.f, 0.f);

    m_localVerts[3].m_position = Vec3(0.f, 1.f, 0.f);
    m_localVerts[4].m_position = Vec3(-2.f, 1.f, 0.f);
    m_localVerts[5].m_position = Vec3(-2.f, -1.f, 0.f);

    m_localVerts[6].m_position = Vec3(0.f, 1.f, 0.f);
    m_localVerts[7].m_position = Vec3(-2.f, -1.f, 0.f);
    m_localVerts[8].m_position = Vec3(0.f, -1.f, 0.f);

    m_localVerts[9].m_position  = Vec3(1.f, 0.f, 0.f);
    m_localVerts[10].m_position = Vec3(0.f, 1.f, 0.f);
    m_localVerts[11].m_position = Vec3(0.f, -1.f, 0.f);

    m_localVerts[12].m_position = Vec3(2.f, -1.f, 0.f);
    m_localVerts[13].m_position = Vec3(-2.f, -1.f, 0.f);
    m_localVerts[14].m_position = Vec3(0.f, -2.f, 0.f);
}

//-----------------------------------------------------------------------------------------------
void PlayerShip::BounceOffWall()
{
    if (m_position.x < m_physicsRadius)
    {
        // We will always see the ship stick on bound no matter how fast is ship
        m_position.x = m_physicsRadius;
        m_velocity.x = -m_velocity.x;
    }

    if (m_position.x > WORLD_SIZE_X - m_physicsRadius) //e.g. 197 if world is 200 wide and radius is 3
    {
        m_position.x = WORLD_SIZE_X - m_physicsRadius;
        m_velocity.x = -m_velocity.x;
    }

    if (m_position.y < m_physicsRadius)
    {
        // We will always see the ship stick on bound no matter how fast is ship
        m_position.y = m_physicsRadius;
        m_velocity.y = -m_velocity.y;
    }

    if (m_position.y > WORLD_SIZE_Y - m_physicsRadius) //e.g. 197 if world is 200 wide and radius is 3
    {
        m_position.y = WORLD_SIZE_Y - m_physicsRadius;
        m_velocity.y = -m_velocity.y;
    }
}

//-----------------------------------------------------------------------------------------------
void PlayerShip::UpdateFromKeyBoard()
{
    m_isTurningLeft  = g_theInput->IsKeyDown(KEYCODE_A);
    m_isTurningRight = g_theInput->IsKeyDown(KEYCODE_D);

    if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE) &&
        m_isReadyToSpawnBullet &&
        !m_isDead)
        g_theGame->SpawnBullet(m_position + GetForwardNormal(), m_orientationDegrees);
}

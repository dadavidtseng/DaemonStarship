//-----------------------------------------------------------------------------------------------
// Bullet.cpp
//

//-----------------------------------------------------------------------------------------------
#include "Game/Bullet.hpp"
#include "Game/Game.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"

//-----------------------------------------------------------------------------------------------
Bullet::Bullet(Game* game, const Vec2& position, const float orientationDegrees)
	: Entity(game, position, orientationDegrees, Rgba8(255.f, 255.f, 0.f, 255.f))
{
	m_physicsRadius  = BULLET_PHYSICS_RADIUS;
	m_cosmeticRadius = BULLET_COSMETIC_RADIUS;
	m_velocity       = Vec2::MakeFromPolarDegrees(m_orientationDegrees, BULLET_SPEED);

	Bullet::InitializeLocalVerts();
}

//-----------------------------------------------------------------------------------------------
Bullet::~Bullet() = default;

//-----------------------------------------------------------------------------------------------
void Bullet::Update(const float deltaSeconds)
{
	if (m_isDead)
		return;

	m_position += m_velocity * deltaSeconds;
}

//-----------------------------------------------------------------------------------------------
void Bullet::Render() const
{
	if (m_isDead)
		return;

	Vertex_PCU tempWorldVerts[BULLET_VERTS_NUM];

	for (int vertIndex = 0; vertIndex < BULLET_VERTS_NUM; vertIndex++)
	{
		tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
	}

	TransformVertexArrayXY3D(BULLET_VERTS_NUM, tempWorldVerts, 1.f, m_orientationDegrees, m_position);

	g_theRenderer->DrawVertexArray(BULLET_VERTS_NUM, tempWorldVerts);
}

//-----------------------------------------------------------------------------------------------
void Bullet::DebugRender() const
{
	const Vec2 playerShipPos = m_game->GetPlayerShip()->GetPosition();

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
	              BULLET_COSMETIC_RADIUS,
	              0.2f,
	              DEBUG_RENDER_MAGENTA);
	DebugDrawRing(m_position,
	              BULLET_PHYSICS_RADIUS,
	              0.2f,
	              DEBUG_RENDER_CYAN);
	DebugDrawLine(m_position,
	              m_position + m_velocity,
	              0.2f,
	              DEBUG_RENDER_YELLOW);
}

//-----------------------------------------------------------------------------------------------
void Bullet::InitializeLocalVerts()
{
	m_localVerts[0].m_position = Vec3(0.f, 0.5f, 0);
	m_localVerts[1].m_position = Vec3(0.f, -0.5f, 0);
	m_localVerts[2].m_position = Vec3(0.5f, 0.f, 0);

	m_localVerts[3].m_position = Vec3(0.f, 0.5f, 0);
	m_localVerts[4].m_position = Vec3(-2.f, 0.f, 0);
	m_localVerts[5].m_position = Vec3(0.f, -0.5f, 0);

	m_localVerts[0].m_color = BULLET_YELLOW_OPAQUE;
	m_localVerts[1].m_color = BULLET_YELLOW_OPAQUE;
	m_localVerts[2].m_color = BULLET_YELLOW_OPAQUE;

	m_localVerts[3].m_color = BULLET_RED_OPAQUE;
	m_localVerts[4].m_color = BULLET_RED_TRANSPARENT;
	m_localVerts[5].m_color = BULLET_RED_OPAQUE;
}

#include "Game/Wasp.hpp"

#include "Game/Game.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"

Wasp::Wasp(Game* game, const Vec2& position, float orientationDegrees)
	: Entity(game, position, orientationDegrees, WASP_COLOR)
{
	m_health         = 3;
	m_physicsRadius  = WASP_PHYSICS_RADIUS;
	m_cosmeticRadius = WASP_COSMETIC_RADIUS;

	Wasp::InitializeLocalVerts();
}

Wasp::~Wasp() = default;

void Wasp::Update(float deltaSeconds)
{
	if (m_isDead)
		return;

	if (m_game->GetPlayerShip() &&
		!m_game->GetPlayerShip()->IsDead())
	{
		Vec2 playerShipPos     = m_game->GetPlayerShip()->GetPosition();
		Vec2 directionToPlayer = (playerShipPos - m_position).GetNormalized();

		m_orientationDegrees = directionToPlayer.GetOrientationDegrees();

		Vec2 fwdNormal    = Vec2::MakeFromPolarDegrees(m_orientationDegrees);
		Vec2 acceleration = fwdNormal * WASP_ACCELERATION;

		m_velocity += acceleration * deltaSeconds;
	}

	m_position += m_velocity * deltaSeconds;
}

void Wasp::Render() const
{
	if (m_isDead)
		return;

	Vertex_PCU tempWorldVerts[WASP_VERTS_NUM];

	for (int vertIndex = 0; vertIndex < WASP_VERTS_NUM; vertIndex++)
	{
		tempWorldVerts[vertIndex]         = m_localVerts[vertIndex];
		tempWorldVerts[vertIndex].m_color = m_color;
	}

	TransformVertexArrayXY3D(WASP_VERTS_NUM, tempWorldVerts, 1.f, m_orientationDegrees, m_position);

	g_theRenderer->DrawVertexArray(WASP_VERTS_NUM, tempWorldVerts);
}

void Wasp::DebugRender() const
{
	Vec2 playerShipPos = m_game->GetPlayerShip()->GetPosition();

	DebugDrawLine(playerShipPos,
	              m_position,
	              0.2f,
	              DEBUG_RENDER_GREY);
	DebugDrawLine(m_position,
	              m_position + GetForwardNormal() * WASP_COSMETIC_RADIUS,
	              0.2f,
	              DEBUG_RENDER_RED);
	DebugDrawLine(m_position,
	              m_position + GetForwardNormal().GetRotated90Degrees() * WASP_COSMETIC_RADIUS,
	              0.2f,
	              DEBUG_RENDER_GREEN);
	DebugDrawRing(m_position,
	              WASP_COSMETIC_RADIUS,
	              0.2f,
	              DEBUG_RENDER_MAGENTA);
	DebugDrawRing(m_position,
	              WASP_PHYSICS_RADIUS,
	              0.2f,
	              DEBUG_RENDER_CYAN);
	DebugDrawLine(m_position,
	              m_position + m_velocity,
	              0.2f,
	              DEBUG_RENDER_YELLOW);
}

void Wasp::InitializeLocalVerts()
{
	m_localVerts[0].m_position = Vec3(2.f, 0.f, 0);
	m_localVerts[1].m_position = Vec3(0.f, 2.f, 0);
	m_localVerts[2].m_position = Vec3(0.f, 1.f, 0);

	m_localVerts[3].m_position = Vec3(2.f, 0.f, 0);
	m_localVerts[4].m_position = Vec3(0.f, 1.f, 0);
	m_localVerts[5].m_position = Vec3(-2.f, 0.f, 0);

	m_localVerts[6].m_position = Vec3(2.f, 0.f, 0);
	m_localVerts[7].m_position = Vec3(-2.f, 0.f, 0);
	m_localVerts[8].m_position = Vec3(0.f, -1.f, 0);

	m_localVerts[9].m_position  = Vec3(2.f, 0.f, 0);
	m_localVerts[10].m_position = Vec3(0.f, -1.f, 0);
	m_localVerts[11].m_position = Vec3(0.f, -2.f, 0);
}

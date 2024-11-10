#include "Game/Entities/Entity.hpp"
#include "Game/GameCommon.hpp"

//-----------------------------------------------------------------------------------------------
Entity::Entity(Game* game, const Vec2& position, const float orientationDegrees, const Rgba8 color)
	: m_position(position),
	  m_velocity(Vec2(0.f, 0.f)),
	  m_orientationDegrees(orientationDegrees),
	  m_angularVelocity(0.f),
	  m_physicsRadius(5.f),
	  m_cosmeticRadius(10.0f),
	  m_health(1),
	  m_isDead(false),
	  m_isGarbage(false),
	  m_color(color),
	  m_game(game)
{
}

//-----------------------------------------------------------------------------------------------
Entity::~Entity() = default;

//-----------------------------------------------------------------------------------------------
void Entity::MarkAsDead()
{
	m_isDead = true;
}

//-----------------------------------------------------------------------------------------------
void Entity::MarkAsGarbage()
{
	m_isGarbage = true;
}

//-----------------------------------------------------------------------------------------------
bool Entity::IsDead() const
{
	return m_isDead;
}

//-----------------------------------------------------------------------------------------------
bool Entity::IsGarbage() const
{
	return m_isGarbage;
}

//-----------------------------------------------------------------------------------------------
bool Entity::IsOffScreen() const
{
	return
		m_position.x < -m_cosmeticRadius ||
		m_position.x > WORLD_SIZE_X + m_cosmeticRadius ||
		m_position.y < -m_cosmeticRadius ||
		m_position.y > WORLD_SIZE_Y + m_cosmeticRadius;
}

void Entity::WrapPosition()
{
	if (m_position.x > WORLD_SIZE_X + m_cosmeticRadius)
	{
		m_position.x = 0.f;
	}

	else if (m_position.x < -m_cosmeticRadius)
	{
		m_position.x = WORLD_SIZE_X;
	}

	if (m_position.y > WORLD_SIZE_Y + m_cosmeticRadius)
	{
		m_position.y = 0.f;
	}

	else if (m_position.y < -m_cosmeticRadius)
	{
		m_position.y = WORLD_SIZE_Y;
	}
}

//-----------------------------------------------------------------------------------------------
Vec2 Entity::GetForwardNormal() const
{
	return Vec2::MakeFromPolarDegrees(m_orientationDegrees, 1);
}

//-----------------------------------------------------------------------------------------------
Vec2 Entity::GetPosition() const
{
	return m_position;
}

Vec2 Entity::GetVelocity() const
{
	return m_velocity;
}

Rgba8 Entity::GetColor() const
{
	return m_color;
}

int Entity::GetHealth() const
{
	return m_health;
}

void Entity::SetHealth(const int delta)
{
	m_health -= delta;
}

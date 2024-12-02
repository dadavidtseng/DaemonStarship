//-----------------------------------------------------------------------------------------------
// Entity.hpp
//

//-----------------------------------------------------------------------------------------------
#pragma once
#include <Engine/Math/Vec2.hpp>
#include "Engine/Core/Vertex_PCU.hpp"

//-----------------------------------------------------------------------------------------------
class Game;

//-----------------------------------------------------------------------------------------------
class Entity
{
public:
	Entity(Game* game, const Vec2& position, float orientationDegrees, Rgba8 color);
	virtual ~Entity();

	virtual void Update(float deltaSeconds) = 0;
	virtual void Render() const = 0;
	virtual void DebugRender() const = 0;

	virtual void MarkAsDead();
	virtual void MarkAsGarbage();

	virtual bool IsDead() const;
	virtual bool IsGarbage() const;
	virtual bool IsOffScreen() const;
	virtual void InitializeLocalVerts() = 0;

	virtual void  WrapPosition();
	virtual Vec2  GetForwardNormal() const;
	virtual Vec2  GetPosition() const;
	virtual Vec2  GetVelocity() const;
	virtual Rgba8 GetColor() const;
	int           GetHealth() const;
	void          SetHealth(int delta);

protected:
	// universal data members used by most/all entities
	Vec2  m_position;           // the Entity’s 2D (x,y) Cartesian origin/center location, in world space
	Vec2  m_velocity;           // the Entity’s linear 2D (x,y) velocity, in world units per second
	float m_orientationDegrees; // its forward angle, in degrees (counter-clockwise from +x/east)
	float m_angularVelocity;    // the Entity’s signed angular velocity (spin rate), in degrees per second
	float m_physicsRadius;      // the Entity’s (inner, conservative) disc-radius for all physics purposes
	float m_cosmeticRadius;     // the Entity’s (outer, liberal) disc-radius that encloses all of its vertexes
	int   m_health;             // (int) how many “hits” the entity can sustain before dying
	bool  m_isDead;             // whether the Entity is “dead” in the game; affects entity and game logic
	bool  m_isGarbage;          // whether the Entity should be deleted at the end of Game::Update()
	Rgba8 m_color;
	Game* m_game = nullptr; // a pointer back to the Game instance
};

//----------------------------------------------------------------------------------------------------
// Entity.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include "Engine/Renderer/Vertex_PCU.hpp"

//----------------------------------------------------------------------------------------------------
class Game;

//----------------------------------------------------------------------------------------------------
class Entity
{
public:
    Entity(Vec2 const& position, float orientationDegrees, Rgba8 const& color);
    virtual ~Entity() = default;

    virtual void Update(float deltaSeconds) = 0;
    virtual void Render() const = 0;
    virtual void DebugRender() const = 0;

    virtual void MarkAsDead();
    virtual void MarkAsGarbage();

    [[nodiscard]] virtual bool IsDead() const;
    [[nodiscard]] virtual bool IsGarbage() const;
    [[nodiscard]] virtual bool IsOffScreen() const;
    virtual void InitializeLocalVerts() = 0;

    virtual void              WrapPosition();
    [[nodiscard]] virtual Vec2  GetForwardNormal() const;
    [[nodiscard]] virtual Vec2  GetPosition() const;
    [[nodiscard]] virtual Vec2  GetVelocity() const;
    [[nodiscard]] virtual Rgba8 GetColor() const;
    int           m_health = 1;             // (int) how many 'hits' the entity can sustain before dying
protected:
    // universal data members used by most/all entities
    Vec2  m_position;           // the Entity's 2D (x,y) Cartesian origin/center location, in world space
    Vec2  m_velocity = Vec2::ZERO;           // the Entity's linear 2D (x,y) velocity, in world units per second
    float m_orientationDegrees; // its forward angle, in degrees (counter-clockwise from +x/east)
    float m_angularVelocity = 0.f;    // the Entity's signed angular velocity (spin rate), in degrees per second
    float m_physicsRadius   = 5.f;      // the Entity's (inner, conservative) disc-radius for all physics purposes
    float m_cosmeticRadius  = 10.f;     // the Entity's (outer, liberal) disc-radius that encloses all of its vertexes

    bool  m_isDead    = false;             // whether the Entity is 'dead' in the game; affects entity and game logic
    bool  m_isGarbage = false;          // whether the Entity should be deleted at the end of Game::Update()
    Rgba8 m_color;
};

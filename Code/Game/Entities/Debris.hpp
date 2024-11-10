#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Entities/Entity.hpp"

class Debris : public Entity
{
public:
	Debris(Game* game, Vec2 const& position, Vec2 const& velocity, float radius, Rgba8 color);
	~Debris() override;

	void Update(float deltaSeconds) override;
	void Render() const override;
	void DebugRender() const override;

private:
	void InitializeLocalVerts() override;

	Vertex_PCU m_localVerts[DEBRIS_VERTS_NUM];
	float      m_lifetime;
	float      m_initialLifetime;
};

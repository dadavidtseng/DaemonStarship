#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Entities/Entity.hpp"

class Game;

//-----------------------------------------------------------------------------------------------
class Asteroid : public Entity
{
public:
	Asteroid(Game* game, const Vec2& position, float orientationDegrees);
	~Asteroid() override;

	void Update(float deltaSeconds) override;
	void Render() const override;
	void DebugRender() const override;

private:
	void InitializeLocalVerts() override;

	Vertex_PCU m_localVerts[ASTEROID_VERTS_NUM];
};
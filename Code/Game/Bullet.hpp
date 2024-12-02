#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Entity.hpp"

//-----------------------------------------------------------------------------------------------
class Bullet : public Entity
{
public:
	Bullet(Game* game, const Vec2& position, float orientationDegrees);
	~Bullet() override;

	void Update(float deltaSeconds) override;
	void Render() const override;
	void DebugRender() const override;

private:
	void InitializeLocalVerts() override;

	Vertex_PCU m_localVerts[BULLET_VERTS_NUM];
};
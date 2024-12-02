#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Entity.hpp"

class Beetle : public Entity
{
public:
	Beetle(Game* game, const Vec2& position, float orientationDegrees);
	~Beetle() override;

	void Update(float deltaSeconds) override;
	void Render() const override;
	void DebugRender() const override;
	

private:
	void InitializeLocalVerts() override;

	Vertex_PCU m_localVerts[BEETLE_VERTS_NUM];
};
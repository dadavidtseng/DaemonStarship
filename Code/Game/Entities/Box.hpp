//-----------------------------------------------------------------------------------------------
// Box.hpp
//

//-----------------------------------------------------------------------------------------------
#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Entities/Entity.hpp"
#include "Engine/Math/AABB2.hpp"

class Box : public Entity
{
public:
	Box(Game* game, const Vec2& position, float orientationDegrees);
	~Box() override;
	void SubscribeToEvent(Game* a);
	static void OnEventReceived(void* sender, void* args);

	void  Update(float deltaSeconds) override;
	void  Render() const override;
	void  DebugRender() const override;
	AABB2 GetBoxCollider();
	void SetPosition(const Vec2& targetPosition);

private:
	void InitializeLocalVerts() override;

	Vertex_PCU m_localVerts[BOX_VERTS_NUM];

	AABB2 m_boxCollider;
	float m_accumulatedTime;
	Vec2 m_targetPosition;
};

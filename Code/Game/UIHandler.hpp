//----------------------------------------------------------------------------------------------------
// UIHandler.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <string>
#include "Engine/Core/Vertex_PCU.hpp"
#include "Game/GameCommon.hpp"

//----------------------------------------------------------------------------------------------------
struct Button;
class Game;

//----------------------------------------------------------------------------------------------------
class UIHandler
{
public:
	explicit UIHandler(Game* game);

	void Update(float deltaSeconds);
	void ConfirmPlayerName() const;
	void HandleKeyboardInput();
	void UpdateButtonSelection();

	void DrawAttractModeUI() const;
	void DrawInGameUI(int currentPlayerShipHealth) const;

	void        DrawPlayerNameInput() const;
	bool        IsFirstButtonSelected() const;
	bool        IsSecondButtonSelected() const;
	std::string GetPlayerShipName();

private:
	void InitializePlayerShipLocalVerts();
	void InitializeAttractModeButtons() const;
	void DrawPlayerShip(Vec2 const& drawPosition, Rgba8 const& color, float scale) const;

	Vertex_PCU  m_localVerts[PLAYER_SHIP_VERTS_NUM];
	float       m_shiningTime;
	Button*     m_attractModeButtons[2] = {};
	int         m_selectedButtonIndex;
	std::string m_playerShipName;
	Game*       m_game;
};

struct Button
{
	float       width;
	float       height;
	Vec2        center;
	Rgba8       color;
	Vec2        textPosition;
	Rgba8       textColor;
	std::string text;
	bool        isSelected;
};

//----------------------------------------------------------------------------------------------------
// UIHandler.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include "Game/GameCommon.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Renderer/Vertex_PCU.hpp"

//----------------------------------------------------------------------------------------------------
struct Button;
class Game;

//----------------------------------------------------------------------------------------------------
class UIHandler
{
public:
    explicit UIHandler(Game* game);

    void Update(double deltaSeconds);
    void ConfirmPlayerName() const;
    void HandleKeyboardInput();
    void UpdateButtonSelection();

    void DrawAttractModeUI() const;
    void DrawInGameUI(int currentPlayerShipHealth) const;

    void   DrawPlayerNameInput() const;
    bool   IsFirstButtonSelected() const;
    bool   IsSecondButtonSelected() const;
    String GetPlayerShipName();

private:
    void InitializePlayerShipLocalVerts();
    void InitializeAttractModeButtons() const;
    void DrawPlayerShip(Vec2 const& drawPosition, Rgba8 const& color, float scale) const;

    Vertex_PCU m_localVerts[PLAYER_SHIP_VERTS_NUM];
    float      m_shiningTime;
    Button*    m_attractModeButtons[2] = {};
    int        m_selectedButtonIndex;
    String     m_playerShipName;
    Game*      m_game;
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

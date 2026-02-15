//----------------------------------------------------------------------------------------------------
// UIHandler.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/UIHandler.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Game.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/SimpleTriangleFont.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/VertexUtils.hpp"

//----------------------------------------------------------------------------------------------------
UIHandler::UIHandler(Game* game)
    : m_shiningTime(0.f),
      m_selectedButtonIndex(0),
      m_game(game)
{
    m_attractModeButtons[0] = new Button();
    m_attractModeButtons[1] = new Button();

    InitializePlayerShipLocalVerts();
    InitializeAttractModeButtons();
}

//----------------------------------------------------------------------------------------------------
void UIHandler::Update(double const deltaSeconds)
{
    m_shiningTime += (float)deltaSeconds;

    UpdateButtonSelection();

    if (m_game->IsPlayerNameInputMode())
    {
        HandleKeyboardInput();
        m_attractModeButtons[0]->textPosition = Vec2(SCREEN_SIZE_X / 2.f - 335.f,
                                                     SCREEN_SIZE_Y / 2.f - 130.f);
        m_attractModeButtons[0]->text = "ENTER NAME!";
    }
}

void UIHandler::ConfirmPlayerName() const
{
    printf("Player ship name confirmed: %s\n", m_playerShipName.c_str());
}

void UIHandler::HandleKeyboardInput()
{
    for (char c = 'A'; c <= 'Z'; ++c)
    {
        if (g_input->WasKeyJustReleased(c))
        {
            m_playerShipName += c;
        }
    }

    for (char n = '0'; n <= '9'; ++n)
    {
        if (g_input->WasKeyJustReleased(n))
        {
            m_playerShipName += n;
        }
    }

    if (g_input->WasKeyJustReleased(KEYCODE_BACKSPACE) && !m_playerShipName.empty())
    {
        m_playerShipName.pop_back();
    }


    if (g_input->WasKeyJustReleased(KEYCODE_ENTER))
    {
        ConfirmPlayerName();
    }
}

void UIHandler::UpdateButtonSelection()
{
    if (g_input->WasKeyJustPressed(KEYCODE_UPARROW))
    {
        m_selectedButtonIndex--;
        if (m_selectedButtonIndex < 0)
        {
            m_selectedButtonIndex = 1;
        }
    }
    else if (g_input->WasKeyJustPressed(KEYCODE_DOWNARROW))
    {
        m_selectedButtonIndex++;
        if (m_selectedButtonIndex > 1)
        {
            m_selectedButtonIndex = 0;
        }
    }

    XboxController const& controller = g_input->GetController(0);

    if (controller.WasButtonJustPressed(XBOX_BUTTON_DPAD_UP))
    {
        m_selectedButtonIndex--;
        if (m_selectedButtonIndex < 0)
        {
            m_selectedButtonIndex = 1;
        }
    }
    else if (controller.WasButtonJustPressed(XBOX_BUTTON_DPAD_DOWN))
    {
        m_selectedButtonIndex++;
        if (m_selectedButtonIndex > 1)
        {
            m_selectedButtonIndex = 0;
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        m_attractModeButtons[i]->isSelected = (i == m_selectedButtonIndex);
    }
}

//-----------------------------------------------------------------------------------------------
void UIHandler::DrawAttractModeUI() const
{
    float spacingX = 160.f;
    float spacingY = 160.f;

    int columns = static_cast<int>(1600.f / spacingX) + 1;
    int rows    = static_cast<int>(800.f / spacingY);

    for (int row = 0; row < rows; row++)
    {
        float offsetShift = 80.f;
        float xOffset     = row % 2 == 0 ? 0.f : offsetShift;

        float timeMod        = fmod(m_shiningTime * 3.f, static_cast<float>(columns));
        float highlightIndex = row % 2 == 0 ? timeMod : static_cast<float>(columns - 1) - timeMod;

        for (int col = 0; col < columns; col++)
        {
            float initialOffsetX = 0.f;
            float initialOffsetY = 80.f;
            float deltaX         = initialOffsetX + static_cast<float>(col) * spacingX + xOffset;
            float deltaY         = initialOffsetY + static_cast<float>(row) * spacingY;

            float distanceToHighlight = fabsf(static_cast<float>(col) - highlightIndex);
            float gradientEffect      = GetClamped(1.f - distanceToHighlight / 2.f, 0.f, 1.f);

            Rgba8 drawColor = Rgba8(102, 153, 204, static_cast<unsigned char>(255 * gradientEffect));

            DrawPlayerShip(Vec2(deltaX, deltaY), drawColor, 40.f);
        }

        std::vector<Vertex_PCU> titleShadowVerts;
        AddVertsForTextTriangles2D(titleShadowVerts,
                                   "STARSHIP",
                                   Vec2(155.f, 395.f),
                                   128.f,
                                   Rgba8(255, 255, 255, 100),
                                   1.f,
                                   true,
                                   0.3f);

        AddVertsForTextTriangles2D(titleShadowVerts,
                                   "'S",
                                   Vec2(650.f, 545.f),
                                   128.f,
                                   Rgba8(255, 255, 255, 100),
                                   1.f,
                                   true,
                                   0.3f);

        g_renderer->DrawVertexArray(static_cast<int>(titleShadowVerts.size()), titleShadowVerts.data());

        std::vector<Vertex_PCU> titleVerts;
        AddVertsForTextTriangles2D(titleVerts,
                                   "STARSHIP",
                                   Vec2(150.f, 400.f),
                                   128.f,
                                   WASP_COLOR,
                                   1.f,
                                   true,
                                   0.3f);
        AddVertsForTextTriangles2D(titleVerts,
                                   "'S",
                                   Vec2(645.f, 550.f),
                                   128.f,
                                   WASP_COLOR,
                                   1.f,
                                   true,
                                   0.3f);
        g_renderer->SetModelConstants();
        g_renderer->SetBlendMode(eBlendMode::ALPHA);
        g_renderer->SetRasterizerMode(eRasterizerMode::SOLID_CULL_NONE);
        g_renderer->SetSamplerMode(eSamplerMode::POINT_CLAMP);
        g_renderer->SetDepthMode(eDepthMode::DISABLED);
        g_renderer->BindTexture(nullptr);
        g_renderer->DrawVertexArray(static_cast<int>(titleVerts.size()), titleVerts.data());

        for (int buttonIndex = 0; buttonIndex < 2; ++buttonIndex)
        {
            // if (!m_attractModeButtons[buttonIndex])
            // 	continue;
            if (m_attractModeButtons[buttonIndex]->isSelected)
            {
                m_attractModeButtons[buttonIndex]->color = Rgba8(255, 255, 255);
            }
            else
            {
                m_attractModeButtons[buttonIndex]->color = Rgba8(100, 100, 100);
            }

            DebugDrawGlowBox(m_attractModeButtons[buttonIndex]->center,
                             Vec2(m_attractModeButtons[buttonIndex]->width, m_attractModeButtons[buttonIndex]->height),
                             m_attractModeButtons[buttonIndex]->color,
                             1.f);

            std::vector<Vertex_PCU> buttonTextVerts;
            AddVertsForTextTriangles2D(buttonTextVerts,
                                       m_attractModeButtons[buttonIndex]->text,
                                       m_attractModeButtons[buttonIndex]->textPosition,
                                       50.f,
                                       m_attractModeButtons[buttonIndex]->textColor,
                                       1.f,
                                       true,
                                       0.3f);

            g_renderer->DrawVertexArray(static_cast<int>(buttonTextVerts.size()), buttonTextVerts.data());
        }
    }
}

//-----------------------------------------------------------------------------------------------
void UIHandler::DrawInGameUI(int currentPlayerShipHealth) const
{
    DebugDrawGlowBox(Vec2(SCREEN_SIZE_X / 2.f, SCREEN_SIZE_Y - 30.f), Vec2(SCREEN_SIZE_X, 60.f), Rgba8(0, 0, 0), 1.f);
    DebugDrawGlowBox(Vec2(SCREEN_SIZE_X / 2.f, 30.f), Vec2(SCREEN_SIZE_X, 60.f), Rgba8(0, 0, 0), 1.f);

    for (int playerShipHealth = 0; playerShipHealth < currentPlayerShipHealth; playerShipHealth++)
    {
        float spacing        = 5.f * 8.f;
        float initialOffsetX = 4.f * 8.f;
        float delta          = initialOffsetX + static_cast<float>(playerShipHealth) * spacing;

        DrawPlayerShip(Vec2(delta, 96.f * 8.f), PLAYER_SHIP_COLOR, 8.f);
    }

    std::vector<Vertex_PCU> titleVerts;
    AddVertsForTextTriangles2D(titleVerts,
                               m_playerShipName + "/SCORE:" + std::to_string(m_game->GetPlayerShip()->m_score) +
                               "/HI:" + std::to_string(m_game->GetHighScore()),
                               Vec2(50.f, 0.f),
                               50.f,
                               Rgba8(255, 255, 255),
                               1.f,
                               true,
                               0.3f);

    g_renderer->DrawVertexArray(static_cast<int>(titleVerts.size()), titleVerts.data());
}

//-----------------------------------------------------------------------------------------------
void UIHandler::DrawPlayerShip(Vec2 const& drawPosition, Rgba8 const& color, float scale) const
{
    Vertex_PCU tempWorldVerts[PLAYER_SHIP_VERTS_NUM];

    for (int vertIndex = 0; vertIndex < PLAYER_SHIP_VERTS_NUM; vertIndex++)
    {
        tempWorldVerts[vertIndex] = m_localVerts[vertIndex];

        tempWorldVerts[vertIndex].m_color = color;
    }

    TransformVertexArrayXY3D(PLAYER_SHIP_VERTS_NUM, tempWorldVerts, scale, 90.f, drawPosition);

    g_renderer->DrawVertexArray(PLAYER_SHIP_VERTS_NUM, tempWorldVerts);
}

//-----------------------------------------------------------------------------------------------
void UIHandler::InitializePlayerShipLocalVerts()
{
    m_localVerts[0].m_position = Vec3(-2.f, 1.f, 0.f);
    m_localVerts[1].m_position = Vec3(2.f, 1.f, 0.f);
    m_localVerts[2].m_position = Vec3(0.f, 2.f, 0.f);

    m_localVerts[3].m_position = Vec3(0.f, 1.f, 0.f);
    m_localVerts[4].m_position = Vec3(-2.f, 1.f, 0.f);
    m_localVerts[5].m_position = Vec3(-2.f, -1.f, 0.f);

    m_localVerts[6].m_position = Vec3(0.f, 1.f, 0.f);
    m_localVerts[7].m_position = Vec3(-2.f, -1.f, 0.f);
    m_localVerts[8].m_position = Vec3(0.f, -1.f, 0.f);

    m_localVerts[9].m_position  = Vec3(1.f, 0.f, 0.f);
    m_localVerts[10].m_position = Vec3(0.f, 1.f, 0.f);
    m_localVerts[11].m_position = Vec3(0.f, -1.f, 0.f);

    m_localVerts[12].m_position = Vec3(2.f, -1.f, 0.f);
    m_localVerts[13].m_position = Vec3(-2.f, -1.f, 0.f);
    m_localVerts[14].m_position = Vec3(0.f, -2.f, 0.f);

    for (Vertex_PCU& m_localVert : m_localVerts)
    {
        m_localVert.m_color = PLAYER_SHIP_COLOR;
    }
}

void UIHandler::InitializeAttractModeButtons() const
{
    for (int buttonIndex = 0; buttonIndex < 2; ++buttonIndex)
    {
        m_attractModeButtons[buttonIndex]->width  = 800.f;
        m_attractModeButtons[buttonIndex]->height = 80.f;

        m_attractModeButtons[buttonIndex]->color = Rgba8(200, 200, 200);

        m_attractModeButtons[buttonIndex]->textColor  = Rgba8(0, 0, 0);
        m_attractModeButtons[buttonIndex]->isSelected = false;
    }

    m_attractModeButtons[0]->center = Vec2(SCREEN_SIZE_X / 2.f,
                                           SCREEN_SIZE_Y / 2.f - 100.f);

    m_attractModeButtons[1]->center = Vec2(SCREEN_SIZE_X / 2.f,
                                           SCREEN_SIZE_Y / 2.f - 300.f);

    m_attractModeButtons[0]->textPosition = Vec2(SCREEN_SIZE_X / 2.f - 125.f,
                                                 SCREEN_SIZE_Y / 2.f - 130.f);

    m_attractModeButtons[1]->textPosition = Vec2(SCREEN_SIZE_X / 2.f - 315.f,
                                                 SCREEN_SIZE_Y / 2.f - 330.f);

    m_attractModeButtons[0]->text = "START";
    m_attractModeButtons[1]->text = "SCOREBOARD";
}


void UIHandler::DrawPlayerNameInput() const
{
    std::vector<Vertex_PCU> textVerts;
    AddVertsForTextTriangles2D(textVerts, m_playerShipName, Vec2(150.f, 600.f), 100.f, WASP_COLOR, 1.f,
                               true, 0.3f);
    g_renderer->DrawVertexArray(static_cast<int>(textVerts.size()), textVerts.data());
}

bool UIHandler::IsFirstButtonSelected() const
{
    return m_attractModeButtons[0]->isSelected;
}

bool UIHandler::IsSecondButtonSelected() const
{
    return m_attractModeButtons[1]->isSelected;
}

std::string UIHandler::GetPlayerShipName()
{
    return m_playerShipName;
}

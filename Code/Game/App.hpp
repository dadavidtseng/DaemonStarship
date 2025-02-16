//----------------------------------------------------------------------------------------------------
// App.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Core/EventSystem.hpp"

class Camera;
//----------------------------------------------------------------------------------------------------
class Game;

//----------------------------------------------------------------------------------------------------
class App
{
public:
    void Startup();
    void Shutdown();
    void RunFrame();

    void RunMainLoop();

    static bool OnWindowClose(EventArgs& arg);
    static bool Event_KeyPressed(EventArgs& args);
    static void RequestQuit();
    static bool m_isQuitting;

private:
    void BeginFrame() const;
    void Update(float deltaSeconds);
    void Render() const;
    void EndFrame() const;

    void HandleKeyPressed();
    void HandleKeyReleased();
    void HandleQuitRequested();
    void AdjustForPauseAndTimeDistortion(float& deltaSeconds) const;
    void DeleteAndCreateNewGame();

    bool    m_isPaused           = true;
    bool    m_isSlowMo           = false;
    float   m_timeLastFrameStart = 0.f;
    Game*   m_theGame            = nullptr;
    Camera* m_devConsoleCamera   = nullptr;
};

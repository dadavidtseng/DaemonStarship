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
    App();
    ~App();
    void Startup();
    void Shutdown();
    void RunFrame();

    void RunMainLoop();

    static bool OnCloseButtonClicked(EventArgs& arg);
    static void RequestQuit();
    static bool m_isQuitting;

private:
    void BeginFrame() const;
    void Update();
    void Render() const;
    void EndFrame() const;

    void HandleKeyPressed();
    void HandleKeyReleased();
    void HandleQuitRequested();
    void AdjustForPauseAndTimeDistortion() const;
    void DeleteAndCreateNewGame();

    bool    m_isSlowMo           = false;
    float   m_timeLastFrameStart = 0.f;
    // Camera* m_devConsoleCamera   = nullptr;
};

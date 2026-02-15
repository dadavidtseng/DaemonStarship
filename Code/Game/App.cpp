//----------------------------------------------------------------------------------------------------
// App.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/App.hpp"
//----------------------------------------------------------------------------------------------------
#include "Game/Game.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Renderer/Renderer.hpp"

//----------------------------------------------------------------------------------------------------
App*  g_app  = nullptr;   // Created and owned by Main_Windows.cpp
Game* g_game = nullptr;   // Created and owned by the App

//----------------------------------------------------------------------------------------------------
STATIC bool App::m_isQuitting = false;

//----------------------------------------------------------------------------------------------------
App::App()
{
    GEngine::Get().Construct();
}

//----------------------------------------------------------------------------------------------------
App::~App()
{
    GEngine::Get().Destruct();
}

//----------------------------------------------------------------------------------------------------
/// @brief
/// Create all engine subsystems in a specific order.
void App::Startup()
{
    GEngine::Get().Startup();

    g_eventSystem->SubscribeEventCallbackFunction("OnCloseButtonClicked", OnCloseButtonClicked);
    g_eventSystem->SubscribeEventCallbackFunction("quit", OnCloseButtonClicked);

    g_game = new Game();
}

//----------------------------------------------------------------------------------------------------
// All Destroy and ShutDown process should be reverse order of the StartUp
//
void App::Shutdown()
{
    GAME_SAFE_RELEASE(g_game);

    g_eventSystem->UnsubscribeEventCallbackFunction("quit", OnCloseButtonClicked);
    g_eventSystem->UnsubscribeEventCallbackFunction("OnCloseButtonClicked", OnCloseButtonClicked);

    GEngine::Get().Shutdown();
}

//----------------------------------------------------------------------------------------------------
// One "frame" of the game.  Generally: Input, Update, Render.  We call this 60+ times per second.
//
void App::RunFrame()
{
    BeginFrame();       // Engine pre-frame stuff
    Update();           // Game updates / moves / spawns / hurts / kills stuff
    Render();           // Game draws current state of things
    EndFrame();         // Engine post-frame stuff
}

//----------------------------------------------------------------------------------------------------
void App::RunMainLoop()
{
    // Program main loop; keep running frames until it's time to quit
    while (!m_isQuitting)
    {
        // Sleep(16); // Temporary code to "slow down" our app to ~60Hz until we have proper frame timing in
        RunFrame();
    }
}

//----------------------------------------------------------------------------------------------------
bool App::OnCloseButtonClicked(EventArgs& arg)
{
    UNUSED(arg)

    RequestQuit();

    return true;
}

//----------------------------------------------------------------------------------------------------
void App::RequestQuit()
{
    m_isQuitting = true;
}

//----------------------------------------------------------------------------------------------------
void App::BeginFrame() const
{
    g_eventSystem->BeginFrame();
    g_window->BeginFrame();
    g_renderer->BeginFrame();
    g_devConsole->BeginFrame();
    g_input->BeginFrame();
    g_audio->BeginFrame();
}

//----------------------------------------------------------------------------------------------------
void App::Update()
{
    Clock::TickSystemClock();

    HandleKeyPressed();
    HandleKeyReleased();
    AdjustForPauseAndTimeDistortion();
    g_game->Update();
}

//----------------------------------------------------------------------------------------------------
// Some simple OpenGL example drawing code.
// This is the graphical equivalent of printing "Hello, world."
//
// Ultimately this function (App::Render) will only call methods on Renderer (like Renderer::DrawVertexArray)
//	to draw things, never calling OpenGL (nor DirectX) functions directly.
//
void App::Render() const
{
    Rgba8 const clearColor = Rgba8::BLACK;

    g_renderer->ClearScreen(clearColor);
    g_game->Render();

    AABB2 const box            = AABB2(Vec2::ZERO, Vec2(1600.f, 30.f));

    g_devConsole->Render(box);
}

//----------------------------------------------------------------------------------------------------
void App::EndFrame() const
{
    g_eventSystem->EndFrame();
    g_window->EndFrame();
    g_renderer->EndFrame();
    g_devConsole->EndFrame();
    g_input->EndFrame();
    g_audio->EndFrame();
}

//----------------------------------------------------------------------------------------------------
void App::HandleKeyPressed()
{
    if (g_devConsole->IsOpen() == true)
    {
        return;
    }

    XboxController const& controller = g_input->GetController(0);

    if (g_input->WasKeyJustPressed(KEYCODE_ESC))
    {
        switch (g_game->IsAttractMode())
        {
        case true:
            RequestQuit();

            break;

        case false:
            g_game->ResetData();
            g_app->DeleteAndCreateNewGame();
            g_game->SetAttractMode(true);
            g_game->SetPlayerShipIsReadyToSpawnBullet(false);

            break;
        }
    }

    if (g_input->WasKeyJustPressed(KEYCODE_O))
    {
        Clock::GetSystemClock().StepSingleFrame();
    }

    if (g_input->WasKeyJustPressed('T')) m_isSlowMo = true;

    if (g_input->WasKeyJustPressed('P')) Clock::GetSystemClock().TogglePause();

    if (g_input->WasKeyJustPressed(KEYCODE_F4) || controller.WasButtonJustPressed(XBOX_BUTTON_DPAD_DOWN))
    {
        if (g_game)
        {
            g_game->MarkAllEntityAsDeadAndGarbage();
        }
    }

    if (!g_game->IsAttractMode())
    {
        if (g_input->WasKeyJustPressed(KEYCODE_F8))
        {
            DeleteAndCreateNewGame();
            g_game->SetAttractMode(true);
            g_game->SetPlayerShipIsReadyToSpawnBullet(!false);
        }
    }
}

//----------------------------------------------------------------------------------------------------
void App::HandleKeyReleased()
{
    XboxController const& controller = g_input->GetController(0);

    if (g_input->WasKeyJustReleased('T') || controller.WasButtonJustReleased(XBOX_BUTTON_DPAD_UP)) m_isSlowMo = false;
}

//----------------------------------------------------------------------------------------------------
void App::HandleQuitRequested()
{
    m_isQuitting = true;
}

//----------------------------------------------------------------------------------------------------
void App::AdjustForPauseAndTimeDistortion() const
{
    if (!g_game->IsAttractMode())
    {
        if (m_isSlowMo == true)
        {
            Clock::GetSystemClock().SetTimeScale(0.1f);
        }

        if (
            m_isSlowMo == false)
        {
            Clock::GetSystemClock().SetTimeScale(1.f);
        }
    }
}

//----------------------------------------------------------------------------------------------------
void App::DeleteAndCreateNewGame()
{
    delete g_game;
    g_game = nullptr;

    g_game = new Game();
}

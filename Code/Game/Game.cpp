//----------------------------------------------------------------------------------------------------
// Game.cpp
//----------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------
#include "Game/Game.hpp"

#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/SimpleTriangleFont.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/LevelData.hpp"
#include "Game/ScoreBoardHandler.hpp"
#include "Game/UIHandler.hpp"

#if defined ERROR
#undef ERROR
#endif

//----------------------------------------------------------------------------------------------------
Game::Game()
{
    g_theEventSystem->SubscribeEventCallbackFunction("setscale", Command_SetTimeScale);
    g_theEventSystem->FireEvent("help");

    m_worldCamera          = new Camera();
    m_screenCamera         = new Camera();
    m_theUIHandler         = new UIHandler(this);
    m_theScoreBoardHandler = new ScoreBoardHandler();
    m_gameClock            = new Clock(Clock::GetSystemClock());

    SpawnPlayerShip();
    SpawnBoxCluster();
    SpawnEnemiesForCurrentWave();

    Vec2 const bottomLeft     = Vec2::ZERO;
    Vec2 const worldTopRight  = Vec2(WORLD_SIZE_X, WORLD_SIZE_Y);
    Vec2 const screenTopRight = Vec2(SCREEN_SIZE_X, SCREEN_SIZE_Y);

    m_worldCamera->SetOrthoGraphicView(bottomLeft, worldTopRight);
    m_screenCamera->SetOrthoGraphicView(bottomLeft, screenTopRight);


    SoundID const InGameBgm = g_theAudio->CreateOrGetSound(IN_GAME_BGM);
    g_theAudio->StartSound(InGameBgm, true, 1.f, 0.f, 1.f, false);
}

//----------------------------------------------------------------------------------------------------
Game::~Game()
{
    delete m_theUIHandler;
    m_theUIHandler = nullptr;

    delete m_theScoreBoardHandler;
    m_theScoreBoardHandler = nullptr;

    delete m_playerShip;
    m_playerShip = nullptr;

    for (int beetleIndex = 0; beetleIndex < MAX_BEETLE_NUM; ++beetleIndex)
    {
        if (!m_beetle[beetleIndex])
            continue;

        delete m_beetle[beetleIndex];
        m_beetle[beetleIndex] = nullptr;
    }

    for (int waspIndex = 0; waspIndex < MAX_WASP_NUM; ++waspIndex)
    {
        if (!m_wasp[waspIndex])
            continue;

        delete m_wasp[waspIndex];
        m_wasp[waspIndex] = nullptr;
    }

    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_NUM; ++asteroidIndex)
    {
        if (!m_asteroids[asteroidIndex])
            continue;

        delete m_asteroids[asteroidIndex];
        m_asteroids[asteroidIndex] = nullptr;
    }

    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS_NUM; ++bulletIndex)
    {
        if (!m_bullets[bulletIndex])
            continue;

        delete m_bullets[bulletIndex];
        m_bullets[bulletIndex] = nullptr;
    }

    for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS_NUM; ++debrisIndex)
    {
        if (!m_debris[debrisIndex])
            continue;

        delete m_debris[debrisIndex];
        m_debris[debrisIndex] = nullptr;
    }

    for (int boxIndex = 0; boxIndex < MAX_BOX_NUM; ++boxIndex)
    {
        if (!m_boxes[boxIndex])
            continue;

        delete m_boxes[boxIndex];
        m_boxes[boxIndex] = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------
void Game::Update()
{
    if (g_theDevConsole->IsOpen() == true)
    {
        return;
    }

    double const deltaSeconds = m_gameClock->GetDeltaSeconds();

    if (m_isAttractMode)
    {
        m_theUIHandler->Update(deltaSeconds);
        if (m_theUIHandler->IsFirstButtonSelected() && g_theInput->WasKeyJustReleased(KEYCODE_SPACE))
        {
            SetPlayerNameInputMode(true);
        }
    }

    // #TODO: Select keyboard or controller
    UpdateFromKeyBoard();

    UpdateFromController();

    if (AreAllEnemiesDead())
    {
        m_currentWave++;

        if (m_currentWave >= static_cast<int>(sizeof(LEVEL_DATA) / sizeof(LevelData)))
        {
            m_timeSinceDeath += (float)deltaSeconds;

            // #TODO: should finish debris animation before going back to attract mode
            if (m_timeSinceDeath >= 3.f)
            {
                ResetData();
            }
            return;
        }
        SpawnEnemiesForCurrentWave();
    }

    if (m_playerShip->GetHealth() == 0)
    {
        m_timeSinceDeath += (float)deltaSeconds;

        if (m_timeSinceDeath >= 3.f)
        {
            ResetData();
        }
    }

    UpdateEntities((float)deltaSeconds);
    DeleteGarbageEntities();

    // #TODO: add UpdateCamera(deltaSeconds);
    if (g_theInput->IsKeyDown('Y'))
    {
        DoShakeCamera((float)deltaSeconds);
    }
    else if (g_theInput->WasKeyJustReleased('Y'))
    {
        ResetCamera();
    }
}

//----------------------------------------------------------------------------------------------------
void Game::Render()
{
    g_theRenderer->BeginCamera(*m_worldCamera);

    if (!m_isAttractMode)
    {
        RenderEntities();
        DebugRenderEntities();
    }

    g_theRenderer->EndCamera(*m_worldCamera);

    g_theRenderer->BeginCamera(*m_screenCamera);

    if (!m_isAttractMode)
    {
        m_theUIHandler->DrawInGameUI(m_playerShip->GetHealth() - 1);
    }
    else
    {
        m_theUIHandler->DrawAttractModeUI();

        if (m_isPlayerNameInputMode)
            m_theUIHandler->DrawPlayerNameInput();
    }

    g_theRenderer->EndCamera(*m_screenCamera);

    if (g_theInput->WasKeyJustPressed('U'))
    {
        PlayerScore  scoreboard[MAX_PLAYERS];
        int          currentSize = 0;
        String const filename    = "Data/Score/Scoreboard.txt";

        m_theScoreBoardHandler->LoadScoreboardFromFile(scoreboard, currentSize, filename);

        m_theScoreBoardHandler->DisplayScoreboard(scoreboard, currentSize);


        m_theScoreBoardHandler->AddScore(scoreboard, currentSize, m_theUIHandler->GetPlayerShipName(),
                                         m_playerShip->GetPlayerScore());

        m_theScoreBoardHandler->SortScoreboard(scoreboard, currentSize);
        printf("Current size: %d\n", currentSize);
        m_theScoreBoardHandler->DisplayScoreboard(scoreboard, currentSize);
        m_highScore = m_theScoreBoardHandler->GetHighScore(scoreboard, currentSize);
        m_theScoreBoardHandler->SaveScoreboardToFile(scoreboard, currentSize, filename);
    }
}

void Game::DebugRender() const
{
}

void Game::ResetData()
{
    m_playerShipHealth = MAX_PLAYER_SHIP_HEALTH;
    m_currentWave      = -1;
    m_timeSinceDeath   = 0.f;
    m_isAttractMode    = true;
    // #TODO: FIX
    SetPlayerShipIsReadyToSpawnBullet(true);
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnBullet(Vec2 const& position, const float orientationDegrees)
{
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS_NUM; bulletIndex++)
    {
        if (m_bullets[bulletIndex])
            continue;

        m_bullets[bulletIndex] = new Bullet(this, position, orientationDegrees);

        return;
    }

    ERROR_RECOVERABLE("Cannot spawn a new asteroid; all slots are full")
}

//----------------------------------------------------------------------------------------------------
PlayerShip* Game::GetPlayerShip() const
{
    return m_playerShip;
}

//----------------------------------------------------------------------------------------------------
void Game::MarkAllEntityAsDeadAndGarbage()
{
    for (int beetleIndex = 0; beetleIndex < MAX_BEETLE_NUM; ++beetleIndex)
    {
        if (m_beetle[beetleIndex] && !m_beetle[beetleIndex]->IsDead())
        {
            m_beetle[beetleIndex]->MarkAsDead();
            m_beetle[beetleIndex]->MarkAsGarbage();

            // #TODO: FIX
            SpawnDebrisCluster(m_beetle[beetleIndex]->GetPosition(),
                               Vec2(0.2f, 0.2f),
                               30,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_beetle[beetleIndex]->GetColor());
        }
    }

    for (int waspIndex = 0; waspIndex < MAX_WASP_NUM; ++waspIndex)
    {
        if (m_wasp[waspIndex] && !m_wasp[waspIndex]->IsDead())
        {
            m_wasp[waspIndex]->MarkAsDead();
            m_wasp[waspIndex]->MarkAsGarbage();
            SpawnDebrisCluster(m_wasp[waspIndex]->GetPosition(),
                               Vec2(0.2f, 0.2f),
                               30,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_wasp[waspIndex]->GetColor());
        }
    }

    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_NUM; ++asteroidIndex)
    {
        if (m_asteroids[asteroidIndex] && !m_asteroids[asteroidIndex]->IsDead())
        {
            m_asteroids[asteroidIndex]->MarkAsDead();
            m_asteroids[asteroidIndex]->MarkAsGarbage();

            SpawnDebrisCluster(m_asteroids[asteroidIndex]->GetPosition(),
                               Vec2(0.2f, 0.2f),
                               30,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_asteroids[asteroidIndex]->GetColor());
        }
    }

    for (int boxIndex = 0; boxIndex < MAX_BOX_NUM; ++boxIndex)
    {
        if (m_boxes[boxIndex] && !m_boxes[boxIndex]->IsDead())
        {
            m_boxes[boxIndex]->MarkAsDead();
            m_boxes[boxIndex]->MarkAsGarbage();
            SpawnDebrisCluster(m_boxes[boxIndex]->GetBoxCollider().GetCenter(),
                               Vec2(0.2f, 0.2f),
                               30,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_boxes[boxIndex]->GetColor());
        }
    }
}

//----------------------------------------------------------------------------------------------------
void Game::SetAttractMode(const bool isAttractMode)
{
    m_isAttractMode = isAttractMode;
}

void Game::SetPlayerNameInputMode(const bool isPlayerNameInputMode)
{
    m_isPlayerNameInputMode = isPlayerNameInputMode;
    printf("SetPlayerNameInputMode: %hhd\n", isPlayerNameInputMode);
}

void Game::SetPlayerShipIsReadyToSpawnBullet(const bool isReadyToSpawnBullet) const
{
    m_playerShip->IsReadyToSpawnBullet(isReadyToSpawnBullet);
    printf("SetPlayerShipIsReadyToSpawnBullet: %hhd\n", isReadyToSpawnBullet);
}

bool Game::IsAttractMode() const
{
    return m_isAttractMode;
}

bool Game::IsPlayerNameInputMode() const
{
    return m_isPlayerNameInputMode;
}

int Game::GetHighScore() const
{
    return m_highScore;
}

//----------------------------------------------------------------------------------------------------
STATIC bool Game::Command_SetTimeScale(EventArgs& args)
{
    float const value = args.GetValue("scale", -1.f);

    if (value == -1.f)
    {
        g_theDevConsole->AddLine(DevConsole::ERROR,"Your keyName is not valid, should be setscale scale=YourValue!");
        return false;
    }

    g_theGame->m_gameClock->SetTimeScale(value);

    return true;
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnPlayerShip()
{
    m_playerShip = new PlayerShip(this, Vec2(20.f, WORLD_CENTER_Y), 0.f, m_playerShipHealth, false);
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnBeetle(Vec2 const& position)
{
    for (int beetleIndex = 0; beetleIndex < MAX_BEETLE_NUM; beetleIndex++)
    {
        if (m_beetle[beetleIndex])
            continue;

        m_beetle[beetleIndex] = new Beetle(this,
                                           position,
                                           0.f);

        return;
    }
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnWasp(Vec2 const& position)
{
    for (int waspIndex = 0; waspIndex < MAX_WASP_NUM; waspIndex++)
    {
        if (m_wasp[waspIndex])
            continue;

        m_wasp[waspIndex] = new Wasp(this,
                                     position,
                                     0.f);

        return;
    }
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnAsteroid(Vec2 const& position)
{
    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_NUM; asteroidIndex++)
    {
        if (m_asteroids[asteroidIndex])
            continue;

        m_asteroids[asteroidIndex] = new Asteroid(this,
                                                  position,
                                                  0.f);

        return;
    }

    ERROR_RECOVERABLE("Cannot spawn a new bullet; all slots are full")
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnDebris(Vec2 const& position, Vec2 const& velocity, float radius, Rgba8 color)
{
    for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS_NUM; debrisIndex++)
    {
        if (m_debris[debrisIndex])
            continue;

        m_debris[debrisIndex] = new Debris(this, position, velocity, radius, color);

        return;
    }
}

//----------------------------------------------------------------------------------------------------
void Game::SpawnDebrisCluster(Vec2 const& position, Vec2 const& velocity, int numDebris, float radius, Rgba8 color)
{
    for (int debrisIndex = 0; debrisIndex < numDebris; debrisIndex++)
    {
        float randomRadius = g_theRNG->RollRandomFloatInRange(1.f, 5.f) * radius;
        float randomX      = g_theRNG->RollRandomFloatInRange(0.f, 360.f);
        float randomY      = g_theRNG->RollRandomFloatInRange(0.f, 360.f);

        SpawnDebris(position, Vec2(velocity.x * randomX, velocity.y * randomY), randomRadius, color);
    }
}

void Game::SpawnBox(Vec2 const& position)
{
    for (int boxIndex = 0; boxIndex < MAX_BOX_NUM; boxIndex++)
    {
        if (m_boxes[boxIndex])
            continue;

        m_boxes[boxIndex] = new Box(this,
                                    position,
                                    0.f);

        return;
    }
}

void Game::SpawnBoxCluster()
{
    float yPosUp   = WORLD_SIZE_Y - BOX_SIDE_LENGTH * 1.1f;
    float yPosDown = BOX_SIDE_LENGTH * 0.1f;

    int boxNumUp   = g_theRNG->RollRandomIntInRange(1, 10);
    int boxNumDown = g_theRNG->RollRandomIntInRange(1, 10);

    for (int i = 0; i < boxNumUp; ++i)
    {
        SpawnBox(Vec2(WORLD_SIZE_X, yPosUp));
        yPosUp -= BOX_SIDE_LENGTH * 1.1f;
    }

    for (int i = 0; i < boxNumDown; ++i)
    {
        SpawnBox(Vec2(WORLD_SIZE_X, yPosDown));
        yPosDown += BOX_SIDE_LENGTH * 1.1f;
    }
}


//----------------------------------------------------------------------------------------------------
void Game::UpdateEntities(float deltaSeconds)
{
    if (m_isAttractMode)
        return;

    HandleEntityIsOffScreen();
    HandleEntityCollision();

    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_NUM; asteroidIndex++)
    {
        if (!m_asteroids[asteroidIndex])
            continue;

        m_asteroids[asteroidIndex]->Update(deltaSeconds);
    }

    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS_NUM; bulletIndex++)
    {
        if (!m_bullets[bulletIndex])
            continue;

        m_bullets[bulletIndex]->Update(deltaSeconds);
    }

    for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS_NUM; debrisIndex++)
    {
        if (!m_debris[debrisIndex])
            continue;

        m_debris[debrisIndex]->Update(deltaSeconds);
    }

    for (int beetleIndex = 0; beetleIndex < MAX_BEETLE_NUM; beetleIndex++)
    {
        if (!m_beetle[beetleIndex])
            continue;

        m_beetle[beetleIndex]->Update(deltaSeconds);
    }

    for (int waspIndex = 0; waspIndex < MAX_WASP_NUM; waspIndex++)
    {
        if (!m_wasp[waspIndex])
            continue;

        m_wasp[waspIndex]->Update(deltaSeconds);
    }

    for (int boxIndex = 0; boxIndex < MAX_BOX_NUM; boxIndex++)
    {
        if (!m_boxes[boxIndex])
            continue;

        m_boxes[boxIndex]->Update(deltaSeconds);
    }

    m_accumulatedTime += deltaSeconds;

    if (m_accumulatedTime >= 1.0f)
    {
        SpawnBoxCluster();

        m_accumulatedTime = 0.0f;
    }

    if (m_playerShip)
    {
        m_playerShip->Update(deltaSeconds);
    }
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromKeyBoard()
{
    if (!m_isAttractMode &&
        g_theInput->WasKeyJustPressed(KEYCODE_F1))
        m_isDebugRendering = !m_isDebugRendering;

    if (m_theUIHandler->IsFirstButtonSelected() &&
        g_theInput->WasKeyJustPressed(KEYCODE_ENTER))
    {
        if (m_isPlayerNameInputMode)
        {
            SetAttractMode(false);
            SetPlayerNameInputMode(false);
            SetPlayerShipIsReadyToSpawnBullet(true);
        }
    }

    if (!g_theInput->WasKeyJustPressed(KEYCODE_ENTER) &&
        g_theInput->IsKeyDown(KEYCODE_ENTER))
        m_playerShip->IsReadyToSpawnBullet(true);

    if (g_theInput->WasKeyJustPressed('I'))
        SpawnAsteroid(GetOffScreenPosition(ASTEROID_COSMETIC_RADIUS));

    if (g_theInput->WasKeyJustPressed('N') &&
        m_playerShip->IsDead() &&
        m_playerShipHealth != 0)
    {
        SpawnPlayerShip();
        SetPlayerShipIsReadyToSpawnBullet(true);
    }
}

//----------------------------------------------------------------------------------------------------
void Game::UpdateFromController()
{
    XboxController const& controller = g_theInput->GetController(0);

    if (controller.WasButtonJustPressed(XBOX_BUTTON_START))
    {
        if (m_isPlayerNameInputMode)
        {
            SetAttractMode(false);
            SetPlayerNameInputMode(false);
            SetPlayerShipIsReadyToSpawnBullet(true);
        }
    }

    if (!controller.WasButtonJustPressed(XBOX_BUTTON_START) &&
        controller.IsButtonDown(XBOX_BUTTON_START))
        m_playerShip->IsReadyToSpawnBullet(true);

    if (controller.WasButtonJustPressed(XBOX_BUTTON_RTHUMB))
        SpawnAsteroid(GetOffScreenPosition(ASTEROID_COSMETIC_RADIUS));

    if (controller.WasButtonJustPressed(XBOX_BUTTON_START) &&
        m_playerShip->IsDead() &&
        m_playerShipHealth != 0)
        SpawnPlayerShip();
}

//----------------------------------------------------------------------------------------------------
void Game::RenderEntities() const
{
    if (m_playerShip)
        m_playerShip->Render();

    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS_NUM; bulletIndex++)
    {
        if (!m_bullets[bulletIndex])
            continue;


        m_bullets[bulletIndex]->Render();
    }

    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_NUM; asteroidIndex++)
    {
        if (!m_asteroids[asteroidIndex])
            continue;

        m_asteroids[asteroidIndex]->Render();
    }

    for (int beetleIndex = 0; beetleIndex < MAX_BEETLE_NUM; beetleIndex++)
    {
        if (!m_beetle[beetleIndex])
            continue;

        m_beetle[beetleIndex]->Render();
    }

    for (int waspIndex = 0; waspIndex < MAX_WASP_NUM; waspIndex++)
    {
        if (!m_wasp[waspIndex])
            continue;

        m_wasp[waspIndex]->Render();
    }

    for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS_NUM; debrisIndex++)
    {
        if (!m_debris[debrisIndex])
            continue;


        m_debris[debrisIndex]->Render();
    }
    for (int boxIndex = 0; boxIndex < MAX_BOX_NUM; boxIndex++)
    {
        if (!m_boxes[boxIndex])
            continue;

        m_boxes[boxIndex]->Render();
    }
}

void Game::RenderDevConsole() const
{
    AABB2 const box = AABB2(Vec2::ZERO, Vec2(1600.f, 100.f));

    g_theDevConsole->Render(box);
}

//----------------------------------------------------------------------------------------------------
void Game::DebugRenderEntities() const
{
    if (!m_isDebugRendering)
        return;

    if (m_playerShip)
        m_playerShip->DebugRender();

    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS_NUM; ++bulletIndex)
    {
        if (!m_bullets[bulletIndex])
            continue;

        m_bullets[bulletIndex]->DebugRender();
    }

    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_NUM; ++asteroidIndex)
    {
        if (!m_asteroids[asteroidIndex])
            continue;

        m_asteroids[asteroidIndex]->DebugRender();
    }

    for (int beetleIndex = 0; beetleIndex < MAX_BEETLE_NUM; ++beetleIndex)
    {
        if (!m_beetle[beetleIndex])
            continue;

        m_beetle[beetleIndex]->DebugRender();
    }

    for (int waspIndex = 0; waspIndex < MAX_WASP_NUM; ++waspIndex)
    {
        if (!m_wasp[waspIndex])
            continue;

        m_wasp[waspIndex]->DebugRender();
    }

    for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS_NUM; ++debrisIndex)
    {
        if (!m_debris[debrisIndex])
            continue;

        m_debris[debrisIndex]->DebugRender();
    }

    for (int boxIndex = 0; boxIndex < MAX_BOX_NUM; ++boxIndex)
    {
        if (!m_boxes[boxIndex])
            continue;

        m_boxes[boxIndex]->DebugRender();
    }
}

void Game::SpawnRandomEnemy(int boxIndex)
{
    switch (g_theRNG->RollRandomIntInRange(0, 2))
    {
    case 0:
        SpawnAsteroid(m_boxes[boxIndex]->GetBoxCollider().GetCenter());
        break;

    case 1:
        SpawnBeetle(m_boxes[boxIndex]->GetBoxCollider().GetCenter());
        break;

    case 2:
        SpawnWasp(m_boxes[boxIndex]->GetBoxCollider().GetCenter());
        break;
    }
}

// #TODO: fix debris velocity
//----------------------------------------------------------------------------------------------------
void Game::HandleEntityCollision()
{
    // PlayerShip vs. Asteroid
    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_NUM; ++asteroidIndex)
    {
        if (!m_asteroids[asteroidIndex])
            continue;

        if (!m_playerShip)
            continue;

        if (m_playerShip->IsDead())
            continue;

        if (DoDiscsOverlap(m_playerShip->GetPosition(),
                           PLAYER_SHIP_PHYSICS_RADIUS,
                           m_asteroids[asteroidIndex]->GetPosition(),
                           ASTEROID_PHYSICS_RADIUS))
        {
            m_playerShip->SetHealth(1);
            m_playerShip->MarkAsDead();
            m_playerShipHealth = m_playerShip->GetHealth();
            m_asteroids[asteroidIndex]->SetHealth(1);

            const SoundID IN_GAME_ENTITY_HIT = g_theAudio->CreateOrGetSound("Data/Audio/InGame_Entity_Hit.mp3");
            g_theAudio->StartSound(IN_GAME_ENTITY_HIT, false, 1.f, 0.f, 1.f, false);

            SpawnDebrisCluster(m_playerShip->GetPosition(),
                               m_asteroids[asteroidIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                               30,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_playerShip->GetColor());
        }

        if (m_asteroids[asteroidIndex]->GetHealth() == 0)
        {
            SpawnDebrisCluster(m_asteroids[asteroidIndex]->GetPosition(),
                               -m_asteroids[asteroidIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                               12,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_asteroids[asteroidIndex]->GetColor());

            m_asteroids[asteroidIndex]->MarkAsDead();
            m_asteroids[asteroidIndex]->MarkAsGarbage();
        }
    }

    // PlayerShip vs. Beetle
    for (int beetleIndex = 0; beetleIndex < MAX_BEETLE_NUM; ++beetleIndex)
    {
        if (!m_beetle[beetleIndex])
            continue;

        if (!m_playerShip)
            continue;

        if (m_playerShip->IsDead())
            continue;


        if (DoDiscsOverlap(m_playerShip->GetPosition(),
                           PLAYER_SHIP_PHYSICS_RADIUS,
                           m_beetle[beetleIndex]->GetPosition(),
                           BEETLE_PHYSICS_RADIUS))
        {
            m_playerShip->SetHealth(1);
            m_playerShip->MarkAsDead();
            m_playerShipHealth = m_playerShip->GetHealth();
            m_beetle[beetleIndex]->SetHealth(1);

            const SoundID IN_GAME_ENTITY_HIT = g_theAudio->CreateOrGetSound("Data/Audio/InGame_Entity_Hit.mp3");
            g_theAudio->StartSound(IN_GAME_ENTITY_HIT, false, 1.f, 0.f, 1.f, false);

            SpawnDebrisCluster(m_playerShip->GetPosition(),
                               m_beetle[beetleIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                               30,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_playerShip->GetColor());

            continue;
        }

        if (m_beetle[beetleIndex]->GetHealth() == 0)
        {
            SpawnDebrisCluster(m_beetle[beetleIndex]->GetPosition(),
                               m_beetle[beetleIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                               12,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_beetle[beetleIndex]->GetColor());

            m_beetle[beetleIndex]->MarkAsDead();
            m_beetle[beetleIndex]->MarkAsGarbage();
        }
    }

    // PlayerShip vs. Wasp
    for (int waspIndex = 0; waspIndex < MAX_WASP_NUM; ++waspIndex)
    {
        if (!m_wasp[waspIndex])
            continue;

        if (!m_playerShip)
            continue;

        if (m_playerShip->IsDead())
            continue;


        if (DoDiscsOverlap(m_playerShip->GetPosition(),
                           PLAYER_SHIP_PHYSICS_RADIUS,
                           m_wasp[waspIndex]->GetPosition(),
                           WASP_PHYSICS_RADIUS))
        {
            const SoundID IN_GAME_ENTITY_HIT = g_theAudio->CreateOrGetSound("Data/Audio/InGame_Entity_Hit.mp3");
            g_theAudio->StartSound(IN_GAME_ENTITY_HIT, false, 1.f, 0.f, 1.f, false);

            m_playerShip->SetHealth(1);
            m_playerShip->MarkAsDead();
            m_playerShipHealth = m_playerShip->GetHealth();
            m_wasp[waspIndex]->SetHealth(1);

            SpawnDebrisCluster(m_playerShip->GetPosition(),
                               m_wasp[waspIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                               30,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_playerShip->GetColor());
        }


        if (m_wasp[waspIndex]->GetHealth() == 0)
        {
            SpawnDebrisCluster(m_wasp[waspIndex]->GetPosition(),
                               -m_wasp[waspIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                               12,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_wasp[waspIndex]->GetColor());

            m_wasp[waspIndex]->MarkAsDead();
            m_wasp[waspIndex]->MarkAsGarbage();
        }
    }


    //  Bullet vs. Asteroid
    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_NUM; ++asteroidIndex)
    {
        for (int bulletIndex = 0; bulletIndex < MAX_BULLETS_NUM; ++bulletIndex)
        {
            if (!m_asteroids[asteroidIndex])
                continue;


            if (!m_bullets[bulletIndex])
                continue;

            if (DoDiscsOverlap(m_bullets[bulletIndex]->GetPosition(),
                               BULLET_PHYSICS_RADIUS,
                               m_asteroids[asteroidIndex]->GetPosition(),
                               ASTEROID_PHYSICS_RADIUS))
            {
                const SoundID IN_GAME_ENTITY_HIT = g_theAudio->CreateOrGetSound("Data/Audio/InGame_Entity_Hit.mp3");
                g_theAudio->StartSound(IN_GAME_ENTITY_HIT, false, 1.f, 0.f, 1.f, false);

                SpawnDebrisCluster(m_bullets[bulletIndex]->GetPosition(),
                                   m_asteroids[asteroidIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                                   3,
                                   ENTITY_HIT_DEBRIS_RADIUS,
                                   m_asteroids[asteroidIndex]->GetColor());

                m_playerShip->SetPlayerScore(10);

                m_bullets[bulletIndex]->MarkAsDead();
                m_bullets[bulletIndex]->MarkAsGarbage();
                m_asteroids[asteroidIndex]->SetHealth(1);
            }

            if (m_asteroids[asteroidIndex]->GetHealth() != 0)
                continue;

            SpawnDebrisCluster(m_asteroids[asteroidIndex]->GetPosition(),
                               -m_asteroids[asteroidIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                               12,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_asteroids[asteroidIndex]->GetColor());

            m_playerShip->SetPlayerScore(100);

            m_asteroids[asteroidIndex]->MarkAsDead();
            m_asteroids[asteroidIndex]->MarkAsGarbage();
        }
    }


    // Bullets vs. Beetle
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS_NUM; ++bulletIndex)
    {
        for (int beetleIndex = 0; beetleIndex < MAX_BEETLE_NUM; ++beetleIndex)
        {
            if (!m_bullets[bulletIndex])
                continue;


            if (!m_beetle[beetleIndex])
                continue;

            if (DoDiscsOverlap(m_bullets[bulletIndex]->GetPosition(),
                               BULLET_PHYSICS_RADIUS,
                               m_beetle[beetleIndex]->GetPosition(),
                               BEETLE_PHYSICS_RADIUS))
            {
                const SoundID IN_GAME_ENTITY_HIT = g_theAudio->CreateOrGetSound("Data/Audio/InGame_Entity_Hit.mp3");
                g_theAudio->StartSound(IN_GAME_ENTITY_HIT, false, 1.f, 0.f, 1.f, false);

                SpawnDebrisCluster(m_bullets[bulletIndex]->GetPosition(),
                                   m_beetle[beetleIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                                   3,
                                   ENTITY_HIT_DEBRIS_RADIUS,
                                   m_beetle[beetleIndex]->GetColor());

                m_playerShip->SetPlayerScore(20);

                m_bullets[bulletIndex]->MarkAsDead();
                m_bullets[bulletIndex]->MarkAsGarbage();
                m_beetle[beetleIndex]->SetHealth(1);
            }

            if (m_beetle[beetleIndex]->GetHealth() != 0)
                continue;

            SpawnDebrisCluster(m_beetle[beetleIndex]->GetPosition(),
                               -m_beetle[beetleIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                               12,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_beetle[beetleIndex]->GetColor());

            m_playerShip->SetPlayerScore(200);

            m_beetle[beetleIndex]->MarkAsDead();
            m_beetle[beetleIndex]->MarkAsGarbage();
        }
    }

    // Bullets vs. Wasp
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS_NUM; ++bulletIndex)
    {
        for (int waspIndex = 0; waspIndex < MAX_WASP_NUM; ++waspIndex)
        {
            if (!m_bullets[bulletIndex])
                continue;

            if (!m_wasp[waspIndex])
                continue;

            if (DoDiscsOverlap(m_bullets[bulletIndex]->GetPosition(),
                               BULLET_PHYSICS_RADIUS,
                               m_wasp[waspIndex]->GetPosition(),
                               WASP_PHYSICS_RADIUS))
            {
                const SoundID IN_GAME_ENTITY_HIT = g_theAudio->CreateOrGetSound("Data/Audio/InGame_Entity_Hit.mp3");
                g_theAudio->StartSound(IN_GAME_ENTITY_HIT, false, 1.f, 0.f, 1.f, false);

                SpawnDebrisCluster(m_bullets[bulletIndex]->GetPosition(),
                                   m_wasp[waspIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                                   3,
                                   ENTITY_HIT_DEBRIS_RADIUS,
                                   m_wasp[waspIndex]->GetColor());

                m_playerShip->SetPlayerScore(50);

                m_bullets[bulletIndex]->MarkAsDead();
                m_bullets[bulletIndex]->MarkAsGarbage();
                m_wasp[waspIndex]->SetHealth(1);
            }

            if (m_wasp[waspIndex]->GetHealth() != 0)
                continue;

            SpawnDebrisCluster(m_wasp[waspIndex]->GetPosition(),
                               -m_wasp[waspIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                               12,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_wasp[waspIndex]->GetColor());

            m_playerShip->SetPlayerScore(500);

            m_wasp[waspIndex]->MarkAsDead();
            m_wasp[waspIndex]->MarkAsGarbage();
        }
    }

    // Bullets vs. Box
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS_NUM; ++bulletIndex)
    {
        for (int boxIndex = 0; boxIndex < MAX_BOX_NUM; ++boxIndex)
        {
            if (!m_bullets[bulletIndex])
                continue;

            if (!m_boxes[boxIndex])
                continue;

            if (m_boxes[boxIndex]->GetBoxCollider().IsPointInside(m_bullets[bulletIndex]->GetPosition()))
            {
                const SoundID IN_GAME_ENTITY_HIT = g_theAudio->CreateOrGetSound("Data/Audio/InGame_Entity_Hit.mp3");
                g_theAudio->StartSound(IN_GAME_ENTITY_HIT, false, 1.f, 0.f, 1.f, false);

                SpawnDebrisCluster(m_bullets[bulletIndex]->GetPosition(),
                                   -m_bullets[bulletIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                                   3,
                                   ENTITY_HIT_DEBRIS_RADIUS,
                                   m_boxes[boxIndex]->GetColor());

                m_playerShip->SetPlayerScore(1);

                m_bullets[bulletIndex]->MarkAsDead();
                m_bullets[bulletIndex]->MarkAsGarbage();
                m_boxes[boxIndex]->SetHealth(1);
            }

            if (m_boxes[boxIndex]->GetHealth() != 0)
                continue;

            SpawnDebrisCluster(m_boxes[boxIndex]->GetPosition(),
                               m_bullets[bulletIndex]->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                               12,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_boxes[boxIndex]->GetColor());

            m_playerShip->SetPlayerScore(1);

            m_boxes[boxIndex]->MarkAsDead();
            m_boxes[boxIndex]->MarkAsGarbage();

            SpawnRandomEnemy(boxIndex);
        }
    }
    HandleCollisionBetweenPlayerShipAndBox();
}

void Game::HandleCollisionBetweenPlayerShipAndBox()
{
    // PlayerShip vs. Box
    for (int boxIndex = 0; boxIndex < MAX_BOX_NUM; ++boxIndex)
    {
        if (!m_boxes[boxIndex])
            continue;

        if (!m_playerShip)
            continue;

        if (m_playerShip->IsDead())
            continue;

        if (DoDiscsOverlap(m_playerShip->GetPosition(),
                           PLAYER_SHIP_COSMETIC_RADIUS,
                           m_boxes[boxIndex]->GetBoxCollider().GetCenter(),
                           BOX_SIDE_LENGTH / 2.f))
        {
            PushDiscOutOfAABB2D(m_playerShip->GetPositionAndSet(), PLAYER_SHIP_PHYSICS_RADIUS,
                                m_boxes[boxIndex]->GetBoxCollider());

            SpawnDebrisCluster(m_playerShip->GetPosition(),
                               -m_playerShip->GetVelocity().GetNormalized() * m_debrisVelocityRate,
                               30,
                               ENTITY_DEAD_DEBRIS_RADIUS,
                               m_playerShip->GetColor());

            Vec2       nearestPoint                  = m_boxes[boxIndex]->GetBoxCollider().GetNearestPoint(m_playerShip->GetPosition());
            Vec2       normalOfSurfaceToReflectOffOf = (m_playerShip->GetPosition() - nearestPoint).GetNormalized();
            const Vec2 newVelocity                   = m_playerShip->GetVelocity().GetReflected(normalOfSurfaceToReflectOffOf);

            m_playerShip->GetVelocityAndSet() = newVelocity;
        }
    }
}

//-----------------------------------------------------------------------------------------------
void Game::HandleEntityIsOffScreen() const
{
    for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS_NUM; ++debrisIndex)
    {
        if (!m_debris[debrisIndex])
            continue;

        if (!m_debris[debrisIndex]->IsOffScreen())
            continue;

        m_debris[debrisIndex]->MarkAsDead();
        m_debris[debrisIndex]->MarkAsGarbage();
    }

    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS_NUM; ++bulletIndex)
    {
        if (!m_bullets[bulletIndex])
            continue;

        if (!m_bullets[bulletIndex]->IsOffScreen())
            continue;

        m_bullets[bulletIndex]->MarkAsDead();
        m_bullets[bulletIndex]->MarkAsGarbage();
    }
}

//-----------------------------------------------------------------------------------------------
Vec2 Game::GetOffScreenPosition(float entityCosmeticRadius) const
{
    float initialY = 0;

    // switch (g_theRNG->RollRandomIntInRange(1, 4))
    // {
    // case 1: // off right bound
    // 	initialX = g_theRNG->RollRandomFloatInRange(WORLD_SIZE_X, WORLD_SIZE_X + entityCosmeticRadius);
    // 	initialY = g_theRNG->RollRandomFloatInRange(0.f, WORLD_SIZE_Y);
    // 	break;
    //
    // case 2: // off left bound
    // 	initialX = g_theRNG->RollRandomFloatInRange(-entityCosmeticRadius, 0.f);
    // 	initialY = g_theRNG->RollRandomFloatInRange(0.f, WORLD_SIZE_Y);
    // 	break;
    //
    // case 3: // off top bound
    // 	initialX = g_theRNG->RollRandomFloatInRange(0.f, WORLD_SIZE_X);
    // 	initialY = g_theRNG->RollRandomFloatInRange(WORLD_SIZE_Y, WORLD_SIZE_Y + entityCosmeticRadius);
    // 	break;
    //
    // case 4: // off bottom bound
    // 	initialX = g_theRNG->RollRandomFloatInRange(0.f, WORLD_SIZE_X);
    // 	initialY = g_theRNG->RollRandomFloatInRange(-entityCosmeticRadius, 0.f);
    // 	break;
    // }

    float initialX = g_theRNG->RollRandomFloatInRange(WORLD_SIZE_X, WORLD_SIZE_X + entityCosmeticRadius);
    // 	initialY = g_theRNG->RollRandomFloatInRange(0.f, WORLD_SIZE_Y);

    return Vec2(initialX, initialY);
}

//-----------------------------------------------------------------------------------------------
void Game::DeleteGarbageEntities()
{
    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_NUM; ++asteroidIndex)
    {
        if (m_asteroids[asteroidIndex] &&
            m_asteroids[asteroidIndex]->IsGarbage())
        {
            delete m_asteroids[asteroidIndex];
            m_asteroids[asteroidIndex] = nullptr;
        }
    }

    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS_NUM; ++bulletIndex)
    {
        if (m_bullets[bulletIndex] &&
            m_bullets[bulletIndex]->IsGarbage())
        {
            delete m_bullets[bulletIndex];
            m_bullets[bulletIndex] = nullptr;
        }
    }

    for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS_NUM; ++debrisIndex)
    {
        if (m_debris[debrisIndex] &&
            m_debris[debrisIndex]->IsGarbage())
        {
            delete m_debris[debrisIndex];
            m_debris[debrisIndex] = nullptr;
        }
    }

    for (int beetleIndex = 0; beetleIndex < MAX_BEETLE_NUM; ++beetleIndex)
    {
        if (m_beetle[beetleIndex] &&
            m_beetle[beetleIndex]->IsGarbage())
        {
            delete m_beetle[beetleIndex];
            m_beetle[beetleIndex] = nullptr;
        }
    }

    for (int waspIndex = 0; waspIndex < MAX_WASP_NUM; ++waspIndex)
    {
        if (m_wasp[waspIndex] &&
            m_wasp[waspIndex]->IsGarbage())
        {
            delete m_wasp[waspIndex];
            m_wasp[waspIndex] = nullptr;
        }
    }

    for (int boxIndex = 0; boxIndex < MAX_BOX_NUM; ++boxIndex)
    {
        if (m_boxes[boxIndex] &&
            m_boxes[boxIndex]->IsGarbage())
        {
            delete m_boxes[boxIndex];
            m_boxes[boxIndex] = nullptr;
        }
    }
}

//-----------------------------------------------------------------------------------------------
void Game::SpawnEnemiesForCurrentWave()
{
    const LevelData& currentWaveData = LEVEL_DATA[m_currentWave];

    for (int i = 0; i < currentWaveData.beetleCount; ++i)
    {
        SpawnBeetle(GetOffScreenPosition(BEETLE_COSMETIC_RADIUS));
    }

    for (int i = 0; i < currentWaveData.waspCount; ++i)
    {
        SpawnWasp(GetOffScreenPosition(WASP_COSMETIC_RADIUS));
    }

    for (int i = 0; i < currentWaveData.asteroidCount; ++i)
    {
        SpawnAsteroid(GetOffScreenPosition(ASTEROID_COSMETIC_RADIUS));
    }
}

//-----------------------------------------------------------------------------------------------
bool Game::AreAllEnemiesDead() const
{
    for (int beetleIndex = 0; beetleIndex < MAX_BEETLE_NUM; ++beetleIndex)
    {
        if (m_beetle[beetleIndex] && !m_beetle[beetleIndex]->IsDead())
            return false;
    }

    for (int waspIndex = 0; waspIndex < MAX_WASP_NUM; ++waspIndex)
    {
        if (m_wasp[waspIndex] && !m_wasp[waspIndex]->IsDead())
            return false;
    }

    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS_NUM; ++asteroidIndex)
    {
        if (m_asteroids[asteroidIndex] && !m_asteroids[asteroidIndex]->IsDead())
            return false;
    }

    return true;
}

void Game::DoShakeCamera(float deltaSeconds)
{
    // Shake the world camera if there is any shake left
    if (m_shakeDuration > 0.f)
    {
        // Reduce shake duration and intensity over time
        m_shakeDuration -= deltaSeconds;
        m_shakeIntensity *= 0.999f; // Decay the intensity

        // Generate random shake offsets
        const float shakeX = g_theRNG->RollRandomFloatInRange(-m_shakeIntensity, m_shakeIntensity);
        const float shakeY = g_theRNG->RollRandomFloatInRange(-m_shakeIntensity, m_shakeIntensity);
        const Vec2  shakeOffset(shakeX * shakeX, shakeY * shakeY);

        // Reset camera to base position before applying shake
        m_worldCamera->SetOrthoGraphicView(m_baseCameraPos, m_baseCameraPos + Vec2(WORLD_SIZE_X, WORLD_SIZE_Y));

        // Apply the shake
        m_worldCamera->Translate2D(shakeOffset);
    }
    else
    {
        // No more shake, reset the camera to its base position
        ResetCamera();
    }
}

void Game::ResetCamera() const
{
    m_worldCamera->SetOrthoGraphicView(m_baseCameraPos, m_baseCameraPos + Vec2(200.f, 100.f));
}

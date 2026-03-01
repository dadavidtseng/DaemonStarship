//----------------------------------------------------------------------------------------------------
// Game.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------------------------
#include "Game/Asteroid.hpp"
#include "Game/Beetle.hpp"
#include "Game/Box.hpp"
#include "Game/Bullet.hpp"
#include "Game/Debris.hpp"
#include "Game/GameCommon.hpp"
#include "Game/PlayerShip.hpp"
#include "Game/Wasp.hpp"
//----------------------------------------------------------------------------------------------------
#include "Engine/Audio/AudioTypes.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/EventSystem.hpp"

//-forward-declaration--------------------------------------------------------------------------------
class Camera;
class ScoreBoardHandler;
class UIHandler;

//-----------------------------------------------------------------------------------------------
enum class eGameState
{
    AttractMode,
    PlayerNameInput,
    Playing,
    ScoreboardDisplay,
};

//-----------------------------------------------------------------------------------------------
class Game
{
public:
    Game();
    ~Game();
    //-----------------------------------------------------------------------------------------------
    void Update();
    void Render();
    void DebugRender() const;
    void ResetData();
    //-----------------------------------------------------------------------------------------------
    // high-level game mechanics(e.g.levels / waves, spawning)
    void        SpawnBullet(Vec2 const& position, float orientationDegrees);
    [[nodiscard]] PlayerShip* GetPlayerShip() const;
    void        MarkAllEntityAsDeadAndGarbage();
    void        SetGameState(eGameState state);
    [[nodiscard]] bool        IsAttractMode() const;
    [[nodiscard]] bool        IsPlayerNameInputMode() const;
    [[nodiscard]] bool        IsPlaying() const;
    void        SetPlayerShipIsReadyToSpawnBullet(bool isReadyToSpawnBullet) const;
    [[nodiscard]] int         GetHighScore() const;

    static bool Command_SetTimeScale(EventArgs& args);

private:
    void SpawnPlayerShip();
    void SpawnBeetle(Vec2 const& position);
    void SpawnWasp(Vec2 const& position);
    void SpawnAsteroid(Vec2 const& position);
    void SpawnDebris(Vec2 const& position, Vec2 const& velocity, float radius, Rgba8 color);
    void SpawnDebrisCluster(Vec2 const& position, Vec2 const& velocity, int numDebris, float radius, Rgba8 color);
    void SpawnBox(Vec2 const& position);
    void SpawnBoxCluster();
    void SpawnRandomEnemy(int boxIndex);
    void UpdateEntities(float deltaSeconds);
    void UpdateFromKeyBoard();
    void UpdateFromController();
    void RenderEntities() const;
    void DebugRenderEntities() const;

    // entity-vs-entity interactions (e.g. physics, damage)
    void HandleEntityCollision();
    void HandleCollisionBetweenPlayerShipAndBox();
    void HandleEntityIsOffScreen() const;

    [[nodiscard]] Vec2 GetOffScreenPosition(float entityCosmeticRadius) const;

    void DeleteGarbageEntities();
    void SpawnEnemiesForCurrentWave();
    [[nodiscard]] bool AreAllEnemiesDead() const;
    void DoShakeCamera(float deltaSeconds);
    void ResetCamera() const;

    // #TODO: Entity* Lists / dynamic_cast<Asteroid*>(m_asteroid[asteroidIndex])

    //-----------------------------------------------------------------------------------------------
    // Template pool helpers — eliminate repeated for-loop patterns across entity pools
    template <typename T, int N>
    void ForEachInPool(T* (&pool)[N], auto&& fn);

    template <typename T, int N>
    void ForEachInPool(T* const (&pool)[N], auto&& fn) const;

    template <typename T, int N>
    T** FindEmptySlot(T* (&pool)[N]);

    template <typename T, int N>
    void DeletePool(T* (&pool)[N]);

    template <typename T, int N>
    void DeleteGarbageInPool(T* (&pool)[N]);

    PlayerShip*        m_playerShip                   = nullptr; // Just one player ship (for now...)
    Bullet*            m_bullets[MAX_BULLETS_NUM]     = {};      // Fixed number of asteroid �slots�; nullptr if unused.
    Asteroid*          m_asteroids[MAX_ASTEROIDS_NUM] = {};      // The �= {};� syntax initializes the array to zeros.
    Beetle*            m_beetles[MAX_BEETLE_NUM]       = {};
    Wasp*              m_wasps[MAX_WASP_NUM]           = {};
    Debris*            m_debris[MAX_DEBRIS_NUM]       = {};
    Box*               m_boxes[MAX_BOX_NUM]           = {};
    Camera*            m_worldCamera                  = nullptr;
    Camera*            m_screenCamera                 = nullptr;
    int                m_currentWave                  = 0;
    float              m_timeSinceDeath               = 0.f;
    int                m_playerShipHealth             = MAX_PLAYER_SHIP_HEALTH;
    eGameState         m_gameState                    = eGameState::AttractMode;
    bool               m_isDebugRendering             = false;
    UIHandler*         m_theUIHandler                 = nullptr;
    float              m_shakeIntensity               = 5.f; // Current intensity of the shake
    float              m_shakeDuration                = 20.f;  // Time remaining for the shake
    Vec2               m_baseCameraPos                = Vec2::ZERO;
    float              m_accumulatedTime              = 0.f;
    ScoreBoardHandler* m_theScoreBoardHandler         = nullptr;
    float              m_debrisVelocityRate           = 0.5f;
    int                m_highScore                    = 0;
    Clock*             m_gameClock                    = nullptr;
    SoundPlaybackID    m_bgmPlaybackID                = MISSING_SOUND_ID;
};

//-----------------------------------------------------------------------------------------------
// Template pool helper implementations
//-----------------------------------------------------------------------------------------------
template <typename T, int N>
void Game::ForEachInPool(T* (&pool)[N], auto&& fn)
{
    for (int i = 0; i < N; ++i)
    {
        if (!pool[i]) continue;

        fn(pool[i]);
    }
}

template <typename T, int N>
void Game::ForEachInPool(T* const (&pool)[N], auto&& fn) const
{
    for (int i = 0; i < N; ++i)
    {
        if (!pool[i]) continue;

        fn(pool[i]);
    }
}

template <typename T, int N>
T** Game::FindEmptySlot(T* (&pool)[N])
{
    for (int i = 0; i < N; ++i)
    {
        if (!pool[i]) return &pool[i];
    }

    return nullptr;
}

template <typename T, int N>
void Game::DeletePool(T* (&pool)[N])
{
    for (int i = 0; i < N; ++i)
    {
        if (!pool[i]) continue;

        delete pool[i];
        pool[i] = nullptr;
    }
}

template <typename T, int N>
void Game::DeleteGarbageInPool(T* (&pool)[N])
{
    for (int i = 0; i < N; ++i)
    {
        if (pool[i] && pool[i]->IsGarbage())
        {
            delete pool[i];
            pool[i] = nullptr;
        }
    }
}

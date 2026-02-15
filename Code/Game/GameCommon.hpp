//-----------------------------------------------------------------------------------------------
// GameCommon.hpp
//

//-----------------------------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------------------------
struct Rgba8;
struct Vec2;
class App;
class Game;

// one-time declaration
extern App*  g_app;
extern Game* g_game;

//-----------------------------------------------------------------------------------------------
// initial settings
//
constexpr float WORLD_SIZE_X  = 200.f;
constexpr float WORLD_SIZE_Y  = 100.f;
constexpr float SCREEN_SIZE_X = 1600.f;
constexpr float SCREEN_SIZE_Y = 800.f;

constexpr float WORLD_CENTER_X = WORLD_SIZE_X / 2.f;
constexpr float WORLD_CENTER_Y = WORLD_SIZE_Y / 2.f;

//-----------------------------------------------------------------------------------------------
// PlayerShip-related
//
constexpr int      PLAYER_SHIP_VERTS_NUM       = 15;
constexpr float    PLAYER_SHIP_ACCELERATION    = 30.f;
constexpr float    PLAYER_SHIP_TURN_SPEED      = 300.f;
constexpr float    PLAYER_SHIP_PHYSICS_RADIUS  = 1.75f;
constexpr float    PLAYER_SHIP_COSMETIC_RADIUS = 2.25f;
constexpr int      MAX_PLAYER_SHIP_HEALTH      = 10;
extern Rgba8 const PLAYER_SHIP_COLOR;

//-----------------------------------------------------------------------------------------------
// Bullet-related
//
constexpr int      MAX_BULLETS_NUM         = 100;
constexpr int      BULLET_VERTS_NUM        = 6;
constexpr float    BULLET_LIFETIME_SECONDS = 2.0f;
constexpr float    BULLET_SPEED            = 50.f;
constexpr float    BULLET_PHYSICS_RADIUS   = 0.5f;
constexpr float    BULLET_COSMETIC_RADIUS  = 2.0f;
extern Rgba8 const BULLET_YELLOW_OPAQUE;
extern Rgba8 const BULLET_RED_OPAQUE;
extern Rgba8 const BULLET_RED_TRANSPARENT;

//-----------------------------------------------------------------------------------------------
// Asteroid-related
//
constexpr int      STARTING_ASTEROIDS_NUM   = 6;
constexpr int      MAX_ASTEROIDS_NUM        = 30;
constexpr int      ASTEROID_TRIS_NUM        = 16;
constexpr int      ASTEROID_VERTS_NUM       = 3 * ASTEROID_TRIS_NUM;
constexpr float    ASTEROID_SPEED           = 10.f;
constexpr float    ASTEROID_PHYSICS_RADIUS  = 1.6f;
constexpr float    ASTEROID_COSMETIC_RADIUS = 2.0f;
extern Rgba8 const ASTEROID_COLOR;

//-----------------------------------------------------------------------------------------------
// Beetle-related
//
constexpr int      MAX_BEETLE_NUM         = 20;
constexpr int      BEETLE_VERTS_NUM       = 6;
constexpr float    BEETLE_PHYSICS_RADIUS  = 1.5f;
constexpr float    BEETLE_COSMETIC_RADIUS = 2.25f;
extern Rgba8 const BEETLE_COLOR;

//-----------------------------------------------------------------------------------------------
// Wasp-related
//
constexpr int      MAX_WASP_NUM         = 20;
constexpr int      WASP_VERTS_NUM       = 12;
constexpr float    WASP_ACCELERATION    = 10.f;
constexpr float    WASP_PHYSICS_RADIUS  = 1.73f;
constexpr float    WASP_COSMETIC_RADIUS = 2.f;
extern Rgba8 const WASP_COLOR;

//-----------------------------------------------------------------------------------------------
// Debris-related
//
// #TODO: fix
constexpr int   MAX_DEBRIS_NUM            = 200000;
constexpr int   DEBRIS_TRI_NUM            = 8;
constexpr int   DEBRIS_VERTS_NUM          = 3 * DEBRIS_TRI_NUM;
constexpr float ENTITY_HIT_DEBRIS_RADIUS  = 0.1f;
constexpr float ENTITY_DEAD_DEBRIS_RADIUS = 0.3f;

//-----------------------------------------------------------------------------------------------
// Box-related
//
constexpr int   MAX_BOX_NUM     = 10000;
constexpr int   BOX_TRI_NUM     = 2;
constexpr int   BOX_VERTS_NUM   = 3 * BOX_TRI_NUM;
constexpr float BOX_SIDE_LENGTH = 4.f;

//-----------------------------------------------------------------------------------------------
// DebugRender-related
//
void DebugDrawRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);
void DebugDrawLine(Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color);
void DebugDrawGlowCircle(Vec2 const& center, float radius, Rgba8 const& color, float glowIntensity);
void DebugDrawGlowBox(Vec2 const& center, Vec2 const& dimensions, Rgba8 const& color, float glowIntensity);
void DebugDrawBoxRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);

extern Rgba8 const DEBUG_RENDER_GREY;
extern Rgba8 const DEBUG_RENDER_RED;
extern Rgba8 const DEBUG_RENDER_GREEN;
extern Rgba8 const DEBUG_RENDER_MAGENTA;
extern Rgba8 const DEBUG_RENDER_CYAN;
extern Rgba8 const DEBUG_RENDER_YELLOW;

extern char const* const IN_GAME_BGM;

//----------------------------------------------------------------------------------------------------
template <typename T>
void GAME_SAFE_RELEASE(T*& pointer)
{
    if (pointer != nullptr)
    {
        delete pointer;
        pointer = nullptr;
    }
}

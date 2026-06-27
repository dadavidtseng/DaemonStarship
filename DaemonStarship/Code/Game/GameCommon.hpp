//----------------------------------------------------------------------------------------------------
// GameCommon.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once

//-forward-declaration--------------------------------------------------------------------------------
struct Rgba8;
struct Vec2;
struct Vertex_PCU;
class App;
class Game;

//----------------------------------------------------------------------------------------------------
/// one-time declaration
//----------------------------------------------------------------------------------------------------
extern App*  g_app;
extern Game* g_game;

//----------------------------------------------------------------------------------------------------
/// initial settings
//----------------------------------------------------------------------------------------------------
float constexpr WORLD_SIZE_X  = 200.f;
float constexpr WORLD_SIZE_Y  = 100.f;
float constexpr SCREEN_SIZE_X = 1600.f;
float constexpr SCREEN_SIZE_Y = 800.f;

float constexpr WORLD_CENTER_X = WORLD_SIZE_X * 0.5f;
float constexpr WORLD_CENTER_Y = WORLD_SIZE_Y * 0.5f;

//----------------------------------------------------------------------------------------------------
/// PlayerShip-related
//----------------------------------------------------------------------------------------------------
int constexpr      PLAYER_SHIP_VERTS_NUM       = 15;
float constexpr    PLAYER_SHIP_ACCELERATION    = 30.f;
float constexpr    PLAYER_SHIP_DECELERATION    = 20.f;
float constexpr    PLAYER_SHIP_TURN_SPEED      = 300.f;
float constexpr    PLAYER_SHIP_PHYSICS_RADIUS  = 1.75f;
float constexpr    PLAYER_SHIP_COSMETIC_RADIUS = 2.25f;
int constexpr      MAX_PLAYER_SHIP_HEALTH      = 10;
extern Rgba8 const PLAYER_SHIP_COLOR;

//----------------------------------------------------------------------------------------------------
/// Bullet-related
//----------------------------------------------------------------------------------------------------
int constexpr      MAX_BULLETS_NUM         = 100;
int constexpr      BULLET_VERTS_NUM        = 6;
float constexpr    BULLET_LIFETIME_SECONDS = 2.f;
float constexpr    BULLET_SPEED            = 50.f;
float constexpr    BULLET_PHYSICS_RADIUS   = 0.5f;
float constexpr    BULLET_COSMETIC_RADIUS  = 2.f;
extern Rgba8 const BULLET_YELLOW_OPAQUE;
extern Rgba8 const BULLET_RED_OPAQUE;
extern Rgba8 const BULLET_RED_TRANSPARENT;

//----------------------------------------------------------------------------------------------------
// Asteroid-related
//----------------------------------------------------------------------------------------------------
int constexpr      STARTING_ASTEROIDS_NUM   = 6;
int constexpr      MAX_ASTEROIDS_NUM        = 30;
int constexpr      ASTEROID_TRIS_NUM        = 16;
int constexpr      ASTEROID_VERTS_NUM       = 3 * ASTEROID_TRIS_NUM;
float constexpr    ASTEROID_SPEED           = 10.f;
float constexpr    ASTEROID_PHYSICS_RADIUS  = 1.6f;
float constexpr    ASTEROID_COSMETIC_RADIUS = 2.f;
extern Rgba8 const ASTEROID_COLOR;

//----------------------------------------------------------------------------------------------------
/// Beetle-related
//----------------------------------------------------------------------------------------------------
int constexpr      MAX_BEETLE_NUM         = 20;
int constexpr      BEETLE_VERTS_NUM       = 6;
float constexpr    BEETLE_PHYSICS_RADIUS  = 1.5f;
float constexpr    BEETLE_COSMETIC_RADIUS = 2.25f;
extern Rgba8 const BEETLE_COLOR;

//----------------------------------------------------------------------------------------------------
/// Wasp-related
//----------------------------------------------------------------------------------------------------
int constexpr      MAX_WASP_NUM         = 20;
int constexpr      WASP_VERTS_NUM       = 12;
float constexpr    WASP_ACCELERATION    = 10.f;
float constexpr    WASP_PHYSICS_RADIUS  = 1.73f;
float constexpr    WASP_COSMETIC_RADIUS = 2.f;
extern Rgba8 const WASP_COLOR;

//----------------------------------------------------------------------------------------------------
// Debris-related
//----------------------------------------------------------------------------------------------------
// #TODO: fix
int constexpr   MAX_DEBRIS_NUM            = 200000;
int constexpr   DEBRIS_TRI_NUM            = 8;
int constexpr   DEBRIS_VERTS_NUM          = 3 * DEBRIS_TRI_NUM;
float constexpr ENTITY_HIT_DEBRIS_RADIUS  = 0.1f;
float constexpr ENTITY_DEAD_DEBRIS_RADIUS = 0.3f;

//----------------------------------------------------------------------------------------------------
/// Box-related
//----------------------------------------------------------------------------------------------------
int constexpr   MAX_BOX_NUM     = 10000;
int constexpr   BOX_TRI_NUM     = 2;
int constexpr   BOX_VERTS_NUM   = 3 * BOX_TRI_NUM;
float constexpr BOX_SIDE_LENGTH = 4.f;

//----------------------------------------------------------------------------------------------------
// DebugRender-related
//----------------------------------------------------------------------------------------------------
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
// Shared vertex data
//----------------------------------------------------------------------------------------------------
void InitializePlayerShipVerts(Vertex_PCU verts[]);

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

# Game Module - DaemonStarship

[Root Directory](../../CLAUDE.md) > [Code](../) > **Game**

## Module Responsibilities

The Game module contains the complete implementation of the DaemonStarship space shooter game. It manages all gameplay mechanics, entity systems, user interface, scoring, and integration with the underlying game engine. This is the primary module containing all game-specific logic and rendering.

## Entry and Startup

### Main Entry Point
- **File**: `Main_Windows.cpp`  
- **Function**: `WinMain()` - Windows application entry point
- **Flow**: Creates App instance → Startup → RunMainLoop → Shutdown → Cleanup

### Application Architecture  
- **App Class** (`App.hpp/cpp`): Main application controller managing frame lifecycle
- **Game Class** (`Game.hpp/cpp`): Core game state and logic management
- **Initialization Sequence**: Window creation → Engine systems → Game initialization → Main loop

## External Interfaces

### Engine Dependencies
- **Custom Engine**: Located at `../../../Engine/Code/Engine/`
- **Core Systems**: Renderer, AudioSystem, EventSystem, Math utilities
- **Input Handling**: Keyboard and controller input processing
- **Graphics Pipeline**: DirectX 11 integration with custom rendering abstractions

### File I/O Interfaces
- **Scoreboard Persistence**: `Run/Data/Score/Scoreboard.txt` - Player score tracking
- **Asset Loading**: Textures, audio files, fonts from `Run/Data/` directories
- **Shader Compilation**: HLSL shader loading from `Run/Data/Shaders/Default.hlsl`

### User Input Interfaces
- **Keyboard Controls**: WASD movement, spacebar shooting, ESC menu navigation
- **Controller Support**: Xbox controller input for movement and actions
- **UI Navigation**: Button selection system for menus and name input

## Key Dependencies and Configuration

### Build Configuration
- **Visual Studio Project**: `Game.vcxproj` - Configured for x86/x64 Debug/Release builds
- **C++ Standard**: C++20 with Unicode character set
- **Platform Target**: Windows 10/11 (Win32/x64)
- **Output Directory**: `Temporary/DaemonStarship_{Platform}_{Configuration}/`

### Engine Integration
- **Include Paths**: `$(SolutionDir)Code/` and `$(SolutionDir)../Engine/Code/`
- **Project Dependencies**: Engine.vcxproj with automatic build ordering
- **Post-Build**: Executable copying to `$(SolutionDir)Run` directory

### External Libraries
- **FMOD**: Audio system integration (fmod.dll, fmod64.dll in Run directory)
- **DirectX 11**: Graphics API for rendering pipeline
- **Windows API**: Platform-specific window management and input

## Data Models

### Entity System Architecture
```cpp
// Base Entity class with virtual interface
class Entity {
    Vec2 m_position, m_velocity;
    float m_orientationDegrees, m_angularVelocity;
    float m_physicsRadius, m_cosmeticRadius;
    bool m_isDead, m_isGarbage;
    int m_health;
};
```

### Core Game Entities
- **PlayerShip**: Player-controlled ship with score tracking and health system
- **Bullet**: Projectile entities with lifetime management and collision detection  
- **Asteroid**: Environmental hazard entities with procedural movement
- **Beetle**: Enemy entity type with basic AI behavior
- **Wasp**: Advanced enemy with acceleration-based movement
- **Debris**: Particle effect entities for visual feedback
- **Box**: Special collectible/interactive entities

### Game State Management
```cpp
// Fixed-size entity arrays for predictable memory usage
PlayerShip* m_playerShip;
Bullet* m_bullets[MAX_BULLETS_NUM];     // 100 bullets max
Asteroid* m_asteroids[MAX_ASTEROIDS_NUM]; // 30 asteroids max  
Beetle* m_beetle[MAX_BEETLE_NUM];       // 20 beetles max
Wasp* m_wasp[MAX_WASP_NUM];            // 20 wasps max
Debris* m_debris[MAX_DEBRIS_NUM];       // 200,000 debris max
Box* m_boxes[MAX_BOX_NUM];             // 10,000 boxes max
```

### UI and Scoring System
```cpp
struct PlayerScore {
    std::string name;
    int score;
    int rank;
};

struct Button {
    float width, height;
    Vec2 center, textPosition;
    Rgba8 color, textColor;
    std::string text;
    bool isSelected;
};
```

## Testing and Quality

### Debug Systems
- **Debug Rendering**: `DebugRender()` methods on all entities for collision visualization
- **Debug Console Integration**: Command system for runtime parameter adjustment
- **Hot Reload Support**: Dynamic code reloading during development sessions

### Quality Assurance Features
- **Memory Management**: RAII-style cleanup with `GAME_SAFE_RELEASE` template
- **Collision Detection**: Separate physics and cosmetic radius systems
- **Performance Monitoring**: Fixed-array entity pools to prevent allocation spikes
- **Error Handling**: Comprehensive bounds checking and null pointer validation

### Build Validation
- **Multiple Configurations**: Debug/Release builds with different optimization levels
- **Platform Coverage**: x86 and x64 architecture support
- **Warning Level**: Level 4 warnings enabled with SDL security checks
- **Static Analysis**: ConformanceMode enabled for standards compliance

## FAQ

### Q: How does the entity system handle memory management?
A: The game uses fixed-size arrays for all entity types, avoiding dynamic allocation during gameplay. Entities are marked as "garbage" and cleaned up at frame boundaries using the `DeleteGarbageEntities()` system.

### Q: What is the relationship between physics and cosmetic radius?
A: Physics radius is used for collision detection (conservative, smaller), while cosmetic radius encompasses the visual representation (liberal, larger). This allows for more forgiving gameplay while maintaining visual accuracy.

### Q: How does the wave/level progression system work?
A: The game uses `m_currentWave` to track progress and `SpawnEnemiesForCurrentWave()` to dynamically spawn appropriate enemy counts. The system checks `AreAllEnemiesDead()` to trigger wave advancement.

### Q: Can the game be extended with new entity types?
A: Yes, new entities can be added by deriving from the Entity base class and implementing the required virtual methods (`Update()`, `Render()`, `DebugRender()`, `InitializeLocalVerts()`). Add corresponding arrays to the Game class for management.

## Related File List

### Core Architecture
- `Main_Windows.cpp` - Application entry point
- `App.hpp/cpp` - Application lifecycle management
- `Game.hpp/cpp` - Core game logic and state
- `GameCommon.hpp/cpp` - Shared constants and utilities
- `Entity.hpp/cpp` - Base entity class definition

### Entity Implementations  
- `PlayerShip.hpp/cpp` - Player character implementation
- `Bullet.hpp/cpp` - Projectile entity system
- `Asteroid.hpp/cpp` - Environmental hazard entities
- `Beetle.hpp/cpp` - Basic enemy AI entity
- `Wasp.hpp/cpp` - Advanced enemy with acceleration
- `Debris.hpp/cpp` - Particle effect system
- `Box.hpp/cpp` - Interactive/collectible entities

### Systems and UI
- `UIHandler.hpp/cpp` - User interface management
- `ScoreBoardHandler.hpp/cpp` - Score persistence system
- `LevelData.hpp/cpp` - Level/wave configuration data
- `EngineBuildPreferences.hpp` - Engine configuration settings

### Build and Configuration
- `Game.vcxproj` - Visual Studio project configuration  
- `Game.vcxproj.filters` - Project file organization

## Changelog

### 2025-09-25 17:43:22
- Initial module documentation created
- Entity system architecture documented
- Build configuration and dependencies catalogued
- Core gameplay systems and data models identified
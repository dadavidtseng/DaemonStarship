<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![DirectX](https://img.shields.io/badge/DirectX-11-107C10?style=for-the-badge&logo=xbox&logoColor=white)
![FMOD](https://img.shields.io/badge/FMOD-000000?style=for-the-badge&logo=fmod&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
[![Apache 2.0 License][license-shield]][license-url]

<!-- PROJECT TITLE -->
<div align="center">
  <h1>Daemon Starship</h1>
  <p>A classic 2D space shooter built with a custom C++20 engine and DirectX 11</p>
</div>

<!-- TODO: Replace with actual gameplay GIF or screenshot -->
<!-- <div align="center">
  <img src="docs/images/demo.gif" alt="Daemon Starship Gameplay" width="720">
</div> -->

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [How to Install](#how-to-install)
- [How to Use](#how-to-use)
- [Project Structure](#project-structure)
- [License](#license)
- [Contact](#contact)

## Overview

Daemon Starship is a 2D space shooter where the player defends against waves of enemies — asteroids, beetles, and wasps — in an infinite wrapping space. The game features a wave progression system across 5 levels of increasing difficulty, persistent high-score tracking, and a dual-radius collision detection system.

Built as a course project at SMU Guildhall, the game runs on a custom [Daemon Engine](https://github.com/dadavidtseng/Engine) providing DirectX 11 rendering, FMOD 3D audio, multithreaded job processing, and a developer console for runtime debugging.

## Features

- **Wave-based combat** — 5 levels with progressive enemy counts (beetles, wasps, asteroids)
- **Fixed-size entity pools** — Zero dynamic allocation during gameplay for consistent frame times
- **Dual-radius collision** — Conservative physics radius for gameplay, liberal cosmetic radius for visuals
- **Persistent scoreboard** — Top 100 high scores saved to disk
- **FMOD audio** — Background music and spatial sound effects

## How to Install

### Prerequisites

- Visual Studio 2022 (or 2019) with C++ desktop development workload
- Windows 10/11 (x64)
- DirectX 11 compatible GPU
- [Daemon Engine](https://github.com/dadavidtseng/Engine) cloned as a sibling directory

### Build

```bash
# Clone both repos side by side
git clone https://github.com/dadavidtseng/Engine.git
git clone https://github.com/dadavidtseng/DaemonStarship.git

# Directory layout should be:
# ├── Engine/
# └── DaemonStarship/
```

1. Open `DaemonStarship.sln` in Visual Studio
2. Set configuration to `Debug | x64`
3. Build the solution (the Engine project is referenced automatically)
4. The executable is deployed to `Run/` via post-build event

## How to Use

### Controls

| Action | Keyboard | Controller |
|--------|----------|------------|
| Move | W / A / S / D | Left Stick |
| Shoot | Spacebar | A Button |
| Pause / Menu | ESC | Start |

### Game Flow

1. **Attract Mode** — Title screen, press to start
2. **Name Input** — Enter your name for the scoreboard
3. **Gameplay** — Survive 5 waves of increasing difficulty
4. **High Scores** — View the top 100 leaderboard

### Running

Launch `Run/DaemonStarship_Debug_x64.exe` from the `Run/` directory (working directory must be `Run/` for asset loading).

## Project Structure

```
DaemonStarship/
├── Code/Game/                # Game source (15 .cpp + 15 .hpp)
│   ├── Main_Windows.cpp      # WinMain entry point
│   ├── App.cpp/hpp           # Application lifecycle
│   ├── Game.cpp/hpp          # Core game logic, wave system
│   ├── Entity.cpp/hpp        # Base entity class
│   ├── PlayerShip.cpp/hpp    # Player character (10 HP)
│   ├── Bullet.cpp/hpp        # Projectiles (max 100)
│   ├── Asteroid.cpp/hpp      # Environmental hazards (max 30)
│   ├── Beetle.cpp/hpp        # Basic enemy AI (max 20)
│   ├── Wasp.cpp/hpp          # Advanced enemy (max 20)
│   ├── Debris.cpp/hpp        # Particle effects (max 200,000)
│   ├── UIHandler.cpp/hpp     # UI management
│   ├── ScoreBoardHandler.cpp/hpp  # Score persistence
│   └── LevelData.cpp/hpp     # Wave configuration
├── Run/                      # Runtime directory
│   ├── Data/Audio/           # BGM and sound effects (MP3)
│   ├── Data/Config/          # JSON engine configuration
│   ├── Data/Fonts/           # Bitmap fonts (PNG)
│   ├── Data/Score/           # Scoreboard persistence
│   └── Data/Shaders/         # HLSL shaders
├── Docs/                     # Documentation
└── DaemonStarship.sln        # Visual Studio solution
```

## License

Copyright 2025 Yu-Wei Tseng

Licensed under the [Apache License, Version 2.0](../LICENSE).

## Contact

**Yu-Wei Tseng**
- Portfolio: [dadavidtseng.info](https://dadavidtseng.info)
- GitHub: [@dadavidtseng](https://github.com/dadavidtseng)
- LinkedIn: [dadavidtseng](https://www.linkedin.com/in/dadavidtseng/)
- Email: dadavidtseng@gmail.com

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- REFERENCE-STYLE LINKS -->
[license-shield]: https://img.shields.io/github/license/dadavidtseng/DaemonStarship.svg?style=for-the-badge
[license-url]: https://github.com/dadavidtseng/DaemonStarship/blob/main/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/dadavidtseng

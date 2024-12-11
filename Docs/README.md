# SD1 Assignment 4: Starship Gold

## C34 Yu-Wei Tseng

This repository contains the implementation of the Starship Gold, including simple physics, behavior, rendering, and management of game entities.

## How to Use

### Keyboard

- **F1:** Toggles the debug render for entities.
- **F4:** Switch to the next map in the game. If it is the last map, the player enters the game win scene and returns to
  attract mode.
- **F8:** Does a hard restart of the game, deleting and re-newing the Game instance.
- **ESF:** To drive forward & turn toward direction.
- **SPACE:** To enter key-in user's name mode, or spawn bullet in-game.
- **ENTER:** To start new game after entering user's name
- **ESC:** To leave game, and again to quit.
- **T:** Slows game simulation time to 1/10 the normal rate.
- **O:** Step single update and pause.
- **N:** Respawn playerShip after it's dead.

### Xbox controller

- **XBOX_BUTTON_DPAD_DOWN:** Switch to the next map in the game. If it is the last map, the player enters the game win scene and returns to
  attract mode.
- **XBOX_LEFT_TRIGGER:** To drive forward & turn toward direction.
- **XBOX_BUTTON_START:** Respawn playerShip after it's dead.
- **XBOX_BUTTON_BACK:** To leave game, and again to quit.
- **XBOX_BUTTON_A:** To spawn bullet in-game.

## Known Issues

- The in-game bgm doesn't restart after re-entering the game.
- Shaking camera is using KEYCODE_Y for placeholder.
- Record the current score is using KEYCODE_U for placeholder.
- Attract mode's scoreboard button does not work.

## Deep Learning

Introducing a new entity, the box, has definitely enhanced the game's playability.
Adding a scoreboard has made the game feel more like a complete experience.
My ultimate goal is to implement an online scoreboard feature, allowing the entire cohort to play and compete.
The codebase requires significant maintenance, and I plan to address this to make the project portfolio-ready.
Lastly, I am also considering giving the entities some background stories, similar to what we did in our other project, Libra.





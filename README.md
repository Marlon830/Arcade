# Arcade

> Made by **Marlon PEGAHI**, **Alexandre BRET** and **Allan LEHERPEUX**

A modular arcade game platform written in C++20. Games and graphics libraries are independent plugins loaded at runtime, swap between them on the fly without restarting.

---

## Features

- Hot-swap between multiple graphics backends (SFML, SDL2, Raylib, Ncurses) during gameplay
- Hot-swap between games (Snake, Centipede) without restarting
- Score tracking across sessions
- Fully extensible: add new games or display libraries by implementing a simple interface

---

## Requirements

- `g++` with C++20 support
- Libraries depending on which display backends you want to build:
  - **SFML**: `libsfml-dev`
  - **SDL2**: `libsdl2-dev`
  - **Raylib**: `libraylib-dev`
  - **Ncurses**: `libncurses-dev`

---

## Build

```sh
# Build everything (core binary + all plugins)
make

# Or build parts individually
make core         # arcade binary only
make games        # game plugins only
make graphicals   # display library plugins only

# Rebuild from scratch
make re
```

---

## Usage

```sh
./arcade ./lib/library/arcade_sfml.so
```

Pass any compiled display library as the argument. The platform starts with the game menu.

---

## Controls

| Key | Action |
|-----|--------|
| `Z` / `Q` / `S` / `D` or Arrows | Move |
| `Enter` | Select |
| `I` | Restart current game |
| `K` | Return to menu |
| `O` / `P` | Switch to next / previous game |
| `L` / `M` | Switch to next / previous graphics library |

---

## Architecture

The platform uses a **plugin architecture**: games and display backends are compiled as shared objects (`.so`) and loaded via `dlopen` at runtime.

```
arcade (core)
├── loads → IDisplayModule plugin  (SFML / SDL2 / Raylib / Ncurses)
└── loads → IGameModule plugin     (Snake / Centipede / Menu)
```

Each frame:
1. Display module polls input events
2. Core forwards events to the game (`onEvent`)
3. Game updates its state (`onUpdate(delta_ms)`)
4. Game provides a `GameDisplay` (tile map + sprites)
5. Display module renders it

When the player switches game or library, the core unloads the old plugin, loads the new one, and reloads all assets.

---

## Adding a New Game

1. Create `lib/games/YourGame/` with `YourGame.hpp`, `YourGame.cpp`, and a `Makefile`
2. Inherit from `Arcade::IGameModule` and implement: `initGame()`, `onUpdate()`, `onEvent()`, `getDisplay()`, `getAssets()`
3. Export `getInstance()` and `deleteInstance()` factory functions
4. Add your target to `lib/games/Makefile`

## Adding a New Display Library

1. Create `lib/library/YourLib/` with `YourLib.hpp`, `YourLib.cpp`, and a `Makefile`
2. Inherit from `Arcade::IDisplayModule` and implement: `display()`, `loadTileSet()`, `loadMusic()`, `loadSound()`, `playMusic()`, `playSound()`, `pollEvent()`
3. Export `getInstance()` and `deleteInstance()` factory functions
4. Add your target to `lib/library/Makefile`

See `doc/README.md` for detailed implementation guidelines and architecture diagrams.

---

## Project Structure

```
arcade/
├── src/
│   ├── Arcade/         # Core orchestrator
│   ├── Interface/      # IGameModule, IDisplayModule, IArcade
│   ├── Utils/          # Event, GameDisplay, TileMap, Assets, Vector, Rect
│   ├── DLLoader/       # dlopen/dlsym wrapper
│   ├── Library/        # RAII plugin instance manager
│   ├── DirectoryTool/  # Plugin discovery (scans lib/)
│   └── TileMap/        # 2D tile grid
├── lib/
│   ├── games/          # Snake, Centipede, Menu
│   └── library/        # SFML, SDL2, Raylib, Ncurses
├── assets/             # Sprites, fonts, sounds
└── doc/                # Architecture diagrams and implementation guide
```

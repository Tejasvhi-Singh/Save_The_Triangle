# Triangle Game

A simple 2D game where you control a triangle that moves forward while avoiding randomly spawning obstacles.

## Game Concept
- **Window**: Black background
- **Player**: White triangle fixed at bottom center, moving forward (upward)
- **Obstacles**: Red squares spawning randomly from the top, moving downwards
- **Camera**: Top-down effect simulated by obstacles coming towards the player
- **Controls**: Currently automatic forward movement (left/right movement can be added later)

## Controls
- **ESC**: Quit game
- **R**: Restart game after collision

## Building the Game

### Prerequisites
- CMake (version 3.10 or higher)
- SFML 2.5 or higher
- C++17 compatible compiler

### Build Instructions

1. **Install SFML** (if not already installed):
   ```bash
   # On macOS with Homebrew:
   brew install sfml
   
   # On Ubuntu/Debian:
   sudo apt-get install libsfml-dev
   
   # On Windows with vcpkg:
   vcpkg install sfml
   ```

2. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the game**:
   ```bash
   ./TriangleGame
   ```

## Game Features
- Smooth 60 FPS gameplay
- Random obstacle spawning
- Collision detection
- Game over and restart functionality
- Clean, modern graphics with outlines

## Future Enhancements
- Left/right movement controls
- Score system
- Power-ups
- Different obstacle types
- Sound effects
- Particle effects 
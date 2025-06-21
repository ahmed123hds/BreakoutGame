# Breakout Game

A classic Breakout game implementation using OpenGL and C++.

## Project Structure

```
BreakoutGame/
├── src/
│   ├── Core/           # Core game logic
│   │   └── Game.cpp    # Main game class
│   ├── Entities/       # Game objects
│   │   ├── GameObject.cpp  # Base game object
│   │   ├── Paddle.cpp      # Player paddle
│   │   ├── Ball.cpp        # Game ball
│   │   └── Brick.cpp       # Breakable bricks
│   ├── Rendering/      # OpenGL rendering system
│   │   ├── Renderer.cpp    # Main renderer 
│   │   └── Shader.cpp      # Shader management
│   ├── Input/          # Input handling
│   │   └── InputManager.cpp # Keyboard/mouse input
│   ├── Physics/        # Physics engine 
│   │   └── PhysicsEngine.cpp # Collision detection and response
│   └── main.cpp        # Application entry point
├── include/            # Header files (mirrors src structure)
├── resources/          # Game assets
│   ├── shaders/        # GLSL shader files
│   └── textures/       # Texture files
└── CMakeLists.txt      # Build configuration
```

## Key Classes for Physics Implementation

### Ball Class (`include/Entities/Ball.h`)
- `GetVelocity()` / `SetVelocity()` - Ball movement vector
- `GetPosition()` / `SetPosition()` - Ball position
- `GetRadius()` - Ball collision radius
- `Update(deltaTime)` - Called each frame for position updates

### Paddle Class (`include/Entities/Paddle.h`)
- `GetPosition()` / `GetSize()` - Paddle bounds for collision
- `MoveLeft()` / `MoveRight()` - Player input handling
- `ConstrainToScreen()` - Keep paddle within bounds

### Brick Class (`include/Entities/Brick.h`)
- `GetPosition()` / `GetSize()` - Brick bounds for collision
- `Hit()` - Called when ball collides with brick
- `IsActive()` - Check if brick should be rendered/collide

### PhysicsEngine Class (`include/Physics/PhysicsEngine.h`)
- `CheckCollision()` - Detect ball-paddle/brick collisions
- `HandleCollision()` - Respond to collisions (change velocity, destroy bricks)
- `Update(deltaTime)` - Main physics simulation step

## Build Instructions

### Prerequisites
- CMake 3.16+
- OpenGL
- GLFW3
- GLEW
- GLM (OpenGL Mathematics)

### Building
```bash
mkdir build
cd build
cmake ..
make
```

### Running
```bash
./BreakoutGame
```

## Controls
- **A/Left Arrow**: Move paddle left
- **D/Right Arrow**: Move paddle right  
- **Space**: Pause/unpause game
- **R**: Restart game (when game over or win) - Completely fresh game state (no persistent data between restarts)
- **Escape**: Exit game

## Game Features
- **Scoring System**: Earn points by destroying bricks with prominent score display
  - Normal bricks: 10 points
  - Hard bricks: 20 points (take 2 hits, 5 points per hit)
  - Unbreakable bricks: 50 points (can't be destroyed)
- **Progressive Difficulty**: Ball speed increases gradually (2% per second, max 50% increase)
- **Proper Game Over**: Ball falls through floor to end game (no floor bouncing)
- **Multiple Brick Types**: 
  - Red (Normal): 1 hit to destroy
  - Blue (Hard): 2 hits to destroy  
  - Gray (Unbreakable): Indestructible corner pieces
- **Enhanced Visuals**: 
  - High-contrast score display with background
  - 7-segment style digital score numbers
  - Clear game over/win messages with restart instructions

## Physics Implementation Notes

The physics system is set up with placeholder methods for you to implement:

1. **Collision Detection**: Implement circle-rectangle intersection in `PhysicsEngine::CheckCollision()`
2. **Collision Response**: Handle velocity changes in `PhysicsEngine::Handle*Collision()` methods
3. **Ball Movement**: Update ball position based on velocity in `Ball::Update()`
4. **Boundary Checking**: Handle screen edge collisions

The rendering system is complete and will display all active game objects. Focus your implementation on the physics calculations and collision responses.

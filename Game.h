#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.h"
#include "Obstacle.h"
#include "Button.h"

// Game states
enum class GameState {
    Menu,
    Playing,
    GameOver
};

// Particle effect for explosions
struct ExplosionParticle {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float life;
    float maxLife;
    
    ExplosionParticle(float x, float y, float vx, float vy, float lifetime);
    void update(float deltaTime);
    bool isDead() const;
};

// Trail particle for player
struct TrailParticle {
    sf::CircleShape shape;
    float life;
    float maxLife;
    
    TrailParticle(float x, float y, float lifetime);
    void update(float deltaTime);
    bool isDead() const;
};

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Clock spawnClock;  // Separate clock for obstacle spawning
    sf::Clock speedClock;  // Clock for speed increments
    sf::Time lastObstacleSpawn;
    sf::Time obstacleSpawnInterval;
    
    // Game state
    GameState currentState;
    bool isRunning;
    
    Player player;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<sf::CircleShape> backgroundParticles;  // Background moving particles
    std::vector<ExplosionParticle> explosionParticles; // Explosion effects
    std::vector<TrailParticle> trailParticles;         // Player trail
    
    // UI elements
    sf::Font font;
    sf::Text scoreText;
    sf::Text speedText;
    sf::Text livesText;
    sf::Text titleText;
    sf::Text gameOverText;
    sf::Text finalScoreText;
    
    // Menu buttons
    std::vector<std::unique_ptr<Button>> menuButtons;
    std::vector<std::unique_ptr<Button>> gameOverButtons;
    
    // Mouse input
    sf::Vector2f mousePos;
    bool mousePressed;
    
    // Game state variables
    float gameSpeed;
    float speedIncrement;      // How much to increase speed by
    float maxSpeed;            // Maximum speed limit
    sf::Time speedIncrementInterval; // How often to increase speed
    
    // Visual effects
    int score;
    int lives;
    float screenShakeTime;
    float screenShakeIntensity;
    sf::Vector2f screenShakeOffset;
    
    // Collision polish
    float invulnerabilityTime;
    float invulnerabilityDuration;
    bool isInvulnerable;
    
    // Menu and UI methods
    void setupMenu();
    void setupGameOverScreen();
    void processMenuEvents();
    void processGameOverEvents();
    void updateMenu();
    void updateGameOver();
    void renderMenu();
    void renderGameOver();
    void startGame();
    void returnToMenu();
    void quitGame();
    
    // Game methods
    void processEvents();
    void update(float deltaTime);
    void render();
    void spawnObstacle();
    void checkCollisions();
    void removeOffscreenObstacles();
    void updateSpeed();
    void updateBackgroundParticles(float deltaTime);
    void spawnBackgroundParticle();
    void updateExplosionParticles(float deltaTime);
    void updateTrailParticles(float deltaTime);
    void createExplosion(float x, float y);
    void addTrailParticle(float x, float y);
    void updateScreenShake(float deltaTime);
    void updateUI();
    void checkObstacleCollisions();
    void updateInvulnerability(float deltaTime);
    
public:
    Game();
    void run();
    void reset();
    void setState(GameState state);
}; 
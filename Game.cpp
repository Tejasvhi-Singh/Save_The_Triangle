#include "Game.h"
#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>

// ExplosionParticle implementation
ExplosionParticle::ExplosionParticle(float x, float y, float vx, float vy, float lifetime)
    : velocity(vx, vy), life(lifetime), maxLife(lifetime) {
    shape.setRadius(2.0f);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Yellow);
}

void ExplosionParticle::update(float deltaTime) {
    life -= deltaTime;
    shape.move(velocity * deltaTime);
    
    // Fade out
    float alpha = (life / maxLife) * 255;
    shape.setFillColor(sf::Color(255, 255, 0, static_cast<sf::Uint8>(alpha)));
}

bool ExplosionParticle::isDead() const {
    return life <= 0;
}

// TrailParticle implementation
TrailParticle::TrailParticle(float x, float y, float lifetime)
    : life(lifetime), maxLife(lifetime) {
    shape.setRadius(3.0f);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Cyan);
}

void TrailParticle::update(float deltaTime) {
    life -= deltaTime;
    
    // Fade out
    float alpha = (life / maxLife) * 255;
    shape.setFillColor(sf::Color(0, 255, 255, static_cast<sf::Uint8>(alpha)));
}

bool TrailParticle::isDead() const {
    return life <= 0;
}

Game::Game() 
    : window(sf::VideoMode(480, 853), "Triangle Game", sf::Style::Close)
    , currentState(GameState::Menu)
    , lastObstacleSpawn(sf::Time::Zero)
    , obstacleSpawnInterval(sf::seconds(1.0f))
    , isRunning(true)
    , mousePressed(false)
    , gameSpeed(300.0f)  // Decreased from 400
    , speedIncrement(40.0f)  // Decreased from 80 for slower progression
    , maxSpeed(1200.0f)  // Decreased from 1500
    , speedIncrementInterval(sf::seconds(2.0f))  // Increased interval for slower progression
    , score(0)
    , lives(5)  // Increased to 5 lives
    , screenShakeTime(0.0f)
    , screenShakeIntensity(0.0f)
    , screenShakeOffset(0.0f, 0.0f)
    , invulnerabilityTime(0.0f)
    , invulnerabilityDuration(1.5f)  // 1.5 seconds of invulnerability
    , isInvulnerable(false) {
    
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    
    // Load font with multiple fallback options
    bool fontLoaded = false;
    
    // Try multiple font options
    const std::vector<std::string> fontPaths = {
        "/System/Library/Fonts/HelveticaNeue.ttc",
        "/System/Library/Fonts/Geneva.ttf",
        "/System/Library/Fonts/SFNSMono.ttf",
        "/System/Library/Fonts/Arial.ttf"
    };
    
    for (const auto& fontPath : fontPaths) {
        if (font.loadFromFile(fontPath)) {
            fontLoaded = true;
            std::cout << "Font loaded successfully: " << fontPath << std::endl;
            break;
        }
    }
    
    if (!fontLoaded) {
        std::cout << "Warning: Could not load any system fonts, text may not display properly" << std::endl;
    }
    
    // Setup UI text
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);  // Smaller text for smaller window
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    
    speedText.setFont(font);
    speedText.setCharacterSize(16);  // Smaller text for smaller window
    speedText.setFillColor(sf::Color::Yellow);
    speedText.setPosition(10, 35);
    
    livesText.setFont(font);
    livesText.setCharacterSize(18);  // Smaller text for smaller window
    livesText.setFillColor(sf::Color::Red);
    livesText.setPosition(10, 60);
    
    // Setup title text
    titleText.setFont(font);
    titleText.setString("TRIANGLE DODGER");
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setStyle(sf::Text::Bold);
    
    // Center title text
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(
        (480.0f - titleBounds.width) / 2.0f,
        150.0f
    );
    
    // Setup game over text
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(32);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    
    // Center game over text
    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(
        (480.0f - gameOverBounds.width) / 2.0f,
        150.0f
    );
    
    // Setup final score text
    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(24);
    finalScoreText.setFillColor(sf::Color::White);
    
    // Initialize background particles
    for (int i = 0; i < 40; ++i) {  // Fewer particles for smaller screen
        spawnBackgroundParticle();
    }
    
    // Setup menu and game over screens
    setupMenu();
    setupGameOverScreen();
}

void Game::updateMenu() {
    // Update background particles for visual effect
    updateBackgroundParticles(0.016f); // Fixed delta time for smooth animation
    
    // Update buttons
    for (auto& button : menuButtons) {
        button->update(mousePos, mousePressed);
    }
}

void Game::updateGameOver() {
    // Update background particles for visual effect
    updateBackgroundParticles(0.016f); // Fixed delta time for smooth animation
    
    // Update final score text
    finalScoreText.setString("Final Score: " + std::to_string(score));
    sf::FloatRect finalScoreBounds = finalScoreText.getLocalBounds();
    finalScoreText.setPosition(
        (480.0f - finalScoreBounds.width) / 2.0f,
        220.0f
    );
    
    // Update buttons
    for (auto& button : gameOverButtons) {
        button->update(mousePos, mousePressed);
    }
}

void Game::renderMenu() {
    window.clear(sf::Color::Black);
    
    // Draw background particles
    for (const auto& particle : backgroundParticles) {
        window.draw(particle);
    }
    
    // Draw title
    window.draw(titleText);
    
    // Draw buttons
    for (auto& button : menuButtons) {
        button->draw(window);
    }
    
    window.display();
}

void Game::renderGameOver() {
    window.clear(sf::Color::Black);
    
    // Draw background particles
    for (const auto& particle : backgroundParticles) {
        window.draw(particle);
    }
    
    // Draw game over text
    window.draw(gameOverText);
    window.draw(finalScoreText);
    
    // Draw buttons
    for (auto& button : gameOverButtons) {
        button->draw(window);
    }
    
    window.display();
}

void Game::updateInvulnerability(float deltaTime) {
    if (isInvulnerable) {
        invulnerabilityTime -= deltaTime;
        player.setFlashing(true);
        if (invulnerabilityTime <= 0) {
            isInvulnerable = false;
            player.setFlashing(false);
        }
    } else {
        player.setFlashing(false);
    }
}



void Game::checkObstacleCollisions() {
    for (size_t i = 0; i < obstacles.size(); ++i) {
        for (size_t j = i + 1; j < obstacles.size(); ++j) {
            if (obstacles[i]->getBounds().intersects(obstacles[j]->getBounds())) {
                // Calculate collision response (elastic collision)
                sf::Vector2f pos1 = obstacles[i]->getPosition();
                sf::Vector2f pos2 = obstacles[j]->getPosition();
                sf::Vector2f vel1 = obstacles[i]->getVelocity();
                sf::Vector2f vel2 = obstacles[j]->getVelocity();
                
                // Calculate collision normal
                sf::Vector2f normal = pos2 - pos1;
                float distance = std::sqrt(normal.x * normal.x + normal.y * normal.y);
                if (distance > 0) {
                    normal /= distance;
                }
                
                // Calculate relative velocity
                sf::Vector2f relativeVel = vel2 - vel1;
                float velocityAlongNormal = relativeVel.x * normal.x + relativeVel.y * normal.y;
                
                // Don't resolve if objects are moving apart
                if (velocityAlongNormal > 0) {
                    continue;
                }
                
                // Calculate impulse
                float restitution = 0.8f;  // Bounciness factor
                float impulse = -(1.0f + restitution) * velocityAlongNormal;
                
                // Apply impulse
                sf::Vector2f impulseVector = normal * impulse;
                obstacles[i]->addVelocity(-impulseVector);
                obstacles[j]->addVelocity(impulseVector);
                
                // Separate the obstacles to prevent sticking
                float overlap = distance - (obstacles[i]->getSize() + obstacles[j]->getSize());
                if (overlap < 0) {
                    sf::Vector2f separation = normal * (-overlap * 0.5f);
                    obstacles[i]->setPosition(pos1 - separation);
                    obstacles[j]->setPosition(pos2 + separation);
                }
                
                // Create small explosion effect at collision point
                sf::Vector2f collisionPoint = (pos1 + pos2) * 0.5f;
                createExplosion(collisionPoint.x, collisionPoint.y);
            }
        }
    }
}

void Game::run() {
    while (window.isOpen() && isRunning) {
        float deltaTime = clock.restart().asSeconds();
        
        // Update mouse position
        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        switch (currentState) {
            case GameState::Menu:
                processMenuEvents();
                updateMenu();
                renderMenu();
                break;
                
            case GameState::Playing:
                processEvents();
                update(deltaTime);
                render();
                break;
                
            case GameState::GameOver:
                processGameOverEvents();
                updateGameOver();
                renderGameOver();
                break;
        }
    }
}

void Game::processMenuEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                mousePressed = true;
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                mousePressed = false;
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
    }
}

void Game::processGameOverEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                mousePressed = true;
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                mousePressed = false;
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            else if (event.key.code == sf::Keyboard::R) {
                startGame();
            }
        }
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                setState(GameState::Menu);
            }
            else if (event.key.code == sf::Keyboard::R) {
                reset();
            }
        }
    }
}

void Game::update(float deltaTime) {
    // Update speed
    updateSpeed();
    
    // Update visual effects
    updateScreenShake(deltaTime);
    updateExplosionParticles(deltaTime);
    updateTrailParticles(deltaTime);
    updateBackgroundParticles(deltaTime);
    updateInvulnerability(deltaTime);
    
    // Handle keyboard input for rocket movement
    bool isMoving = false;
    bool isSpeedBoosting = false;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.moveLeft(deltaTime);
        addTrailParticle(player.getPosition().x, player.getPosition().y);
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.moveRight(deltaTime);
        addTrailParticle(player.getPosition().x, player.getPosition().y);
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        player.moveForward(deltaTime);
        addTrailParticle(player.getPosition().x, player.getPosition().y);
        isMoving = true;
        isSpeedBoosting = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        player.moveBackward(deltaTime);
        addTrailParticle(player.getPosition().x, player.getPosition().y);
        isMoving = true;
    }
    
    // Set power states based on movement
    if (isSpeedBoosting && !isInvulnerable) {
        player.setPowerState(Player::PowerState::SpeedBoost, 0.1f); // Short duration for movement
    }
    
    // Return to center rotation when not moving
    if (!isMoving) {
        player.setTargetRotation(0.0f);
    }
    
    player.update(deltaTime);
    
    // Spawn obstacles
    if (spawnClock.getElapsedTime() > obstacleSpawnInterval) {
        spawnObstacle();
        spawnClock.restart();
    }
    
    // Update obstacles
    for (auto& obstacle : obstacles) {
        obstacle->update(deltaTime);
    }
    
    removeOffscreenObstacles();
    checkObstacleCollisions();  // Check obstacle-to-obstacle collisions
    checkCollisions();
    updateUI();
    
    // Score is now based on dodged obstacles (handled in removeOffscreenObstacles)
}

void Game::render() {
    window.clear(sf::Color::Black);
    
    // Apply screen shake
    sf::View view = window.getView();
    view.setCenter(240 + screenShakeOffset.x, 426.5f + screenShakeOffset.y);  // Center for 480x853
    window.setView(view);
    
    // Draw background particles
    for (const auto& particle : backgroundParticles) {
        window.draw(particle);
    }
    
    // Draw trail particles
    for (const auto& particle : trailParticles) {
        window.draw(particle.shape);
    }
    
    // Draw explosion particles
    for (const auto& particle : explosionParticles) {
        window.draw(particle.shape);
    }
    
    player.draw(window);
    for (auto& obstacle : obstacles) {
        obstacle->draw(window);
    }
    
    // Reset view for UI
    view.setCenter(240, 426.5f);  // Center for 480x853
    window.setView(view);
    
    // Draw UI
    window.draw(scoreText);
    window.draw(speedText);
    window.draw(livesText);
    
    window.display();
}

void Game::spawnObstacle() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> xDis(60.0f, 420.0f);  // Adjusted for 480 width
    static std::uniform_real_distribution<float> speedDis(0.5f, 2.0f);  // Random speed multiplier
    
    float x = xDis(gen);
    float y = -50.0f;
    float speedMultiplier = speedDis(gen);
    float speed = gameSpeed * speedMultiplier;  // Truly random speed!
    obstacles.push_back(std::make_unique<Obstacle>(x, y, speed));
}

void Game::spawnBackgroundParticle() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> xDis(0.0f, 480.0f);  // Full 480 width
    static std::uniform_real_distribution<float> yDis(0.0f, 853.0f);  // Full 853 height
    static std::uniform_real_distribution<float> sizeDis(1.0f, 2.5f);   // Smaller particles
    
    sf::CircleShape particle;
    particle.setRadius(sizeDis(gen));
    particle.setPosition(xDis(gen), yDis(gen));
    particle.setFillColor(sf::Color(200, 200, 200, 100));
    backgroundParticles.push_back(particle);
}

void Game::updateBackgroundParticles(float deltaTime) {
    for (auto& particle : backgroundParticles) {
        sf::Vector2f pos = particle.getPosition();
        pos.y += gameSpeed * 0.3f * deltaTime;
        
        if (pos.y > 853.0f) {  // Adjusted for 853 height
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<float> xDis(0.0f, 480.0f);
            pos.x = xDis(gen);
            pos.y = -20.0f;
        }
        
        particle.setPosition(pos);
    }
}

void Game::updateExplosionParticles(float deltaTime) {
    for (auto it = explosionParticles.begin(); it != explosionParticles.end();) {
        it->update(deltaTime);
        if (it->isDead()) {
            it = explosionParticles.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::updateTrailParticles(float deltaTime) {
    for (auto it = trailParticles.begin(); it != trailParticles.end();) {
        it->update(deltaTime);
        if (it->isDead()) {
            it = trailParticles.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::createExplosion(float x, float y) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> velDis(-200.0f, 200.0f);
    static std::uniform_real_distribution<float> lifeDis(0.5f, 1.0f);
    
    for (int i = 0; i < 15; ++i) {
        float vx = velDis(gen);
        float vy = velDis(gen);
        float life = lifeDis(gen);
        explosionParticles.emplace_back(x, y, vx, vy, life);
    }
}

void Game::addTrailParticle(float x, float y) {
    trailParticles.emplace_back(x, y, 0.3f);
}

void Game::updateScreenShake(float deltaTime) {
    if (screenShakeTime > 0) {
        screenShakeTime -= deltaTime;
        
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> shakeDis(-1.0f, 1.0f);
        
        screenShakeOffset.x = shakeDis(gen) * screenShakeIntensity;
        screenShakeOffset.y = shakeDis(gen) * screenShakeIntensity;
        
        if (screenShakeTime <= 0) {
            screenShakeOffset = sf::Vector2f(0, 0);
        }
    }
}

void Game::updateUI() {
    scoreText.setString("Score: " + std::to_string(score));
    speedText.setString("Speed: " + std::to_string(static_cast<int>(gameSpeed)));
    livesText.setString("Lives: " + std::to_string(lives));
}

void Game::updateSpeed() {
    if (speedClock.getElapsedTime() > speedIncrementInterval) {
        gameSpeed += speedIncrement;
        if (gameSpeed > maxSpeed) {
            gameSpeed = maxSpeed;
        }
        
        float newInterval = obstacleSpawnInterval.asSeconds() - 0.15f;
        if (newInterval > 0.2f) {
            obstacleSpawnInterval = sf::seconds(newInterval);
        }
        
        // Power state for high speed
        if (gameSpeed > 800.0f && !isInvulnerable) {
            player.setPowerState(Player::PowerState::Overcharged, 0.5f);
        }
        
        speedClock.restart();
    }
}

void Game::checkCollisions() {
    if (isInvulnerable) return; // Skip collision check if invulnerable
    
    for (auto it = obstacles.begin(); it != obstacles.end();) {
        if (player.getBounds().intersects((*it)->getBounds())) {
            // Create explosion at collision point
            createExplosion(player.getPosition().x, player.getPosition().y);
            
            // Screen shake
            screenShakeTime = 0.3f;
            screenShakeIntensity = 10.0f;
            
            lives--;
            if (lives <= 0) {
                std::cout << "Game Over! Final Score: " << score << std::endl;
                setState(GameState::GameOver);
            } else {
                std::cout << "Lives remaining: " << lives << std::endl;
                // Reset player position
                player.reset();
                
                // Activate invulnerability
                isInvulnerable = true;
                invulnerabilityTime = invulnerabilityDuration;
                player.setPowerState(Player::PowerState::Invulnerable, invulnerabilityDuration);
            }
            
            // Remove the obstacle that caused the collision
            it = obstacles.erase(it);
            return; // Exit after first collision to prevent multiple life losses
        } else {
            ++it;
        }
    }
}

void Game::removeOffscreenObstacles() {
    // Count obstacles that go offscreen (dodged) and add to score
    int dodgedCount = 0;
    obstacles.erase(
        std::remove_if(obstacles.begin(), obstacles.end(),
            [&dodgedCount](const std::unique_ptr<Obstacle>& obstacle) {
                if (obstacle->isOffscreen()) {
                    dodgedCount++;
                    return true;
                }
                return false;
            }),
        obstacles.end()
    );
    
    // Add score for dodged obstacles
    if (dodgedCount > 0) {
        score += dodgedCount;
        std::cout << "Dodged " << dodgedCount << " obstacles! Score: " << score << std::endl;
        
        // Power states based on score milestones
        if (score % 10 == 0 && score > 0) {
            // Every 10 points - charging state
            player.setPowerState(Player::PowerState::Charging, 1.0f);
        }
        if (score % 25 == 0 && score > 0) {
            // Every 25 points - overcharged state
            player.setPowerState(Player::PowerState::Overcharged, 2.0f);
        }
    }
}

void Game::reset() {
    obstacles.clear();
    backgroundParticles.clear();
    explosionParticles.clear();
    trailParticles.clear();
    player.reset();
    isRunning = true;
    spawnClock.restart();
    speedClock.restart();
    gameSpeed = 300.0f;  // Reset to initial speed
    obstacleSpawnInterval = sf::seconds(1.0f);
    speedIncrement = 40.0f;  // Reset speed increment
    speedIncrementInterval = sf::seconds(2.0f);  // Reset interval
    score = 0;
    lives = 5;  // Reset to 5 lives
    screenShakeTime = 0.0f;
    screenShakeOffset = sf::Vector2f(0, 0);
    
    for (int i = 0; i < 40; ++i) {  // Fewer particles for smaller screen
        spawnBackgroundParticle();
    }
} 

void Game::setupMenu() {
    // Clear existing buttons
    menuButtons.clear();
    
    // Play button
    auto playButton = std::make_unique<Button>(font, "PLAY", 140, 300, 200, 50);
    playButton->setOnClick([this]() { startGame(); });
    playButton->setColors(
        sf::Color(0, 150, 0, 200),    // Normal - Green
        sf::Color(0, 200, 0, 200),    // Hover - Bright Green
        sf::Color(0, 100, 0, 200)     // Pressed - Dark Green
    );
    menuButtons.push_back(std::move(playButton));
    
    // Quit button
    auto quitButton = std::make_unique<Button>(font, "QUIT", 140, 370, 200, 50);
    quitButton->setOnClick([this]() { quitGame(); });
    quitButton->setColors(
        sf::Color(150, 0, 0, 200),    // Normal - Red
        sf::Color(200, 0, 0, 200),    // Hover - Bright Red
        sf::Color(100, 0, 0, 200)     // Pressed - Dark Red
    );
    menuButtons.push_back(std::move(quitButton));
}

void Game::setupGameOverScreen() {
    // Clear existing buttons
    gameOverButtons.clear();
    
    // Play Again button
    auto playAgainButton = std::make_unique<Button>(font, "PLAY AGAIN", 140, 400, 200, 50);
    playAgainButton->setOnClick([this]() { startGame(); });
    playAgainButton->setColors(
        sf::Color(0, 150, 0, 200),    // Normal - Green
        sf::Color(0, 200, 0, 200),    // Hover - Bright Green
        sf::Color(0, 100, 0, 200)     // Pressed - Dark Green
    );
    gameOverButtons.push_back(std::move(playAgainButton));
    
    // Main Menu button
    auto menuButton = std::make_unique<Button>(font, "MAIN MENU", 140, 470, 200, 50);
    menuButton->setOnClick([this]() { returnToMenu(); });
    menuButton->setColors(
        sf::Color(0, 100, 150, 200),  // Normal - Blue
        sf::Color(0, 150, 200, 200),  // Hover - Bright Blue
        sf::Color(0, 50, 100, 200)    // Pressed - Dark Blue
    );
    gameOverButtons.push_back(std::move(menuButton));
    
    // Quit button
    auto quitButton = std::make_unique<Button>(font, "QUIT", 140, 540, 200, 50);
    quitButton->setOnClick([this]() { quitGame(); });
    quitButton->setColors(
        sf::Color(150, 0, 0, 200),    // Normal - Red
        sf::Color(200, 0, 0, 200),    // Hover - Bright Red
        sf::Color(100, 0, 0, 200)     // Pressed - Dark Red
    );
    gameOverButtons.push_back(std::move(quitButton));
}

void Game::startGame() {
    currentState = GameState::Playing;
    reset();
}

void Game::returnToMenu() {
    currentState = GameState::Menu;
    reset();
}

void Game::quitGame() {
    window.close();
    isRunning = false;
}

void Game::setState(GameState state) {
    currentState = state;
} 
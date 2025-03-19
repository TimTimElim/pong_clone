// Simple example showing how the game loop works
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <random>
#include <iostream>

// Helper function to convert degrees to radians
constexpr float degToRad(float degrees){
    return degrees * 3.14159265358979323846f / 180.f;
}

// Function to generate a new ball velocity with a random direction
sf::Vector2f getInitialBallVelocity(float speed, std::mt19937& gen) {
    // Randomly choose left (true) or right (false)
    bool goLeft = std::uniform_int_distribution<>(0, 1)(gen) == 0;
    float angleDegrees;
    if (goLeft) {
        // Left: angle between 135° and 225° (i.e., 180° ± 45°)
        std::uniform_real_distribution<float> angleDist(135.f, 225.f);
        angleDegrees = angleDist(gen);
    } else {
        // Right: angle between -45° and 45° (i.e., 0° ± 45°)
        std::uniform_real_distribution<float> angleDist(-45.f, 45.f);
        angleDegrees = angleDist(gen);
    }
    float angleRadians = degToRad(angleDegrees);
    return { std::cos(angleRadians) * speed, std::sin(angleRadians) * speed };
}

int main()
{
    // Window dimensions
    const int windowWidth = 800;
    const int windowHeight = 600;

    // Create an instance of the RenderWindow class.
    // This object is the graphical window of our program.
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Simple Pong Clone");
    window.setFramerateLimit(60);

    // Enable vertical sync. Locks framerate to the display and prevents tearing.
    window.setVerticalSyncEnabled(true);

    // Paddle dimensions
    const float paddleWidth = 10.f;
    const float paddleHeight = 100.f;

    // Left paddle
    sf::RectangleShape leftPaddle(sf::Vector2f (paddleWidth, paddleHeight));
    leftPaddle.setFillColor(sf::Color::White);
    leftPaddle.setPosition({50.f, windowHeight / 2.f - paddleHeight / 2.f});

    // Right paddle
    sf::RectangleShape rightPaddle(sf::Vector2f (paddleWidth, paddleHeight));
    rightPaddle.setFillColor(sf::Color::White);
    rightPaddle.setPosition({windowWidth - 50.f - paddleWidth, windowHeight / 2.f - paddleHeight / 2.f});

    // Ball dimensions
    const float ballRadius = 10.f;
    sf::CircleShape ball(ballRadius);
    ball.setFillColor(sf::Color::White);
    ball.setPosition({windowWidth / 2.f - ballRadius, windowHeight / 2.f - ballRadius});
    // Initial Ball Velocity
    const float initialSpeed = 4.f;

    // Randomizer engine setup
    std::random_device rd;
    std::mt19937 gen(rd());

    // Set initial ball velocity using helper function
    sf::Vector2f ballVelocity = getInitialBallVelocity(initialSpeed, gen);

    // Score tracking
    int leftScore = 0;
    int rightScore = 0;
    int winningScore = 2; // Score required to change gamestate from normal to winning state.

    // Game state variable to track winning condition
    bool showWin = false;
    sf::Clock winClock;

    // Load font for score tracking and drawing on the main window
    sf::Font font("../font/comic.ttf");

    sf::Text textLeftScore(font);
    textLeftScore.setString("P1: " + std::to_string(leftScore));
    textLeftScore.setPosition({windowWidth / 2 - 100.f, 0.f});

    sf::Text textRightScore(font);
    textRightScore.setString("PC: " + std::to_string(rightScore));
    textRightScore.setPosition({windowWidth / 2 + 100.f, 0.f});

    sf::Text winText(font);
    winText.setFillColor(sf::Color::Yellow);

    // Load sound for sound effects
    sf::SoundBuffer buffer_pong("../sound/arcade-fx-288597_pong_sound.mp3");
    sf::SoundBuffer buffer_gameOver("../sound/game-over-arcade-6435.mp3");
    sf::Sound sound_pong(buffer_pong);
    sf::Sound sound_gameOver(buffer_gameOver);

    // Run the game loop as long as the window is open
    while (window.isOpen())
    {
        // Read all events from the window, one at a time
        while (const std::optional event = window.pollEvent())
        {
            // Quit when the user presses 'x', ignore all other events
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (!showWin) // Normal gameplay
        {
            // Paddle movement speed
            const float paddleSpeed = 10.f;

            // Move left paddle using W (up) and S (down)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                leftPaddle.move({0.f, -paddleSpeed});
                if (leftPaddle.getPosition().y < 0)
                    leftPaddle.setPosition({leftPaddle.getPosition().x, 0.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                leftPaddle.move({0.f, +paddleSpeed});
                if (leftPaddle.getPosition().y + paddleHeight > windowHeight)
                    leftPaddle.setPosition({leftPaddle.getPosition().x, windowHeight - paddleHeight});
            }

            // Move right paddle using up and down arrows
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
                rightPaddle.move({0.f, -paddleSpeed});
                if (rightPaddle.getPosition().y < 0)
                    rightPaddle.setPosition({rightPaddle.getPosition().x, 0.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                rightPaddle.move({0.f, +paddleSpeed});
                if (rightPaddle.getPosition().y + paddleHeight > windowHeight)
                    rightPaddle.setPosition({rightPaddle.getPosition().x, windowHeight - paddleHeight});
            }

            // Otherwise let P2 be controlled by AI
            const float aiSpeed = 4.f; // Adjust for difficulty

            // Calculate the center y position of the right paddle
            float rightPaddleCenterY = rightPaddle.getPosition().y + rightPaddle.getSize().y / 2.f;
            // Calculate the target y position based on the ball's position
            // Adjust by the ball's radius to aim for the ball's center
            float ballCenterY = ball.getPosition().y + ballRadius;

            // Move the AI paddle toward teh ball for a fixed amount per frame
            if (rightPaddleCenterY < ballCenterY)
            {
                rightPaddle.move({0.f, aiSpeed});
                // Ensure paddle doesn't move below window
                if (rightPaddle.getPosition().y + rightPaddle.getSize().y > windowHeight)
                    rightPaddle.setPosition({rightPaddle.getPosition().x, windowHeight - rightPaddle.getSize().y});
            }
            else if (rightPaddleCenterY > ballCenterY)
            {
                rightPaddle.move({0.f, -aiSpeed});
                // Ensure paddle doesn't move above window
                if (rightPaddle.getPosition().y < 0)
                    rightPaddle.setPosition({rightPaddle.getPosition().x, 0.f});
            }

            // Ball movement
            ball.move({ballVelocity});
            sf::Vector2f ballPos = ball.getPosition();

            // Bounce off top and bottom edge
            if (ballPos.y <= 0 || ballPos.y + 2 * ballRadius >= windowHeight) {
                ballVelocity.y = -ballVelocity.y;
            }

            // Paddle collision check
            if (ball.getGlobalBounds().findIntersection(leftPaddle.getGlobalBounds())) {
                // Play sound on collision
                sound_pong.play();
                // Bounces ball off the left paddle and inverts its direction
                ballVelocity.x = std::abs(ballVelocity.x);
                ballVelocity *= 1.2f;

            }
            if (ball.getGlobalBounds().findIntersection(rightPaddle.getGlobalBounds())) {
                // Play sound on collision
                sound_pong.play();
                // Bounces ball off the left paddle and inverts its direction
                ballVelocity.x = -std::abs(ballVelocity.x);
                ballVelocity *= 1.2f;
            }

            // Check if ball goes past the left or right edge - Scoring
            if (ballPos.x < 0) {
                // Play sound
                sound_gameOver.play();
                // Right player scores if ball goes outside left edge
                rightScore++;
                std::cout << "Right Player Scores! (" << leftScore << " - " << rightScore << ")" << std::endl;
                // Reset ball to center and reverse its horizontal direction
                ball.setPosition({windowWidth / 2.f - ballRadius, windowHeight / 2.f - ballRadius});
                ballVelocity = getInitialBallVelocity(initialSpeed, gen);
            } else if (ballPos.x > windowWidth) {
                // Play sound
                sound_gameOver.play();
                // Left player scores if ball goes outside right edge
                leftScore++;
                std::cout << "Left Player Scores! (" << leftScore << " - " << rightScore << ")" << std::endl;
                // Reset ball to center and reverse its horizontal direction
                ball.setPosition({windowWidth / 2.f - ballRadius, windowHeight / 2.f - ballRadius});
                ballVelocity = getInitialBallVelocity(initialSpeed, gen);
            }

            // Check Win condition
            if (leftScore >= winningScore) {
                // Winning announcement
                winText.setString("Player Wins!");
                // Center text (adjust origin after setting string)
                sf::FloatRect textRect = winText.getLocalBounds();
                winText.setOrigin(textRect.getCenter());
                winText.setPosition({windowWidth / 2.f, windowHeight / 2.f});
                showWin = true;
                winClock.restart();
            } else if (rightScore >= winningScore) {
                // Winning announcement
                winText.setString("Computer Wins!");
                // Center text (adjust origin after setting string)
                sf::FloatRect textRect = winText.getLocalBounds();
                winText.setOrigin(textRect.getCenter());
                winText.setPosition({windowWidth / 2.f, windowHeight / 2.f});
                showWin = true;
                winClock.restart();
            }
        }

        if (showWin) // Win Screen displayed
        {
            if (winClock.getElapsedTime().asSeconds() >= 2)
            {
                // Reset scores
                leftScore = 0;
                rightScore = 0;
                // Ball reset
                ball.setPosition({windowWidth / 2.f - ballRadius, windowHeight / 2.f - ballRadius});
                ballVelocity.x = -std::abs(ballVelocity.x);

                // Resume normal gameplay
                showWin = false;
            }
        }

        // Clear the window to black
        window.clear();
        if (showWin)
        {
            window.draw(winText);
        }
        else
        {
            // Draw our circle
            textLeftScore.setString("P1: " + std::to_string(leftScore));
            textRightScore.setString("PC: " + std::to_string(rightScore));
            window.draw(textLeftScore);
            window.draw(textRightScore);
            window.draw(leftPaddle);
            window.draw(rightPaddle);
            window.draw(ball);
        }



        // Flip the back buffer and front buffer, showing what was drawn.
        // This is a technique called double buffering, where we draw on a non-visible screen then flip them.
        window.display();
    }

    return 0;
}
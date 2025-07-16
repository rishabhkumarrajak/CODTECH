#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <ctime>

const int WIDTH = 800, HEIGHT = 600, BLOCK_SIZE = 20;
const float INITIAL_SPEED = 0.15f;

enum Direction { Stop = 0, Left, Right, Up, Down };

struct SnakeSegment {
    int x, y;
};

class SnakeGame {
private:
    sf::RenderWindow window;
    std::vector<SnakeSegment> snake;
    SnakeSegment food;
    Direction dir = Stop;
    sf::Font font;
    sf::Text scoreText;
    sf::SoundBuffer eatBuffer, gameOverBuffer;
    sf::Sound eatSound, gameOverSound;
    int score = 0;
    float speed = INITIAL_SPEED;
    sf::Clock clock;

    void spawnFood() {
        food.x = rand() % (WIDTH / BLOCK_SIZE);
        food.y = rand() % (HEIGHT / BLOCK_SIZE);
    }

    void reset() {
        snake.clear();
        snake.push_back({ WIDTH / (2 * BLOCK_SIZE), HEIGHT / (2 * BLOCK_SIZE) });
        score = 0;
        speed = INITIAL_SPEED;
        dir = Stop;
        spawnFood();
    }

public:
    SnakeGame() : window(sf::VideoMode(WIDTH, HEIGHT), "Snake Game with SFML") {
        window.setFramerateLimit(60);
        font.loadFromFile("arial.ttf");
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        eatBuffer.loadFromFile("eat.wav");
        gameOverBuffer.loadFromFile("gameover.wav");
        eatSound.setBuffer(eatBuffer);
        gameOverSound.setBuffer(gameOverBuffer);
        reset();
    }

    void run() {
        while (window.isOpen()) {
            sf::Event e;
            while (window.pollEvent(e)) {
                if (e.type == sf::Event::Closed)
                    window.close();
            }

            // Input
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dir = Left;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dir = Right;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) dir = Up;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) dir = Down;

            if (clock.getElapsedTime().asSeconds() >= speed) {
                update();
                clock.restart();
            }

            draw();
        }
    }

    void update() {
        // Move snake
        SnakeSegment head = snake.front();
        switch (dir) {
            case Left: head.x--; break;
            case Right: head.x++; break;
            case Up: head.y--; break;
            case Down: head.y++; break;
            default: break;
        }

        // Collision checks
        if (head.x < 0 || head.x >= WIDTH / BLOCK_SIZE || head.y < 0 || head.y >= HEIGHT / BLOCK_SIZE)
            return gameOver();

        for (auto s : snake)
            if (s.x == head.x && s.y == head.y)
                return gameOver();

        snake.insert(snake.begin(), head);

        // Eat food
        if (head.x == food.x && head.y == food.y) {
            eatSound.play();
            score += 10;
            speed *= 0.95f; // Increase difficulty
            spawnFood();
        } else {
            snake.pop_back();
        }
    }

    void draw() {
        window.clear();

        // Draw snake
        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
        block.setFillColor(sf::Color::Green);
        for (auto s : snake) {
            block.setPosition(s.x * BLOCK_SIZE, s.y * BLOCK_SIZE);
            window.draw(block);
        }

        // Draw food
        block.setFillColor(sf::Color::Red);
        block.setPosition(food.x * BLOCK_SIZE, food.y * BLOCK_SIZE);
        window.draw(block);

        // Draw score
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setPosition(10, 10);
        window.draw(scoreText);

        window.display();
    }

    void gameOver() {
        gameOverSound.play();
        sf::sleep(sf::seconds(1.5));
        reset();
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));
    SnakeGame game;
    game.run();
    return 0;
}

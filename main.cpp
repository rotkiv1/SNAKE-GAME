#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <thread>

sf::RenderWindow mWindow;

auto generator = std::default_random_engine(std::random_device()());
std::uniform_int_distribution<int> distribution(0, 405 / 15 - 1);

class Snake {
    sf::RectangleShape s;

    public:
        float x = distribution(generator) * 15;
        float y = distribution(generator) * 15;

        Snake() noexcept {
            s.setPosition(x, y);
            s.setFillColor(sf::Color::Magenta);
            s.setSize({15.f, 15.f});
        }

        Snake(float&& a, float&& b) : x(std::move(a)), y(std::move(b)) {
            s.setPosition(x, y);
            s.setFillColor(sf::Color::Magenta);
            s.setSize({15.f, 15.f});
        }

        ~Snake() = default;

        bool operator ==(const Snake& s) {
            return this->x == s.x && this->y == s.y;
        }

        void update(sf::Vector2<float> v) noexcept {
            this->x += 15 * v.x;
            this->y += 15 * v.y;
        }

        auto show() noexcept {
            s.setPosition(x, y);
            return s;
        }
};


class Game {
    public:
        Game() noexcept : movement({1.f, 0.f}) {
            if (!food.loadFromFile("food.png")) {
                // error...
            }
            if (!over.loadFromFile("snake.ttf")) {
                // error...
            }
            if (!mWindow.isOpen()) {
                mWindow.create(sf::VideoMode(405, 405), "Snake");
            }
            target.setPosition(distribution(generator) * 15,
                               distribution(generator) * 15);
            target.setTextureRect({0, 0, 15, 15});
            target.setTexture(food);
            sn.push_back(snake);
        }

        ~Game() = default;

        void run() {
            sf::Clock clock;
            sf::Time timeSinceLastUpdate = sf::Time::Zero;
            while (mWindow.isOpen()) {
                processEvents();
                timeSinceLastUpdate += clock.restart();
                while (timeSinceLastUpdate > TimePerFrame) {
                    timeSinceLastUpdate -= TimePerFrame;
                    processEvents();
                    update();
                }
                render();
            }
        }

    private:
        bool checkFood(const sf::Sprite& target, const std::vector<Snake>& snake) {
            return (target.getPosition().x == snake.back().x) &&
                   (target.getPosition().y == snake.back().y);
        }

        void processEvents() {
            sf::Event event;
            while (mWindow.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::KeyPressed:
                        handlePlayerInput(event.key.code, true);
                        break;
                    case sf::Event::KeyReleased:
                        handlePlayerInput(event.key.code, false);
                        break;
                    case sf::Event::Closed:
                        mWindow.close();
                        break;
                }
            }
        }

        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
            if (key == sf::Keyboard::W) {
                mIsMovingUp = (movement.y == 1.f) ? false : isPressed;
            } else if (key == sf::Keyboard::S) {
                mIsMovingDown = (movement.y == -1.f) ? false : isPressed;
            } else if (key == sf::Keyboard::A) {
                mIsMovingLeft = (movement.x == 1.f) ? false : isPressed;
            } else if (key == sf::Keyboard::D) {
                mIsMovingRight = (movement.x == -1.f) ? false : isPressed;
            }
        }

        void update() {
            updateSnake();
            for (auto i = 0; i < sn.size() - 1; i++) {
                if (sn[i] == sn.back()) {
                    runNewGame();
                }
            }
            if (checkFood(target, sn)) {
                grow();
                target.setPosition(distribution(generator) * 15,
                                   distribution(generator) * 15);
            }
        }

        bool determineEnd(const std::vector<Snake> sn) noexcept {
            return (sn.back().x >= 405.f) || (sn.back().x <= -15.f) ||
                   (sn.back().y <= -15.f) || (sn.back().y >= 405.f);
        }

        void render() {
            mWindow.clear();
            if (determineEnd(sn)) {
                runNewGame();
            }
            for (auto& part : sn) {
                mWindow.draw(part.show());
            }
            mWindow.draw(target);
            mWindow.display();
        }

        void runNewGame() {
            sf::Text text;
            text.setFont(over);
            text.setString("GAME\nOVER");
            text.setCharacterSize(50);
            text.setPosition(150.f, 120.f);
            text.setFillColor(sf::Color::White);
            mWindow.draw(text);
            mWindow.display();
            using namespace std::literals;
            std::this_thread::sleep_for(500ms);
            Game g2;
            g2.run();
        }

        void grow() {
            auto x = sn.back();
            sn.push_back(x);
        }

        void updateSnake() {
            auto x = sn.back();
            sn.erase(sn.begin());
            if (mIsMovingUp) {
                movement.x = 0.f;
                movement.y = -1.f;
            }
            if (mIsMovingDown) {
                movement.x = 0.f;
                movement.y = 1.f;
            }
            if (mIsMovingLeft) {
                movement.x = -1.f;
                movement.y = 0.f;
            }
            if (mIsMovingRight) {
                movement.x = 1.f;
                movement.y = 0.f;
            }
            x.update(movement);
            sn.push_back(x);
        }

        sf::Time TimePerFrame = sf::seconds(1.f / 15.f);
        sf::Sprite target;
        sf::Texture food;
        sf::Vector2f movement;
        sf::Font over;
        Snake snake;
        std::vector<Snake> sn;

        bool mIsMovingUp = false;
        bool mIsMovingDown = false;
        bool mIsMovingLeft = false;
        bool mIsMovingRight = false;
};

int main() {
    Game g;
    g.run();
}

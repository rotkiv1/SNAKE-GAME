#include <SFML/Graphics.hpp>

#include <thread>
#include <memory>

#include "OneSnakePart.h"
#include "Snake.h"
#include "generator.h"

Game::Game()
     : movement({1.f, 0.f}) {
    mWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(405, 405),
                                                 "Snake");
    initGame();
}

Game::Game(std::shared_ptr<sf::RenderWindow> previous)
     : mWindow(previous), movement({1.f, 0.f}) {
    initGame();
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow->isOpen()) {
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

bool Game::checkFood(const sf::Sprite& target,
                     const std::vector<OnePartOfSnake>& snake) noexcept {
    auto snakeHeadPosition = snake.back();
    return (target.getPosition().x == snakeHeadPosition.x) &&
           (target.getPosition().y == snakeHeadPosition.y);
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case sf::Event::Closed:
                mWindow->close();
                break;
        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key,
                             bool isPressed) noexcept {
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

void Game::update() {
    updateSnake();
    auto snakeHead = snake.back();
    for (auto i = 0; i < snake.size() - 1; i++) {
        if (snake[i] == snakeHead) {
            runNewGame();
        }
    }
    if (checkFood(target, snake)) {
        growSnake();
        float x = distribution(generator) * 15;
        float y = distribution(generator) * 15;
        while (!canPlaceFoodOutsideSnake(x, y, textScore) &&
               !isFoodPlacesRight(x, y, snake)) {
                x = distribution(generator) * 15;
                y = distribution(generator) * 15;
        }
        target.setPosition(x, y);
        score += 15;
        textScore.setString("SCORE\t" + std::to_string(score));
    }
}

bool Game::canPlaceFoodOutsideSnake(float foodX, float foodY,
                                    const sf::Text& score) noexcept {
    return foodX > score.getLocalBounds().width &&
           foodY > score.getLocalBounds().height;
}

bool Game::isFoodPlacesRight(float foodX, float foodY,
                       const std::vector<OnePartOfSnake>& snake) noexcept {
    for (const auto& part : snake) {
        if (part.x == foodX && part.y == foodY) {
            return false;
        }
    }
    return true;
}

bool Game::determineEnd(const std::vector<OnePartOfSnake>& snake) noexcept {
    auto headOfSnake = snake.back();
    return (headOfSnake.x >= 405.f) || (headOfSnake.x <= -15.f) ||
           (headOfSnake.y <= -15.f) || (headOfSnake.y >= 405.f);
}

void Game::render() {
    mWindow->clear();
    if (determineEnd(snake)) {
        runNewGame();
    }
    mWindow->draw(backgroundSprite);
    for (auto& part : snake) {
        mWindow->draw(part.show());
    }
    mWindow->draw(target);
    mWindow->draw(textScore);
    mWindow->display();
}

void Game::runNewGame() {
    sf::Text text;
    text.setFont(over);
    text.setString("GAME\nOVER");
    text.setCharacterSize(50);
    text.setPosition(150.f, 120.f);
    text.setFillColor(sf::Color::White);

    textScore.setPosition(160.f, 230.f);

    mWindow->draw(backgroundSprite);
    mWindow->draw(textScore);
    mWindow->draw(text);
    mWindow->display();

    using namespace std::literals;
    std::this_thread::sleep_for(500ms);

    Game newGame(mWindow);
    newGame.run();
}

void Game::growSnake() {
    auto newPart = snake.back();
    snake.push_back(newPart);
}

void Game::updateSnake() {
    auto newPart = snake.back();
    snake.erase(snake.begin());
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
    newPart.update(movement);
    snake.push_back(newPart);
}

void Game::initGame() {
    over.loadFromFile("snake.ttf");

    backgroundTexture.loadFromFile("background.png");
    backgroundSprite.setTexture(backgroundTexture);

    imageFood.loadFromFile("food.png");
    imageFood.createMaskFromColor(sf::Color(4, 1, 44));
    food.loadFromImage(imageFood);

    textScore.setFont(over);
    textScore.setString("SCORE\t" + std::to_string(score));
    textScore.setCharacterSize(20);
    textScore.setPosition(0.f, 0.f);
    textScore.setFillColor(sf::Color::White);

    float x = distribution(generator) * 15;
    float y = distribution(generator) * 15;
    while (!canPlaceFoodOutsideSnake(x, y, textScore) &&
           !isFoodPlacesRight(x, y, snake)) {
            x = distribution(generator) * 15;
            y = distribution(generator) * 15;
    }

    target.setPosition(x, y);
    target.setTextureRect({0, 0, 15, 15});
    target.setTexture(food);

    snake.push_back(snakeHead);
}


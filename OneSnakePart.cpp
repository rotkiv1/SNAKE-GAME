#include <SFML/Graphics.hpp>

#include "OneSnakePart.h"

OnePartOfSnake::OnePartOfSnake() {
    createPartOfSnake(x, y);

    snakePartImage.loadFromFile("snakePart.png");
    snakePartImage.createMaskFromColor(sf::Color(4, 1, 44));
    snakePartTexture.loadFromImage(snakePartImage);
    snakePartView.setTexture(snakePartTexture);
}

OnePartOfSnake::OnePartOfSnake(float&& a, float&& b) noexcept
: x(std::move(a)), y(std::move(b)) {
    createPartOfSnake(x, y);
}

void OnePartOfSnake::update(const sf::Vector2<float>& v) noexcept {
    this->x += 15 * v.x;
    this->y += 15 * v.y;
}

sf::Sprite OnePartOfSnake::show() noexcept {
    snakePartView.setPosition(x, y);
    return snakePartView;
}

void OnePartOfSnake::createPartOfSnake(float x, float y) noexcept {
    snakePartView.setPosition(x, y);
}

bool OnePartOfSnake::operator ==(const OnePartOfSnake& part) const noexcept {
    return this->x == part.x && this->y == part.y;
}

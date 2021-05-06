#ifndef ONESNAKEPART_H_INCLUDED
#define ONESNAKEPART_H_INCLUDED

#include "generator.h"

class OnePartOfSnake {

    public:

        float x = distribution(generator) * 15;
        float y = distribution(generator) * 15;

        OnePartOfSnake();
        OnePartOfSnake(float&& a, float&& b) noexcept;

        ~OnePartOfSnake() noexcept = default;

        bool operator ==(const OnePartOfSnake& part) const noexcept;

        void update(const sf::Vector2<float>& v) noexcept;

        sf::Sprite show() noexcept;

    private:

        void createPartOfSnake(float x, float y) noexcept;

        sf::Image snakePartImage;
        sf::Texture snakePartTexture;
        sf::Sprite snakePartView;
};

#endif


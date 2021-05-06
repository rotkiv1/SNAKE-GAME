#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#include <memory>
#include <thread>

class Game {

    public:

        Game();
        Game(std::shared_ptr<sf::RenderWindow> previous);

        ~Game() noexcept = default;

        void run();

    private:

        bool checkFood(const sf::Sprite& target,
                       const std::vector<OnePartOfSnake>& snake) noexcept;

        void processEvents();

        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) noexcept;

        void update();

        bool canPlaceFoodOutsideSnake(float foodX, float foodY,
                                      const sf::Text& score) noexcept;

        bool isFoodPlacesRight(float foodX, float foodY,
                               const std::vector<OnePartOfSnake>& snake) noexcept;

        bool determineEnd(const std::vector<OnePartOfSnake>& snake) noexcept;

        void render();

        void runNewGame();

        void growSnake();

        void updateSnake();

        void initGame();

        sf::Time TimePerFrame = sf::seconds(1.f / 15.f);
        sf::Sprite target, backgroundSprite;
        sf::Texture food, backgroundTexture;
        sf::Vector2f movement;
        sf::Font over;
        sf::Text textScore;
        sf::Image imageFood;
        std::shared_ptr<sf::RenderWindow> mWindow;

        OnePartOfSnake snakeHead;
        std::vector<OnePartOfSnake> snake;

        int score = 0;
        bool mIsMovingUp = false;
        bool mIsMovingDown = false;
        bool mIsMovingLeft = false;
        bool mIsMovingRight = false;
};




#endif

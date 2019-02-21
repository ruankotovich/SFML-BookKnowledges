#include <SFML/Graphics.hpp>
#include <iostream>
auto TimePerFrame = sf::seconds(1.f / 120.f);

class Game {
private:
    bool mIsMovingUp = false, mIsMovingDown = false, mIsMovingLeft = false, mIsMovingRight = false;
    sf::RenderWindow mWindow;
    sf::Texture mTexture;
    sf::Sprite mPlayer;
    // sf::CircleShape mPlayer;
    sf::Vector2f speed, acceleration, friction;

public:
    Game()
        : mWindow(sf::VideoMode(640, 480), "SFML Application")
        , speed(1.f, 1.f)
        , acceleration(3.f, 3.f)
        , friction(.5f, .5f)
        , mPlayer()
    {
        // mPlayer.setRadius(40.f);
        // mPlayer.setPosition(100.f, 100.f);
        // mPlayer.setFillColor(sf::Color::Red);
        mWindow.setVerticalSyncEnabled(true);

        if (!mTexture.loadFromFile("gf/sphere.png")) {
            // error
        }

        mPlayer.setTexture(mTexture);
        mPlayer.setPosition(100.f, 100.f);
        mPlayer.setRotation(0.f);
        mPlayer.setOrigin(mTexture.getSize().x / 2.f, mTexture.getSize().y / 2.f);
    }
    void run()
    {
        sf::Clock clock;
        sf::Time timeSinceLastUpdate = sf::Time::Zero;

        while (mWindow.isOpen()) {
            timeSinceLastUpdate += clock.restart();
            processEvents();

            while (timeSinceLastUpdate > TimePerFrame) {
                timeSinceLastUpdate -= TimePerFrame;
                processEvents();
                update(TimePerFrame);
            }

            render();
        }
    }

private:
    void handlePlayerInput(sf::Keyboard::Key key,
        bool isPressed)
    {
        if (key == sf::Keyboard::W)
            mIsMovingUp = isPressed;
        else if (key == sf::Keyboard::S)
            mIsMovingDown = isPressed;
        else if (key == sf::Keyboard::A)
            mIsMovingLeft = isPressed;
        else if (key == sf::Keyboard::D)
            mIsMovingRight = isPressed;
    }

    void processEvents()
    {
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

    void
    update(sf::Time& deltaTime)
    {
        sf::Vector2f movement(0.f, 0.f);
        if (mIsMovingUp) {
            speed.y -= acceleration.y;
            if (!mIsMovingDown || movement.y > 0) {
                movement.y += speed.y;
            }
        }

        if (mIsMovingDown) {
            speed.y += acceleration.y;
            if (!mIsMovingUp || movement.y < 0) {
                movement.y += speed.y;
            }
        }

        if (mIsMovingLeft) {
            speed.x -= acceleration.x;
            if (!mIsMovingRight || movement.x < 0) {
                movement.x += speed.x;
            }
        }

        if (mIsMovingRight) {
            speed.x += acceleration.x;
            if (!mIsMovingLeft || movement.x > 0) {
                movement.x += speed.x;
            }
        }

        if (speed.y != 0.f && (mIsMovingDown == mIsMovingUp)) {
            speed.y = speed.y + (speed.y > 0 ? -1.f : 1.f) * friction.y;
            movement.y += speed.y;
        }

        if (speed.x != 0.f && (mIsMovingLeft == mIsMovingRight)) {
            speed.x = speed.x + (speed.x > 0 ? -1.f : 1.f) * friction.x;
            movement.x += speed.x;
        }

        if (movement.x != 0.f || movement.y != 0) {
            float rotation = mPlayer.getRotation();
            float xBy60 = movement.x / 60;
            float yBy60 = movement.y / 60;

            rotation += xBy60 + yBy60;

            mPlayer.setRotation(rotation);
        }

        mPlayer.move(movement * deltaTime.asSeconds());
    }

    void render()
    {
        mWindow.clear();
        mWindow.draw(mPlayer);
        mWindow.display();
    }
};

int main()
{
    Game game;
    game.run();
}

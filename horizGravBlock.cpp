#include <SFML/Graphics.hpp>
#include <cmath>

#include "include/Game.hpp"
#include "include/HorizGravBlock.hpp"
#include "include/Graphics.hpp"

namespace
{
    const float max_speed = 0.6f;
    const float slowdown_factor = 0.85f;
}

HorizGravBlock::HorizGravBlock(int x, int y)
{
    texture.loadFromFile("./resources/grav_box.png");

    sprite = new sf::Sprite(texture);
    setPosition(x, y);

    position = new sf::Vector2f(x, y);
    velocity = new sf::Vector2f(0.f, 0.f);
    acceleration = new sf::Vector2f(0.f, 0.f);

    rect = new sf::IntRect(x, y, 16, 16);
}

HorizGravBlock::~HorizGravBlock()
{
    delete sprite;
    delete rect;
    delete position;
    delete velocity;
    delete acceleration;
}

void HorizGravBlock::setPosition(int x, int y)
{
    position->x = x;
    position->y = y;

    rect->left = x;
    rect->top = y;

    sprite->setPosition(x, y);
}

void HorizGravBlock::setPosition(sf::Vector2f &vec)
{
    position->x = vec.x;
    position->y = vec.y;

    rect->left = vec.x;
    rect->top = vec.y;

    sprite->setPosition(vec);
}

void HorizGravBlock::update(sf::Time time)
{
    velocity->x += acceleration->x * time.asMilliseconds();

    if (acceleration->x < 0.0f)
    {
        velocity->x = std::max(velocity->x, -max_speed);
    }
    else if (acceleration->x > 0.0f)
    {
        velocity->x = std::min(velocity->x, max_speed);
    }
    else
    {
        velocity->x *= slowdown_factor;
    }

    position->x += std::round(velocity->x * time.asMilliseconds());

    setPosition(*position);
}

void HorizGravBlock::draw(Graphics &graphics) const
{
    graphics.window->draw(*sprite);
}

void HorizGravBlock::startMovingLeft()
{
    acceleration->x = -Game::gravity_acc;
}
void HorizGravBlock::startMovingRight()
{
    acceleration->x = Game::gravity_acc;
}

void HorizGravBlock::stopMoving()
{
    acceleration->x = 0.0f;
}
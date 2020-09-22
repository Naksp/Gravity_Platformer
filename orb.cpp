#include <SFML/Graphics.hpp>
#include <iostream>

#include "./include/Orb.hpp"

Orb::Orb(int x, int y, Graphics &graphics)
{
    sprite = new AnimatedSprite("./resources/orb_spritesheet.png", 15, 10, 0, 0, 16, graphics);
    rect = new sf::IntRect(x, y, 16, 16);

    sprite->setPosition(x, y);
}

Orb::~Orb()
{
    delete sprite;
    delete rect;
}

sf::IntRect* Orb::getRect()
{
    return rect;
}

void Orb::update(sf::Time time)
{
    sprite->update(time.asMilliseconds());
}

void Orb::draw(Graphics &graphics)
{
    graphics.window->draw(*sprite);
}
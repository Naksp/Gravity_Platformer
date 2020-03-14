#include <SFML/Graphics.hpp>
#include <iostream>

#include "./include/Orb.hpp"

Orb::Orb(int x, int y, Graphics &graphics)
{
    sprite = new AnimatedSprite("./resources/orb.png", 1, 1, 0, 0, 16, graphics);
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

void Orb::draw(Graphics &graphics)
{
    graphics.window->draw(*sprite);
}
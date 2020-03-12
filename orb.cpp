#include <SFML/Graphics.hpp>
#include <iostream>

#include "./include/orb.hpp"

Orb::Orb(int x, int y, Graphics &graphics)
{
    sprite = new AnimatedSprite("./resources/orb.png", 1, 1, 0, 0, 16, graphics);
    rect = new sf::IntRect(0, 0, 16, 16);

    sprite->setPosition(x, y);
}

Orb::~Orb()
{
    delete sprite;
    delete rect;
}

void Orb::draw(Graphics &graphics)
{
    graphics.window->draw(*sprite);
}
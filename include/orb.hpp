#ifndef ORB_H
#define ORB_H

#include <SFML/Graphics.hpp>

#include "animatedSprite.hpp"
#include "graphics.hpp"

class Orb
{
    public:
        Orb(int x, int y, Graphics &graphics);
        ~Orb();

        void draw(Graphics &graphics);
        sf::IntRect* getRect();

    private:
        AnimatedSprite *sprite;
        sf::IntRect *rect;

};

#endif
#ifndef GRAVITY_OBJECT_H
#define GRAVITY_OBJECT_H

#include <SFML/Graphics.hpp>

#include "Graphics.hpp"
#include "Map.hpp"

/**
 * 
 * Abstract class for objects affected by gravity
 * 
 */
class GravityObject
{
    public:
        virtual ~GravityObject() {};

        virtual void setPosition(int x, int y) = 0;

        virtual void update(sf::Time time, Map &map) = 0;

        virtual void draw(Graphics &graphics) const = 0;

        virtual void reset() = 0;

        virtual sf::IntRect* getRect() = 0;

        virtual void startMovingUp() = 0;
        virtual void startMovingDown() = 0;
        virtual void startMovingLeft() = 0;
        virtual void startMovingRight() = 0;
        virtual void stopMoving() = 0;

};

#endif
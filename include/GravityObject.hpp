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
        virtual void setPosition(int x, int y) = 0;

        virtual void update(sf::Time time, Map &map) = 0;

        virtual void draw(Graphics &graphics) const = 0;

        sf::IntRect* getRect() { return rect; };
    
    private:
        /*
        virtual void startMovingUp() = 0;
        virtual void startMovingDown() = 0;
        virtual void startMovingLeft() = 0;
        virtual void startMovingRight() = 0;
        virtual void stopMoving() = 0;
        */
    private:
        sf::IntRect *rect;

        sf::Vector2f *position;
        sf::Vector2f *velocity;
        sf::Vector2f *acceleration;

};

#endif
#ifndef HORIZ_GRAV_BLOCK_H
#define HORIZ_GRAV_BLOCK_H

#include <SFML/Graphics.hpp>

#include "GravityObject.hpp"
#include "Map.hpp"

class HorizGravBlock : public GravityObject
{
    public:
        HorizGravBlock(int x, int y);
        ~HorizGravBlock();

        void setPosition(int x, int y);
        void setPosition(sf::Vector2f &vec);
        void update(sf::Time time, Map &map);
        void draw(Graphics &graphics) const;

        void startMovingLeft();
        void startMovingRight();
        void stopMoving();

        sf::IntRect* getRect() { return rect; };

    private:
        sf::IntRect leftCollision(int delta) const;
        sf::IntRect rightCollision(int delta) const;

    private:
        sf::Texture texture;
        sf::Sprite *sprite;
        sf::IntRect *rect;

        //int g_sign;
        sf::Vector2f *position;
        sf::Vector2f *velocity;
        sf::Vector2f *acceleration;
};

#endif
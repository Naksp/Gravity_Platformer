#ifndef VERT_GRAV_BLOCK_H
#define VERT_GRAV_BLOCK_H

#include <SFML/Graphics.hpp>

#include "GravityObject.hpp"
#include "Map.hpp"

class VertGravBlock : public GravityObject
{
    public:
        VertGravBlock(int x, int y);
        ~VertGravBlock();

        void setPosition(int x, int y);
        void setPosition(sf::Vector2f &vec);
        void update(sf::Time time, Map &map);
        void draw(Graphics &graphics) const;

        void startMovingUp();
        void startMovingDown();
        void stopMoving();

        sf::IntRect* getRect() { return rect; };

    private:
        sf::IntRect topCollision(int delta) const;
        sf::IntRect bottomCollision(int delta) const;

    private:
        sf::Texture texture;
        sf::Sprite *sprite;
        sf::IntRect *rect;

        sf::Vector2f *position;
        sf::Vector2f *velocity;
        sf::Vector2f *acceleration;
};

#endif //VERT_GRAV_BLOCK_H
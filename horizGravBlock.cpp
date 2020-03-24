#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "include/Game.hpp"
#include "include/Graphics.hpp"
#include "include/HorizGravBlock.hpp"
#include "include/Map.hpp"

namespace
{
    const float max_speed = 0.6f;
    const float slowdown_factor = 0.85f;

    struct CollisionData
    {
        bool collided;
        int row, col;
    };

    CollisionData setCollisionData(const sf::IntRect &rect, const Map &map)
    {
        CollisionData data = { false, 0, 0 };

        // Get colliding tiles
        std::vector<Map::CollisionTile> tiles(map.getCollidingTiles(rect));

        for (size_t i = 0; i < tiles.size(); i++)
        {
            // Check for wall tiles
            if (tiles[i].tile_type == Map::WALL_TILE)
            {
                // Set collision data
                data = { true, tiles[i].row, tiles[i].col };
                break;
            }
        }

        return data;
    }
}

HorizGravBlock::HorizGravBlock(int x, int y)
{
    texture.loadFromFile("./resources/grav_box.png");

    sprite = new sf::Sprite(texture);

    position = new sf::Vector2f(x, y);
    velocity = new sf::Vector2f(0.f, 0.f);
    acceleration = new sf::Vector2f(0.f, 0.f);

    // Set collision rect and initial position
    rect = new sf::IntRect(x, y, 16, 16);
    setPosition(x, y);
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

void HorizGravBlock::update(sf::Time time, Map &map)
{
    velocity->x += acceleration->x * time.asMilliseconds();
    
    const int delta = (int) round(velocity->x * time.asMilliseconds());

    if (delta < 0.0f)
    {
        CollisionData data = setCollisionData(leftCollision(delta), map);
        // Collision
        if (data.collided)
        {
            velocity->x = 0;
            position->x = data.col * Game::tile_size + Game::tile_size;
        }
        else
        {
            velocity->x = std::max(velocity->x, -max_speed);
            position->x += std::round(velocity->x * time.asMilliseconds());
        }
    }
    else if (delta > 0.0f)
    {
        CollisionData data = setCollisionData(rightCollision(delta), map);
        if (data.collided)
        {
            velocity->x = 0;
            position->x = data.col * Game::tile_size - rect->width;
        }
        else
        {
            velocity->x = std::min(velocity->x, max_speed);
            position->x += std::round(velocity->x * time.asMilliseconds());
        }
    }
    else
    {
        velocity->x *= slowdown_factor;
    }

    //position->x += std::round(velocity->x * time.asMilliseconds());

    setPosition(*position);
    //std::cout << position->x << ", " << position->y << std::endl;
}

void HorizGravBlock::draw(Graphics &graphics) const
{
    graphics.window->draw(*sprite);

    /*
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(rect->width, rect->height));
    rectangle.setPosition(*position);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(1);
    rectangle.setFillColor(sf::Color::Transparent);
    
    graphics.window->draw(rectangle);
    */
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

sf::IntRect HorizGravBlock::leftCollision(int delta) const
{
    assert(delta <= 0);
    return sf::IntRect(rect->left + delta,
                       rect->top,
                       rect->width / 2,
                       rect->height);
}

sf::IntRect HorizGravBlock::rightCollision(int delta) const
{
    assert(delta >= 0);
    return sf::IntRect(rect->left + rect->width / 2,
                       rect->top,
                       rect->width / 2 + delta,
                       rect->height);
}
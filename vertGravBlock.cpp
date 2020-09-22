#include <SFML/Graphics.hpp>
#include <cmath>

#include "include/Game.hpp"
#include "include/Graphics.hpp"
#include "include/Map.hpp"
#include "include/VertGravBlock.hpp"

namespace
{
    const float max_speed = 0.4f;
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

VertGravBlock::VertGravBlock(int x, int y)
{
    texture.loadFromFile("./resources/v_grav_block.png");

    sprite = new sf::Sprite(texture);

    position = new sf::Vector2f(x, y);
    velocity = new sf::Vector2f(0.f, 0.f);
    acceleration = new sf::Vector2f(0.f, 0.f);

    rect = new sf::IntRect(x, y, 16, 16);
    setPosition(x, y);
}

VertGravBlock::~VertGravBlock()
{
    delete sprite;
    delete rect;
    delete position;
    delete velocity;
    delete acceleration;
}

void VertGravBlock::setPosition(int x, int y)
{
    position->x = x;
    position->y = y;

    rect->left = x;
    rect->top = y;

    sprite->setPosition(x, y);
}

void VertGravBlock::setPosition(sf::Vector2f &vec)
{
    position->x = vec.x;
    position->y = vec.y;

    rect->left = vec.x;
    rect->top = vec.y;

    sprite->setPosition(vec.x, vec.y);
}

void VertGravBlock::update(sf::Time time, Map &map)
{
    velocity->y += acceleration->y * time.asMilliseconds();

    if (acceleration->y < 0.0f)
    {
        velocity->y = std::max(velocity->y, -max_speed);
    }
    else if (acceleration->y > 0.0f)
    {
        velocity->y = std::min(velocity->y, max_speed);
    }
    else
    {
        velocity->y *= slowdown_factor;
    }

    const int delta = (int) round(velocity->y * time.asMilliseconds());
    
    if (delta < 0.0f)
    {
        CollisionData data = setCollisionData(topCollision(delta), map);

        if (data.collided)
        {
            velocity->y = 0;
            position->y = data.row * Game::tile_size + Game::tile_size;
        }
        else
        {
            position->y += delta;
        }
    }
    else if (delta > 0.0f)
    {
        CollisionData data = setCollisionData(bottomCollision(delta), map);
        if (data.collided)
        {
            velocity->y = 0;
            position->y = data.row * Game::tile_size - rect->height;
        }
        else
        {
            position->y += delta;
        }
    }

    setPosition(*position);
}

void VertGravBlock::draw(Graphics &graphics) const
{
    graphics.window->draw(*sprite);
}

void VertGravBlock::startMovingUp()
{
    acceleration->y = -Game::gravity_acc;
}

void VertGravBlock::startMovingDown()
{
    acceleration->y = Game::gravity_acc;
}

void VertGravBlock::startMovingLeft()
{
    stopMoving();
}

void VertGravBlock::startMovingRight()
{
    stopMoving();
}

void VertGravBlock::stopMoving()
{
    acceleration->y = 0.0f;
}

sf::IntRect VertGravBlock::topCollision(int delta) const
{
    assert(delta <= 0);
    return sf::IntRect(rect->left,
                       rect->top + delta,
                       rect->width - 1, // Might be an issue down the road
                       rect->height / 2);
}

sf::IntRect VertGravBlock::bottomCollision(int delta) const
{
    assert(delta >= 0);
    return sf::IntRect(rect->left,
                       rect->top + rect->height / 2,
                       rect->width - 1,
                       rect->height / 2 + delta);
}
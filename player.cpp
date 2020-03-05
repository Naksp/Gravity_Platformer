#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "include/game.hpp"
#include "include/graphics.hpp"
#include "include/mapRect.hpp"
#include "include/player.hpp"

namespace
{
    // Walking constants
    const float slowDownFactor = 0.85f; // (original 0.8f)
    const float walkingAcceleration = 0.0012f; // (pixels / ms) / ms (original 0.0012f)
    const float maxSpeedX = 0.250f; // pixels / ms (original 0.325f)

    // Gravity constants
    const float gravity = 0.003f;
    const float max_fall_speed = 0.6f;

    // Jump constants
    const float jump_speed = 0.3f;
    const float jump_time = 200;
    const int jump_frame = 5;
    const int fall_frame = 1;

    // Collision constants
    //const sf::IntRect collision_x(6, 4, 7, 6);
    //const sf::IntRect collision_y(2, 0, 4, 16);
    const MapRect collision_x(6, 4, 7, 6);
    const MapRect collision_y(2, 0, 4, 16);

    struct CollisionData
    {
        bool collided;
        int row, col;
    };

    CollisionData setWallCollisionData(const Map &map, const MapRect &rect)
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

// Constructor
Player::Player(float x, float y, Graphics &graphics) :
    motion(STANDING),
    facing(RIGHT),
    on_ground(true)
{
    // Initialize sprites
    initSprites(graphics);

    // Set movement vectors
    position = new sf::Vector2f(x, y);
    velocity = new sf::Vector2f(0.0f, 0.0f);
    acceleration = new sf::Vector2f(0.0f, 0.0f);

    // Set initial sprite position
    sprites[getSpriteState()]->setPosition(*position);
}

// Destructor
Player::~Player()
{
    delete position;
    delete velocity;
    delete acceleration;
}

void Player::update(sf::Time time, Map map)
{
    // Update jump duration
    jump.update(time.asMilliseconds());

    updateX(time, map);
    updateY(time, map);

    sprites[getSpriteState()]->update(time.asMilliseconds());
    sprites[getSpriteState()]->setPosition(*position);
}

void Player::startMovingRight()
{
    facing = RIGHT;
    acceleration->x = walkingAcceleration;
}

void Player::startMovingLeft()
{
    facing = LEFT;
    acceleration->x = -walkingAcceleration;
}

void Player::stopMoving()
{
    acceleration->x = 0.0f;
}

 // Start jump and 
void Player::startJump()
{
    // Jump hasn't started
    if (onGround())
    {
        // Start jump
        jump.reset();
        velocity->y = -jump_speed;
        on_ground = false;
    }
    // Jump has started
    else if (velocity->y < 0.0f)
    {
        // Reactivate jump if fall hasn't started
        jump.reactivate();
    }
}

void Player::stopJump()
{
    jump.deactivate();
}

void Player::Jump::reset()
{
    time_remaining = jump_time;
    reactivate();
}

void Player::Jump::update(int elapsed_time)
{
    if (jump_active)
    {
        time_remaining -= elapsed_time;
        if (time_remaining <= 0)
        {
            jump_active = false;
        }
    }
}

// Set player position
void Player::setPosition(float x, float y)
{
    sprites[getSpriteState()]->setPosition(x, y);

}

// Set player postion (Vector2f)
void Player::setPosition(sf::Vector2f &vec)
{
    sprites[getSpriteState()]->setPosition(vec);
}

void Player::draw(Graphics &graphics)
{
    graphics.window->draw(*sprites[getSpriteState()]);
}

// Initialize Player sprites
void Player::initSprites(Graphics &graphics)
{
    sprites[SpriteState(STANDING, RIGHT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/santa.png", 15, 1, 0, 16, graphics));
    sprites[SpriteState(STANDING, LEFT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/santa.png", 15, 1, 0, 16, graphics));
    sprites[SpriteState(STANDING, LEFT)]->faceLeft(); 

    sprites[SpriteState(WALKING, RIGHT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/santa.png", 15, 6, 0, 16, graphics));
    sprites[SpriteState(WALKING, LEFT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/santa.png", 15, 6, 0, 16, graphics));
    sprites[SpriteState(WALKING, LEFT)]->faceLeft(); 

    sprites[SpriteState(JUMPING, RIGHT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/santa.png", 15, 1, jump_frame, 16, graphics));
    sprites[SpriteState(JUMPING, LEFT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/santa.png", 15, 1, jump_frame, 16, graphics));
    sprites[SpriteState(JUMPING, LEFT)]->faceLeft(); 

    sprites[SpriteState(FALLING, RIGHT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/santa.png", 15, 1, fall_frame, 16, graphics));
    sprites[SpriteState(FALLING, LEFT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/santa.png", 15, 1, fall_frame, 16, graphics));
    sprites[SpriteState(FALLING, LEFT)]->faceLeft(); 
}

// Get current Sprite State
Player::SpriteState Player::getSpriteState()
{
    if (onGround())
    {
        motion = acceleration->x == 0.0f ? STANDING : WALKING;
    }
    else
    {
        motion = velocity->y <= 0.0f ? JUMPING : FALLING;
    }
    
    return SpriteState(motion, facing);
}

MapRect Player::leftCollision(int delta) const{
    assert(delta <=0);
    return MapRect(position->x + collision_x.left() + delta,
                   position->y + collision_x.top(),
                   collision_x.width() / 2 - delta,
                   collision_x.height());
}

MapRect Player::rightCollision(int delta) const
{
    assert(delta >= 0);
    return MapRect(position->x + collision_x.left() / 2,
                   position->y + collision_x.top(),
                   collision_x.width() / 2 + delta,
                   collision_x.height());
}

MapRect Player::topCollision(int delta) const
{
    assert(delta <=0);
    return MapRect(position->x + collision_y.left(),
                   position->y + collision_y.top() + delta,
                   collision_y.width(),
                   collision_y.height() / 2 - delta);
}

// Bottom of sprite, upper Y value
MapRect Player::bottomCollision(int delta) const
{
    assert(delta >= 0);
    return MapRect(position->x + collision_y.left(),
                   position->y + collision_y.top() + collision_y.height() / 2,
                   collision_y.width(),
                   collision_y.height() / 2 + delta);
}


void Player::updateX(sf::Time time, Map map)
{
    position->x += round(velocity->x * time.asMilliseconds());

    // Update velocity
    velocity->x += acceleration->x * time.asMilliseconds();

    // Set x velocity cap
    if (acceleration->x < 0.0f)
    {
        velocity->x = std::max(velocity->x, -maxSpeedX);
    }
    else if (acceleration->x > 0.0f)
    {
        velocity->x = std::min(velocity->x, maxSpeedX);
    }
    else
    {
        // Slow x velocity
        velocity->x *= slowDownFactor;
    }

}

void Player::updateY(sf::Time time, Map map)
{

    // If jump is expired, set fall velocity
    if (!jump.active())
    {
        velocity->y = std::min(velocity->y + (gravity * time.asMilliseconds()), max_fall_speed);
    }

    // Calculate delta Y
    const int delta = (int) round(velocity->y * time.asMilliseconds());
    std::cout << "delta: "  << delta << std::endl;

    if (delta > 0)
    {
        // Check collision on bottom
        CollisionData data = setWallCollisionData(map, bottomCollision(delta));

        // React to collision
        if (data.collided)
        {
            position->y = data.row * Game::tile_size - collision_y.bottom();
            velocity->y = 0;
            on_ground = true;
        }
        else
        {
            position->y += delta;
            on_ground = false;
        }

        // Check collision in up direction
        data = setWallCollisionData(map, topCollision(0));

        if (data.collided)
        {
            std::cout << "*****" << std::endl;
            position->y = data.row * Game::tile_size + collision_y.height();
        }
    }
    else
    {
        CollisionData data = setWallCollisionData(map, topCollision(delta));

        if (data.collided)
        {
            position->y = data.row * Game::tile_size + collision_y.height();
            velocity->y = 0;
        }
        else
        {
            position->y += delta;
        }
        
    }
}


bool operator<(const Player::SpriteState &a, const Player::SpriteState &b)
{
    if (a.motion != b.motion)
    {
        return a.motion < b.motion;
    }
    if (a.facing != b.facing)
    {
        return a.facing < b.facing;
    }
    return false;
}
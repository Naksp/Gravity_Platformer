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
    const float gravity = 0.002f;
    const float max_fall_speed = 0.6f;

    // Jump constants
    const float jump_speed = 0.22f;
    const float jump_time = 300;
    const int jump_frame = 5;
    const int fall_frame = 1;

    // Collision constants
    const MapRect collision_x(0, 5, 16, 6);
    const MapRect collision_y(3, 0, 9, 16);

    struct CollisionData
    {
        bool collided;
        int row, col;
    };

    // Set CollisionData struct with corrent info based on delta
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

    // Set initial spawn point
    spawn_point = new sf::Vector2f(x, y);
    setSpawn(*spawn_point);
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
    facing = STILL;
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

void Player::setSpawn(sf::Vector2f &spawn)
{
    spawn_point->x = spawn.x;
    spawn_point->y = spawn.y;
}

void Player::respawn()
{
    setPosition(*spawn_point);
}

// Set player position
void Player::setPosition(float x, float y)
{
    position->x = x;
    position->y = y;

}

// Set player postion (Vector2f)
void Player::setPosition(sf::Vector2f &vec)
{
    position->x = vec.x;
    position->y = vec.y;
}

void Player::draw(Graphics &graphics)
{
    graphics.window->draw(*sprites[getSpriteState()]);
}

void Player::drawCollision(Graphics &graphics)
{
    sf::RectangleShape x_rectangle = collision_x.toRectangle(sf::Color::Blue);
    sf::RectangleShape y_rectangle = collision_y.toRectangle(sf::Color::Green);

    x_rectangle.setPosition(sf::Vector2f(position->x + collision_x.left(), position->y + collision_x.top()));
    y_rectangle.setPosition(sf::Vector2f(position->x + collision_y.left(), position->y + collision_y.top()));

    graphics.window->draw(x_rectangle);
    graphics.window->draw(y_rectangle);
}

// Initialize Player sprites
void Player::initSprites(Graphics &graphics)
{
    sprites[SpriteState(STANDING, RIGHT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 0, 16, graphics));
    sprites[SpriteState(STANDING, LEFT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 0, 16, graphics));
    sprites[SpriteState(STANDING, STILL)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 0, 16, graphics));

    sprites[SpriteState(WALKING, RIGHT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 1, 16, graphics));
    sprites[SpriteState(WALKING, LEFT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 2,16, graphics));

    sprites[SpriteState(JUMPING, RIGHT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 4, 16, graphics));
    sprites[SpriteState(JUMPING, LEFT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 5, 16, graphics));
    sprites[SpriteState(JUMPING, STILL)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 3, 16, graphics));

    sprites[SpriteState(FALLING, RIGHT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 7, 16, graphics));
    sprites[SpriteState(FALLING, LEFT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 8, 16, graphics));
    sprites[SpriteState(FALLING, STILL)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 6, 16, graphics));
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

// Get left collision rectangle
MapRect Player::leftCollision(int delta) const{
    assert(delta <=0);
    return MapRect(position->x + collision_x.left() + delta,
                   position->y + collision_x.top(),
                   collision_x.width() / 2 - delta,
                   collision_x.height());
}

// Get right collision rectangle
MapRect Player::rightCollision(int delta) const
{
    assert(delta >= 0);
    return MapRect(position->x + collision_x.left() + collision_x.width() / 2,
                   position->y + collision_x.top(),
                   collision_x.width() / 2 + delta,
                   collision_x.height());
}

// Get top collision rectangle
MapRect Player::topCollision(int delta) const
{
    assert(delta <=0);
    return MapRect(position->x + collision_y.left(),
                   position->y + collision_y.top() + delta,
                   collision_y.width(),
                   collision_y.height() / 2 - delta);
}

// Get bottom collision rectangle
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

    const int delta = (int) round(velocity->x * time.asMilliseconds());

    // Player is moveing right
    if (delta > 0)
    {
        // Check collision on right
        CollisionData data = setWallCollisionData(map, rightCollision(delta));

        // Collision
        if (data.collided)
        {
            position->x = data.col * Game::tile_size - collision_x.width();
            velocity->x = 0;
        }
        // No collision
        else
        {
            position->x += delta;
        }

        // Check on left
        data = setWallCollisionData(map, leftCollision(0));
        if (data.collided)
        {
            position->x = data.col * Game::tile_size + collision_x.width() + collision_x.left();    
        }
    }
    // Player is moving left
    else
    {
        // Check collision on left
        CollisionData data = setWallCollisionData(map, leftCollision(delta));

        // Collision
        if (data.collided)
        {
            position->x = data.col * Game::tile_size + collision_x.width();
            velocity->x = 0;
        }
        // No collision
        else
        {
            position->x += delta;
        }
        
        // Check on right
        data = setWallCollisionData(map, rightCollision(0));
        if (data.collided)
        {
            position->x = data.col * Game::tile_size - collision_x.right();
        }
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

    // Player is moving down
    if (delta > 0)
    {
        // Check collision on bottom
        CollisionData data = setWallCollisionData(map, bottomCollision(delta));

        // Collision
        if (data.collided)
        {
            position->y = data.row * Game::tile_size - collision_y.bottom();
            velocity->y = 0;
            on_ground = true;
        }
        // No collision
        else
        {
            position->y += delta;
            on_ground = false;
        }

        // Check in up direction
        data = setWallCollisionData(map, topCollision(0));
        if (data.collided)
        {
            position->y = data.row * Game::tile_size + collision_y.height();
        }
    }
    // Player is moving up
    else
    {
        CollisionData data = setWallCollisionData(map, topCollision(delta));

        if (data.collided)
        {
            jump.endJump();
            position->y = data.row * Game::tile_size + collision_y.height();
            velocity->y = 0;
        }
        else
        {
            position->y += delta;
        }

        data = setWallCollisionData(map, bottomCollision(0));
        if (data.collided)
        {
            position->y = data.row * Game::tile_size - collision_y.bottom();
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
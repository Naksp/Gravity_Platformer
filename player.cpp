#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "include/Game.hpp"
#include "include/Graphics.hpp"
#include "include/MapRect.hpp"
#include "include/Player.hpp"

namespace
{
    // Walking constants
    const float slowDownFactor = 0.85f; // (original 0.8f)
    const float walkingAcceleration = 0.0012f; // (pixels / ms) / ms (original 0.0012f)
    const float maxSpeedX = 0.175f; // pixels / ms (original 0.325f)

    // Gravity constants
    const float max_fall_speed = 0.4f;

    // Jump constants
    const float jump_speed = 0.18f;
    const float jump_time = 200;
    const int jump_frame = 5;
    const int fall_frame = 1;

    // Collision constants
    const MapRect collision_x(0, 3, 16, 9);
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
    facing(FRONT),
    gravity(DOWN),
    g_sign(1),
    on_ground(true)
{
    // Initialize sprites
    initSprites(graphics);

    // Set movement vectors
    position = new sf::Vector2f(x, y);
    velocity = new sf::Vector2f(0.0f, 0.0f);
    acceleration = new sf::Vector2f(0.0f, 0.0f);

    rect = new sf::IntRect(x, y, 16, 16);

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

void Player::update(sf::Time time, Map &map)
{
    // Update jump duration
    jump.update(time.asMilliseconds());

    if (gravity == UP || gravity == DOWN)
    {
        updateX(time, map);
        updateY(time, map); 
    }
    else
    {
        updateX2(time, map); 
        updateY2(time, map);
    }

    rect->left = position->x;
    rect->top = position->y;
    

    sprites[getSpriteState()]->update(time.asMilliseconds());
    sprites[getSpriteState()]->setPosition(*position);
}

void Player::startMovingRight()
{
    if (gravity == DOWN || gravity == UP)
    {
        facing = (gravity == DOWN || gravity == LEFT) ? FRONT : BACK;
        acceleration->x = walkingAcceleration;
    }
}

void Player::startMovingLeft()
{
    if (gravity == DOWN || gravity == UP)
    {
        facing = (gravity == DOWN || gravity == LEFT) ? BACK : FRONT;
        acceleration->x = -walkingAcceleration;
    }
}

void Player::startMovingUp()
{
    if (gravity == LEFT || gravity == RIGHT)
    {
        facing = (gravity == DOWN || gravity == LEFT) ? BACK : FRONT;
        acceleration->x = -walkingAcceleration;
    }
}

void Player::startMovingDown()
{
    if (gravity == LEFT || gravity == RIGHT)
    {
        facing = (gravity == DOWN || gravity == LEFT) ? FRONT : BACK;
        acceleration->x = walkingAcceleration;
    }
}

void Player::stopMoving()
{
    acceleration->x = 0.0f;
}

// Gravity Functions
void Player::setGravityLeft()
{
    setRotation(90.f);
    gravity = LEFT;
    g_sign = -1;
    on_ground = false;
}

void Player::setGravityRight()
{
    setRotation(270.f);
    gravity = RIGHT;
    g_sign = 1;
    on_ground = false;
}

void Player::setGravityUp()
{
    setRotation(180.f);
    gravity = UP;
    g_sign = -1;
    on_ground = false;
}

void Player::setGravityDown()
{
    setRotation(0.f);
    gravity = DOWN;
    g_sign = 1;
    on_ground = false;
}


// Jump Functions
void Player::startJump()
{
    // Jump hasn't started
    if (onGround())
    {
        // Start jump
        jump.reset();
        //*velocity->y = -jump_speed * g_sign;
        if (gravity == DOWN || gravity == UP)
        {
            velocity->y = -jump_speed * g_sign;
        }
        else
        {
            velocity->x = -jump_speed * g_sign;
        }

        
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

void Player::setSpawn(int x, int y)
{
    spawn_point->x = x;
    spawn_point->y = y;
}

void Player::respawn()
{
    velocity->x = 0;
    velocity->y = 0;
    acceleration->x = 0;
    setPosition(*spawn_point);
}

// Set player position
void Player::setPosition(float x, float y)
{
    position->x = x;
    position->y = y;
    sprites[getSpriteState()]->setPosition(*position);

}

// Set player postion (Vector2f)
void Player::setPosition(sf::Vector2f &vec)
{
    position->x = vec.x;
    position->y = vec.y;
    sprites[getSpriteState()]->setPosition(*position);
}

void Player::setRotation(float angle)
{
    std::map<SpriteState, boost::shared_ptr<AnimatedSprite> >::iterator it;
    for (it = sprites.begin(); it != sprites.end(); it++)
    {
        it->second->setRotation(angle);
    }
}

void Player::draw(Graphics &graphics)
{
    graphics.window->draw(*sprites[getSpriteState()]);
}

void Player::drawCollision(Graphics &graphics)
{
    sf::RectangleShape x_rectangle = collision_x.toRectangle(sf::Color::Blue);
    sf::RectangleShape y_rectangle = collision_y.toRectangle(sf::Color::Blue);

    /*
    sf::RectangleShape rect_rectangle;
    rect_rectangle.setSize(sf::Vector2f(16, 16));
    rect_rectangle.setPosition(*position);
    rect_rectangle.setOutlineColor(sf::Color::Red);
    rect_rectangle.setOutlineThickness(1);
    rect_rectangle.setFillColor(sf::Color::Transparent);
    */


    x_rectangle.setPosition(sf::Vector2f(position->x + collision_x.left(), position->y + collision_x.top()));
    y_rectangle.setPosition(sf::Vector2f(position->x + collision_y.left(), position->y + collision_y.top()));

    graphics.window->draw(x_rectangle);
    graphics.window->draw(y_rectangle);
    //graphics.window->draw(rect_rectangle);
}

sf::IntRect* Player::getRect()
{
    return rect;
}

std::vector<sf::IntRect>* Player::getCollisionRects()
{
    std::vector<sf::IntRect>* rects = new std::vector<sf::IntRect>();
    

    // Get left, right collision rects
    if (frame_delta_x > 0)
    {
        rects->push_back(*leftCollision(0).getRect());
        rects->push_back(*rightCollision(frame_delta_x).getRect());
    }
    else 
    {
        rects->push_back(*leftCollision(frame_delta_x).getRect());
        rects->push_back(*rightCollision(0).getRect());
    }

    // Get bottom, top collision rects
    if (frame_delta_y > 0)
    {
        rects->push_back(*bottomCollision(frame_delta_y).getRect());
        rects->push_back(*topCollision(0).getRect());
    }
    else
    {
        rects->push_back(*bottomCollision(0).getRect());
        rects->push_back(*topCollision(frame_delta_y).getRect());
    }

    return rects;
}

void Player::collideX()
{
    velocity->x = 0;
}

void Player::collideY()
{
    velocity->y = 0;
}

void Player::landOnGround()
{
    on_ground = true;
}

// Initialize Player sprites
void Player::initSprites(Graphics &graphics)
{
    sprites[SpriteState(STANDING, FRONT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 1, 16, graphics));
    sprites[SpriteState(STANDING, BACK)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 2, 16, graphics));

    sprites[SpriteState(WALKING, FRONT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 1, 16, graphics));
    sprites[SpriteState(WALKING, BACK)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 2,16, graphics));

    sprites[SpriteState(JUMPING, FRONT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 4, 16, graphics));
    sprites[SpriteState(JUMPING, BACK)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 5, 16, graphics));

    sprites[SpriteState(FALLING, FRONT)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 7, 16, graphics));
    sprites[SpriteState(FALLING, BACK)] = boost::shared_ptr<AnimatedSprite>( new AnimatedSprite("./resources/robo_ball.png", 3, 6, 0, 8, 16, graphics));
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

std::string Player::debugSpriteState() const
{
    std::string state;
    switch (motion)
    {
    case STANDING:
        state = "STANDING";
        break;
    case JUMPING:
        state = "JUMPING";
        break;
    case WALKING:
        state = "WALKING";
        break; 
    default:
        break;
    }

    if (facing == FRONT)
    {
        state += ", FRONT";
    }
    else 
    {
        state += ", BACK";
    }

    return state;
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

void Player::updateX(sf::Time time, Map &map)
{
    // Update velocity
    velocity->x += acceleration->x * time.asMilliseconds();

    // Set x velocity cap
    if (acceleration->x < 0.0f)
    {
        //velocity->x = std::max(velocity->x, -maxSpeedX);
        if (velocity->x < -maxSpeedX)
        {
            velocity->x *= slowDownFactor;
        }
    }
    else if (acceleration->x > 0.0f)
    {
        //velocity->x = std::min(velocity->x, maxSpeedX);
        if (velocity->x > maxSpeedX)
        {
            velocity->x *= slowDownFactor;
        }
    }
    else
    {
        // Slow x velocity
        velocity->x *= slowDownFactor;
    }

    const int delta = (int) round(velocity->x * time.asMilliseconds());

    // Player is moving right
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

    frame_delta_x = delta;
}

void Player::updateY(sf::Time time, Map &map)
{

    // If jump is expired, set fall velocity
    if (!jump.active())
    {
        if (gravity == DOWN)
        {
            velocity->y = std::min(velocity->y + (g_sign * Game::gravity_acc * time.asMilliseconds()), max_fall_speed);
        }
        else
        {
            velocity->y = std::max(velocity->y + (g_sign * Game::gravity_acc * time.asMilliseconds()), -max_fall_speed);
        }
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
            if (gravity == DOWN)
            {
                on_ground = true;
            }
        }
        // No collision
        else
        {
            position->y += delta;
            if (gravity == DOWN)
            {
                on_ground = false;
            }
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
            //jump.endJump();
            position->y = data.row * Game::tile_size + collision_y.height();
            velocity->y = 0;
            if (gravity == UP)
            {
                on_ground = true;
            }
        }
        else
        {
            position->y += delta;
            if (position->y < 0)
            {
                position->y = 0;
            }
            if (gravity == UP)
            {
                on_ground = false;
            }
        }

        data = setWallCollisionData(map, bottomCollision(0));
        if (data.collided)
        {
            position->y = data.row * Game::tile_size - collision_y.bottom();
        }
        
    }
    frame_delta_y = delta;
}

void Player::updateX2(sf::Time time, Map &map)
{
    // If jump is expired, set fall velocity
    if (!jump.active())
    {
        if (gravity == RIGHT)
        {
            velocity->x = std::min(velocity->x + (g_sign * Game::gravity_acc * time.asMilliseconds()), max_fall_speed);
        }
        else
        {
            velocity->x = std::max(velocity->x + (g_sign * Game::gravity_acc * time.asMilliseconds()), -max_fall_speed);
        }
    }

    const int delta = (int) round(velocity->x * time.asMilliseconds());

    // Player moving right
    if (delta > 0)
    {
        CollisionData data = setWallCollisionData(map, rightCollision(delta));

        if (data.collided)
        {
            position->x = data.col * Game::tile_size - collision_x.width();
            velocity->x = 0;
            if (gravity == RIGHT)
            {
                on_ground = true;
            }
        }
        else
        {
            position->x += delta;
            if (gravity == RIGHT)
            {
                on_ground = false;
            }
        }

        // Check on left
        data = setWallCollisionData(map, leftCollision(0));
        if (data.collided)
        {
            position->x = data.col * Game::tile_size + collision_x.width() + collision_x.left();
        }
    }
    // Player moving left
    else
    {
        CollisionData data = setWallCollisionData(map, leftCollision(delta));

        if (data.collided)
        {
            position->x = data.col * Game::tile_size + collision_x.width();
            velocity->x = 0;
            if (gravity == LEFT)
            {
                on_ground = true;
            }
        }
        else
        {
            position->x += delta;
            if (gravity == UP)
            {
                on_ground = false;
            }
        }

        // Check on right

        data = setWallCollisionData(map, rightCollision(0));
        if (data.collided)
        {
            position->x = data.col * Game::tile_size - collision_x.right();
        }
    }

    frame_delta_x = delta;
}

void Player::updateY2(sf::Time time, Map &map)
{
    velocity->y += acceleration->x * time.asMilliseconds();

    if (acceleration->x < 0.0f)
    {
        //velocity->y = std::max(velocity->y, -maxSpeedX);
        if (velocity->y < -maxSpeedX)
        {
            velocity->y *= slowDownFactor;
        }
    }
    else if (acceleration->x > 0.0f)
    {
        //velocity->y = std::min(velocity->y, maxSpeedX);
        if (velocity->y < -maxSpeedX)
        {
            velocity->y *= slowDownFactor;
        }
    }
    else
    {
        velocity->y *= slowDownFactor;
    }

    const int delta = (int) round(velocity->y * time.asMilliseconds());

    // Player moving down
    if (delta > 0)
    {

        // Check collision on bottom
        CollisionData data = setWallCollisionData(map, bottomCollision(delta));

        if (data.collided)
        {
            position->y = data.row * Game::tile_size - collision_y.bottom();
            velocity->y = 0;
        }
        else
        {
            position->y += delta;
        }

        // Check top
        data = setWallCollisionData(map, leftCollision(0));
        if (data.collided)
        {
            position->y = data.row * Game::tile_size + collision_y.height();
        }
        
    }
    // Player is moving up
    else
    {
        // Check top
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
        
        // Check bottom
        data = setWallCollisionData(map, bottomCollision(0));
        if (data.collided)
        {
            position->y = data.row * Game::tile_size - collision_y.bottom();
        }
    }

    frame_delta_y = delta;
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
#ifndef PLAYER_H
#define PLAYER_H

#include <boost/shared_ptr.hpp>

#include "animatedSprite.hpp"
#include "level.hpp"
#include "map.hpp"
#include "mapRect.hpp"

class Player
{
    public:
        // Constructor
        Player(float x, float y, Graphics &graphics);

        // Destructor
        ~Player();

        // Update player
        void update(sf::Time time, Level &level);

        // Movement functions
        void startMovingRight();
        void startMovingLeft();
        void startMovingUp();
        void startMovingDown();
        void stopMoving();

        // Gravity change
        void setGravityLeft();
        void setGravityRight();
        void setGravityUp();
        void setGravityDown();

        void startJump();
        void stopJump();

        void setSpawn(sf::Vector2f &spawn);
        void respawn();

        // Set position of Player
        void setPosition(float x, float y);
        void setPosition(sf::Vector2f &vec);

        void setRotation(float angle);

        void draw(Graphics &graphics);
        void drawCollision(Graphics &graphics);

    private:
        /**
         * Update X direction of movement vectors
         * 
         * @param time  Time of last frame
         * @param map   Map of sprites
         */
        void updateX(sf::Time time, Map &map);

        /**
         * Update Y direction of movement vectors
         * 
         * @param time  Time of last frame
         * @param map   Map of sprites
         */
        void updateY(sf::Time time, Map &map);

        /**
         * Updatas X direction for horizontal gravity
         * 
         * @param time  Time of last frame
         * @param map   Map of sprites
         */
        void updateX2(sf::Time time, Map &map);

        /**
         * Updatas Y direction for horizontal gravity
         * 
         * @param time  Time of last frame
         * @param map   Map of sprites
         */
        void updateY2(sf::Time time, Map &map);

    // Private variables
    private:
        sf::Vector2f *spawn_point;

        // Player state
        enum Motion {STANDING, WALKING, JUMPING, FALLING};
        enum Facing {BACK, FRONT};
        enum Gravity {LEFT, RIGHT, UP, DOWN};
        //enum Facing {LEFT, RIGHT, STILL};

        // State struct for defining movement
        struct SpriteState
        {
            SpriteState(Motion motion=STANDING, Facing facing=FRONT) :
                motion(motion),
                facing(facing) {}

            Motion motion;
            Facing facing;
        };
        friend bool operator<(const SpriteState& a, const SpriteState& b);

        // Jump Class
        struct Jump
        {
            public:
                Jump() : time_remaining(0), jump_active(false) {}

                void update(int elaplsed_time);
                void reset();

                void reactivate() { jump_active = time_remaining > 0; };
                void deactivate() { jump_active = false; };
                bool active() const { return jump_active; };
                void endJump() { time_remaining = 0; };

            private:
                int time_remaining;
                bool jump_active;
        };
        
        // Jump object
        Jump jump;
        Motion motion;

        // Initialize sprite map
        void initSprites(Graphics &graphics);

        // Get currest SpriteState
        SpriteState getSpriteState();

        // Collision functions
        MapRect leftCollision(int delta) const;
        MapRect leftCollision2(int delta) const;
        MapRect rightCollision(int delta) const;
        MapRect topCollision(int delta) const;
        MapRect bottomCollision(int delta) const;

        bool onGround() const { return on_ground; }

        // Current facing
        Facing facing;

        // Gravity vars
        Gravity gravity;
        int g_sign;

        // Movement vectors
        sf::Vector2f *position;
        sf::Vector2f *velocity;
        sf::Vector2f *acceleration;
        bool on_ground;

        // Sprite map
        std::map<SpriteState, boost::shared_ptr<AnimatedSprite> > sprites;
};

#endif
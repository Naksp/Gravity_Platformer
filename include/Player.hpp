#ifndef PLAYER_H
#define PLAYER_H

#include <boost/shared_ptr.hpp>

#include "AnimatedSprite.hpp"
#include "Map.hpp"
#include "MapRect.hpp"

class Player
{
    public:
        // Constructor
        Player(float x, float y, Graphics &graphics);

        // Destructor
        ~Player();

        // Update player
        void update(sf::Time time, Map &map);

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
        void setSpawn(int x, int y);
        void respawn();
        bool isAlive() { return alive; };

        // Set position of Player
        void setPosition(float x, float y);
        void setPosition(sf::Vector2f &vec);

        sf::Vector2f* getPosition() const { return position; };
        sf::Vector2f* getVelocity() const { return velocity; };
        sf::Vector2f* getAcceleration() const { return acceleration; };

        /**
         * Sets rotation of sprite, adjusting offset to keep same position
         * 
         * @param angle Angle of target rotation
         */
        void setRotation(float angle);

        void draw(Graphics &graphics);
        void drawCollision(Graphics &graphics);

        sf::IntRect* getRect();

        /**
         * Gets collision rects (delta rects) of player for current update cycle
         * 
         * @return Vector of IntRects. Order is: { left, right, bottom, top }
         */
        std::vector<sf::IntRect>* getCollisionRects();

        void collideX();
        void collideY();

        void pushX();
        void pushY();

        /**
         * Set player on_ground to true
         * Used to correctly set player movement state
         */
        void landOnGround();

        /**
         * For debugging spritestate
         * @return sprite state as string
         */
        std::string debugSpriteState() const;

    private:
        /**
         * Update X direction for vertical gravity
         * 
         * @param time  Time of last frame
         * @param map   Map of sprites
         */
        void updateX(sf::Time time, Map &map);

        /**
         * Update Y direction for vertical gravity
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

        bool checkCrushX();
        bool checkCrushY();


    // Private variables
    private:
        sf::Vector2f *spawn_point;

        bool alive;

        // Player state
        enum Motion {STANDING, WALKING, JUMPING, FALLING};
        enum Facing {BACK, FRONT};
        enum Gravity {UP, DOWN, LEFT, RIGHT};
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
        MapRect rightCollision(int delta) const;
        MapRect topCollision(int delta) const;
        MapRect bottomCollision(int delta) const;

        sf::IntRect *rect;

        /**
         * Used to determine player movement state
         * @return true if player in currently on ground
         */
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

        bool beingPushedX;
        bool beingPushedY;

        int frame_delta_x, frame_delta_y;

        // Sprite map
        std::map<SpriteState, boost::shared_ptr<AnimatedSprite> > sprites;
};

#endif
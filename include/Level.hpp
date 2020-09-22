#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

#include "Graphics.hpp"
#include "HorizGravBlock.hpp"
#include "Map.hpp"
#include "Orb.hpp"
#include "Player.hpp"
#include "VertGravBlock.hpp"
#include "GravityObject.hpp"

class Level
{
    public:
        Level(const std::string level_path, int level_num, Player &player, Graphics &graphics);
        ~Level();

        Map* getMap();

        // Gravity change
        void setGravityLeft(Player &player);
        void setGravityRight(Player &player);
        void setGravityUp(Player &player);
        void setGravityDown(Player &player);

        int getGravity();

        void start(Player &player);

        int update(Player &player, sf::Time time);

        void draw(Graphics &graphics);

        /**
         * Resets level
         * Respawns player and GravObjects to initial coords
         * 
         * @param player Player to respawn
         */
        void reset(Player &player);

    private:
        void updateGravBlocks(Player &player, sf::Time time);

    public:
        // UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3
        enum Gravity {UP, DOWN, LEFT, RIGHT};

    private:
        const int level_num;
        Map* map;
        std::vector<sf::Texture> textures;

        // Current gravity direction
        Gravity gravity;
        Gravity initialGravity;

        sf::Vector2f *player_spawn;

        // Player collision rect
        std::vector<sf::IntRect>* player_collision;
    
        // Vector of all GravObjects in level
        std::vector<std::unique_ptr<GravityObject>> *grav_objects;

        Orb *orb;
};

#endif
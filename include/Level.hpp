#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

#include "Graphics.hpp"
#include "HorizGravBlock.hpp"
#include "Map.hpp"
#include "Orb.hpp"
#include "Player.hpp"

class Level
{
    public:
        Level(const std::string level_path, Player &player, Graphics &graphics);
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

    private:
        void checkMapCollision(sf::IntRect &rect, Map &map);

    public:
        enum Gravity {UP, DOWN, LEFT, RIGHT};

    private:
        Map* map;
        std::vector<sf::Texture> textures;

        Gravity gravity;

        sf::Vector2f *player_spawn;
    
        std::vector<HorizGravBlock*> *h_grav_blocks;

        Orb *orb;
};

#endif
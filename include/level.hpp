#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

#include "graphics.hpp"
#include "map.hpp"
#include "orb.hpp"
#include "player.hpp"

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

        int update(Player &player);

        void draw(Graphics &graphics);

    public:
        enum Gravity {UP, DOWN, LEFT, RIGHT};

    private:
        Map* map;
        std::vector<sf::Texture> textures;

        Gravity gravity;

        sf::Vector2f *player_spawn;

        Orb* orb;
};

#endif
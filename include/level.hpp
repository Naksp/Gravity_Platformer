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
        Level(const std::string level_path, Graphics &graphics);
        ~Level();

        Map* getMap();

        int update(Player &player);

        void draw(Graphics &graphics);

    private:
        Map* map;
        std::vector<sf::Texture> textures;

        Orb* orb;
};

#endif
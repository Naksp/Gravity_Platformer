#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

#include "graphics.hpp"
#include "map.hpp"
#include "orb.hpp"

class Level
{
    public:
        Level(const std::string map_path, const std::string texture_path, Graphics &graphics);
        Level(const std::string level_path, Graphics &graphics);
        ~Level();

        Map* getMap();

        void draw(Graphics &graphics);

    private:
        void loadTestLevel(Graphics &graphics);

    private:
        Map* map;
        std::vector<sf::Texture> textures;

        Orb* orb;
};

#endif
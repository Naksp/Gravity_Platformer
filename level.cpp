#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "./include/level.hpp"
#include "./include/orb.hpp"
#include "./include/player.hpp"

Level::Level(const std::string level_path, Graphics &graphics)
{
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Loading level from " << level_path << "..." << std::endl;
    // Open file stream
    std::ifstream level_file;
    level_file.open(level_path);

    if (level_file.is_open())
    {
        // Get next line of file
        std::string line;
        while (std::getline(level_file, line))
        {
            // Read map data
            if (line.compare("[MAP]") == 0)
            {
                // Create new map
                std::getline(level_file, line);
                map = Map::loadMapFile(line, graphics);
            }
            // Read orb data
            else if (line.compare("[ORB]") == 0)
            {
                int x, y;
                std::getline(level_file, line);

                // Get coordinates for orb
                std::stringstream line_stream(line);
                line_stream >> x >> y;

                // Create new orb
                orb = new Orb(x, y, graphics);
            }
        }
    }

    level_file.close();
    std::cout << "--------------------------------------------" << std::endl;
}

Level::~Level()
{
    delete map;
}

Map* Level::getMap()
{
    return map;
}

int Level::update(Player &player)
{
    if (player.getRect()->intersects(*orb->getRect()))
    {
        //std::cout << "YOU WON!" << std::endl;
        return 1;
    }
    
    return 0;
}

void Level::draw(Graphics &graphics)
{
    map->draw(graphics);
    orb->draw(graphics);
}
#include <SFML/Graphics.hpp>

#include "./include/level.hpp"

Level::Level(const std::string map_path, const std::string texture_path, Graphics &graphics)
{
    map = Map::loadMapFile(map_path, graphics);
}

Level::~Level()
{
    delete map;
}

Map* Level::getMap()
{
    return map;
}

void Level::draw(Graphics &graphics)
{
    map->draw(graphics);
}

/*
void Level::loadTestLevel(Graphics &graphics, const std::string map_path,)
{
    map = Map::loadMapFile(map_path)
}
*/
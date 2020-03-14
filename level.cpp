#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "./include/Level.hpp"
#include "./include/Orb.hpp"
#include "./include/Player.hpp"

Level::Level(const std::string level_path, Player &player, Graphics &graphics)
{
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Loading level from " << level_path << "..." << std::endl;
    // Open file stream
    std::ifstream level_file;
    level_file.open(level_path);

    bool map_load = false, orb_load = false;
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

                map_load = true;
            }
            else if (line.compare("[PLAYER]") == 0)
            {
                int x, y;
                std::getline(level_file, line);

                // Get coordinates for orb
                std::stringstream line_stream(line);
                line_stream >> x >> y;

                player_spawn = new sf::Vector2f(x, y);

                std::cout << "Player at " << x << ", " << y << std::endl;
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
                std::cout << "Orb at " << x << ", " << y << std::endl;

                orb_load = true;
            }
            else if (line.compare("[GRAVITY]") == 0)
            {
                int grav;
                std::getline(level_file, line);
                std::stringstream line_stream(line);
                line_stream >> grav;

                switch (grav)
                {
                case UP:
                    gravity = UP;
                    break;
                case DOWN:
                    gravity = DOWN;
                    break;
                case LEFT:
                    gravity = LEFT;
                    break;
                case RIGHT:
                    gravity = RIGHT;
                    break;
                default:
                    break;
                }
            }
        }
    }
    else
    {
        std::cerr << "ERROR: Failed to open " << level_path << "!" << std::endl;
    }
    if (!map_load)
    {
        std::cerr << "ERROR: Failed to load map!" << std::endl;
    }
    if (!orb_load)
    {
        std::cerr << "ERROR: Failed to create orb!" << std::endl;
    }

    level_file.close();
    std::cout << "--------------------------------------------" << std::endl;
}

Level::~Level()
{
    delete map;
    delete orb;
    delete player_spawn;
}

Map* Level::getMap()
{
    return map;
}

void Level::setGravityLeft(Player &player)
{
    gravity = LEFT;
    player.setGravityLeft();
}
void Level::setGravityRight(Player &player)
{
    gravity = RIGHT;
    player.setGravityRight();
}
void Level::setGravityUp(Player &player)
{
    gravity = UP;
    player.setGravityUp();
}
void Level::setGravityDown(Player &player)
{
    gravity = DOWN;
    player.setGravityDown();
}

int Level::getGravity()
{
    return gravity;
}

void Level::start(Player &player)
{
    switch (gravity)
    {
    case UP:
        player.setGravityUp();
        break;
    case DOWN:
        player.setGravityDown();
        break;
    case LEFT:
        player.setGravityLeft();
        break;
    case RIGHT:
        player.setGravityRight();
        break;
    default:
        break;
    }

    player.setSpawn(*player_spawn);
}

int Level::update(Player &player)
{
    if (player.getRect()->intersects(*orb->getRect()))
    {
        return 1;
    }
    
    return 0;
}

void Level::draw(Graphics &graphics)
{
    map->draw(graphics);
    orb->draw(graphics);
    //std::cout << orb->getRect()->left << "," << orb->getRect()->top << std::endl;
}
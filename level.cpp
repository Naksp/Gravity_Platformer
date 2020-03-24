#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "./include/Game.hpp"
#include "./include/HorizGravBlock.hpp"
#include "./include/Level.hpp"
#include "./include/Orb.hpp"
#include "./include/Player.hpp"

Level::Level(const std::string level_path, Player &player, Graphics &graphics)
{
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Loading level from " << level_path << "..." << std::endl;

    h_grav_blocks = new std::vector<HorizGravBlock*>();

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
            else if (line.compare("[HGBLOCKS]") == 0)
            {
                int x, y;
                std::getline(level_file, line);

                std::stringstream line_stream(line);
                line_stream >> x >> y;

                h_grav_blocks->push_back(new HorizGravBlock(x * Game::tile_size, y * Game::tile_size));
            }
            // Set initial gravity
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

        // Check if anything failed
        if (!map_load)
        {
            std::cerr << "ERROR: Failed to load map!" << std::endl;
        }
        if (!orb_load)
        {
            std::cerr << "ERROR: Failed to create orb!" << std::endl;
        }
    }
    else
    {
        std::cerr << "ERROR: Failed to open " << level_path << "!" << std::endl;
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

    for (uint i = 0; i < h_grav_blocks->size(); i++)
    {
        h_grav_blocks->at(i)->startMovingLeft();
    }
}
void Level::setGravityRight(Player &player)
{
    gravity = RIGHT;
    player.setGravityRight();

    for (uint i = 0; i < h_grav_blocks->size(); i++)
    {
        h_grav_blocks->at(i)->startMovingRight();
    }
}
void Level::setGravityUp(Player &player)
{
    gravity = UP;
    player.setGravityUp();

    for (uint i = 0; i < h_grav_blocks->size(); i++)
    {
        h_grav_blocks->at(i)->stopMoving();
    }
}
void Level::setGravityDown(Player &player)
{
    gravity = DOWN;
    player.setGravityDown();

    for (uint i = 0; i < h_grav_blocks->size(); i++)
    {
        h_grav_blocks->at(i)->stopMoving();
    }
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

int Level::update(Player &player, sf::Time time)
{
    if (player.getRect()->intersects(*orb->getRect()))
    {
        return 1;
    }
    // TODO add collision detection
    for (uint i = 0; i < h_grav_blocks->size(); i++)
    {
        h_grav_blocks->at(i)->update(time, *map);
    }
    
    return 0;
}

void Level::draw(Graphics &graphics)
{
    map->draw(graphics);
    for (uint i = 0; i < h_grav_blocks->size(); i++)
    {
        h_grav_blocks->at(i)->draw(graphics);
    }
    orb->draw(graphics);
    //std::cout << orb->getRect()->left << "," << orb->getRect()->top << std::endl;
}

void Level::checkMapCollision(sf::IntRect &rect, Map &map)
{

}
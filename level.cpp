#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

#include "./include/Game.hpp"
#include "./include/HorizGravBlock.hpp"
#include "./include/Level.hpp"
#include "./include/MapRect.hpp"
#include "./include/Orb.hpp"
#include "./include/Player.hpp"
#include "./include/VertGravBlock.hpp"

Level::Level(const std::string level_path, int level_num, Player &player, Graphics &graphics) :
    level_num(level_num)
{
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Loading level " << level_num << " from " << level_path << "..." << std::endl;

    grav_objects = new std::vector<std::unique_ptr<GravityObject>>();

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
            // Player spawn data
            else if (line.compare("[PLAYER]") == 0)
            {
                int x, y;
                std::getline(level_file, line);

                std::stringstream line_stream(line);
                line_stream >> x >> y;

                player_spawn = new sf::Vector2f(x * Game::tile_size, y * Game::tile_size);
                player.setSpawn(*player_spawn);

                std::cout << "Player at " << x * Game::tile_size << ", " << y * Game::tile_size << std::endl;
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
                orb = new Orb(x * Game::tile_size, y * Game::tile_size, graphics);
                std::cout << "Orb at " << x * Game::tile_size << ", " << y * Game::tile_size << std::endl;

                orb_load = true;
            }
            // Horizontal Gravity Blocks
            else if (line.compare("[HGBLOCKS]") == 0)
            {
                int x, y;
                while (std::getline(level_file, line) && line.compare("") != 0)
                {
                    std::stringstream line_stream(line);
                    line_stream >> x >> y;

                    grav_objects->push_back(std::make_unique<HorizGravBlock>(x * Game::tile_size, y * Game::tile_size));
                }
            }
            // Vertical Gravity Blocks
            else if (line.compare("[VGBLOCKS]") == 0)
            {
                int x, y;
                while (std::getline(level_file, line) && line.compare("") != 0)
                {
                    std::stringstream line_stream(line);
                    line_stream >> x >> y;

                    grav_objects->push_back(std::make_unique<VertGravBlock>(x * Game::tile_size, y * Game::tile_size));
                }
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
                    initialGravity = UP;
                    break;
                case DOWN:
                    gravity = DOWN;
                    initialGravity = DOWN;
                    break;
                case LEFT:
                    gravity = LEFT;
                    initialGravity = LEFT;
                    break;
                case RIGHT:
                    gravity = RIGHT;
                    initialGravity = RIGHT;
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

    for (uint i = 0; i < grav_objects->size(); i++)
    {
        grav_objects->at(i)->startMovingLeft();
    }
}
void Level::setGravityRight(Player &player)
{
    gravity = RIGHT;
    player.setGravityRight();

    for (uint i = 0; i < grav_objects->size(); i++)
    {
        grav_objects->at(i)->startMovingRight();
    }
}
void Level::setGravityUp(Player &player)
{
    gravity = UP;
    player.setGravityUp();

    for (uint i = 0; i < grav_objects->size(); i++)
    {
        grav_objects->at(i)->startMovingUp();
    }
}
void Level::setGravityDown(Player &player)
{
    gravity = DOWN;
    player.setGravityDown();

    for (uint i = 0; i < grav_objects->size(); i++)
    {
        grav_objects->at(i)->startMovingDown();
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
    player.update(time, *map);
    if (!player.isAlive())
    {
        return -1;
    }
    player_collision = player.getCollisionRects();

    // Update grav blocks
    // This checks player and map collision
    updateGravBlocks(player, time);

    orb->update(time);

    // Check if level is won
    if (player.getRect()->intersects(*orb->getRect()))
    {
        return 1;
    }
    
    return 0;
}

void Level::draw(Graphics &graphics)
{
    map->draw(graphics);

    for (uint i = 0; i < grav_objects->size(); i++)
    {
        grav_objects->at(i)->draw(graphics);
    }

    orb->draw(graphics);
    if (graphics.debugState())
    for (uint i = 0; i < player_collision->size(); i++)
    {
        graphics.drawRect(player_collision->at(i), sf::Color::Green);
    }
}

void Level::reset(Player &player)
{
    player.respawn();

    for (auto& curr_block : *grav_objects)
    {
        curr_block->reset();
    }

    switch (initialGravity)
    {
    case UP:
        setGravityUp(player);
        break;
    case DOWN:
        setGravityDown(player);
        break;
    case LEFT:
        setGravityLeft(player);
        break;
    case RIGHT:
        setGravityRight(player);
        break;
    default:
        break;
    }
}

void Level::updateGravBlocks(Player &player, sf::Time time)
{
    for (auto& curr_block : *grav_objects)
    {
        curr_block->update(time, *map);

        // Check relative y collisions first, then relative x
        if (gravity == UP || gravity == DOWN)
        {
            for (int j = player_collision->size() - 1; j >= 0; j--)
            {
                // Check for collisions
                if (player_collision->at(j).intersects(*curr_block.get()->getRect()))
                {
                    switch (j)
                    {
                    // Left Collision
                    case 0 :
                        if (player.getVelocity()->x < 0)
                        {
                            player.collideX();
                        }
                        player.setPosition(curr_block->getRect()->left + curr_block->getRect()->width, player.getPosition()->y);
                        if (curr_block->getVelocity()->x > 0)
                        {
                            player.pushX();
                        }
                        player_collision = player.getCollisionRects();
                        break;
                    // Right Collision
                    case 1 :
                        if (player.getVelocity()->x > 0)
                        {
                            player.collideX();
                        }
                        player.setPosition(curr_block->getRect()->left - Game::tile_size, player.getPosition()->y);
                        if (curr_block->getVelocity()->x < 0)
                        {
                            player.pushX();
                        }
                        player_collision = player.getCollisionRects();
                        break;
                    // Bottom collision
                    case 2 :
                        player.collideY();
                        if (gravity == DOWN)
                        {
                            player.landOnGround();
                        }
                        player.setPosition(player.getPosition()->x, curr_block->getRect()->top - Game::tile_size);
                        if (curr_block->getVelocity()->y > 0)
                        {
                            player.pushY();
                        }
                        player_collision = player.getCollisionRects();
                        break;
                    // Top collision
                    case 3 :
                        player.collideY();
                        if (gravity == UP)
                        {
                            player.landOnGround();
                        }
                        player.setPosition(player.getPosition()->x, curr_block->getRect()->top + Game::tile_size);
                        if (curr_block->getVelocity()->y < 0)
                        {
                            player.pushY();
                        }
                        player_collision = player.getCollisionRects();
                        break;
                    
                    default:
                        break;
                    }
                }
            }

        }

        // Gravity is Left or Right
        else
        {
            for (uint j = 0; j < player_collision->size(); j++)
            {
                // Check for collisions
                if (player_collision->at(j).intersects(*curr_block->getRect()))
                {
                    switch (j)
                    {
                    // Left Collision
                    case 0 :
                        player.collideX();
                        if (gravity == LEFT)
                        {
                            player.landOnGround();
                        }
                        player.setPosition(curr_block->getRect()->left + curr_block->getRect()->width, player.getPosition()->y);
                        if (curr_block->getVelocity()->x > 0)
                        {
                            player.pushX();
                        }
                        player_collision = player.getCollisionRects();
                        break;
                    // Right Collision
                    case 1 :
                        player.collideX();
                        if (gravity == RIGHT)
                        {
                            player.landOnGround();
                        }
                        player.setPosition(curr_block->getRect()->left - Game::tile_size, player.getPosition()->y);
                        if (curr_block->getVelocity()->x < 0)
                        {
                            player.pushX();
                        }
                        player_collision = player.getCollisionRects();
                        break;
                    // Bottom collision
                    case 2 :
                        player.collideY();
                        player.setPosition(player.getPosition()->x, curr_block->getRect()->top - Game::tile_size);
                        if (curr_block->getVelocity()->y > 0)
                        {
                            player.pushY();
                        }
                        player_collision = player.getCollisionRects();
                        break;
                    case 3 :
                        player.collideY();
                        player.setPosition(player.getPosition()->x, curr_block->getRect()->top + Game::tile_size);
                        if (curr_block->getVelocity()->y < 0)
                        {
                            player.pushY();
                        }
                        player_collision = player.getCollisionRects();
                        break;
                    
                    default:
                        break;
                    }
                }
            }
        }
    }
}
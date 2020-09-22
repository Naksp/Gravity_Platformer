#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include "include/Game.hpp"
#include "include/Graphics.hpp"
//#include "include/HorizGravBlock.hpp"
#include "include/Map.hpp"
#include "include/MapRect.hpp"

using boost::shared_ptr;
using std::vector;

namespace
{
    const int num_rows = 15;
    const int num_cols = 20;
}

Map* Map::loadMapFile(const std::string file_path, Graphics &graphics)
{
    // Create new map
    Map *map = new Map();
    map->tiles = vector<vector<Tile> >(
        num_rows, vector<Tile>(
            num_cols, Tile()));

    // TODO make this a function to load from file maybe
    map->textures = new vector<sf::Texture>();
    map->textures->push_back(graphics.loatTexture("./resources/metal_tile.png"));
    map->textures->push_back(graphics.loatTexture("./resources/spikes.png"));
    shared_ptr<sf::Sprite> wall_sprite(new sf::Sprite(map->textures->at(0)));
    shared_ptr<sf::Sprite> spike_sprite(new sf::Sprite(map->textures->at(1)));

    Tile wall_tile(WALL_TILE, wall_sprite);
    Tile spike_tile(SPIKE_TILE, spike_sprite);

    // Stream vars
    std::ifstream map_file;
    map_file.open(file_path);
    std::string line;

    int row = 0;
    assert(map_file.is_open());
    if (map_file.is_open())
    {
        std::cout << "Loading map from " << file_path << std::endl;
        // Get next row
        while (std::getline(map_file, line) && row < num_rows)
        {
            std::stringstream line_stream(line);

            int col = 0;
            int val;
            // Get next col
            while (line_stream >> val)
            {
                if (val == 1)
                {
                    map->tiles[row][col] = wall_tile;
                }
                else if (val == 2)
                {
                    map->tiles[row][col] = spike_tile;
                }
                col++;
            }
            row++;
        }

        map_file.close();
    }

    return map;
}

std::vector<Map::CollisionTile> Map::getCollidingTiles(const sf::IntRect &rect) const
{
    const int first_row = rect.top / Game::tile_size;
    const int last_row = ((rect.top + rect.height) / Game::tile_size) >= num_rows ? num_rows-1  // TODO Maybe find a more elegant solution to this
                       : ((rect.top + rect.height) / Game::tile_size);
    const int first_col = rect.left / Game::tile_size;
    const int last_col = (rect.left + rect.width) / Game::tile_size;

    /*
    std::cout << rect.left << "\t" << rect.top << "\t" << rect.width << "\t" << rect.height << std::endl;

    std::cout << "First row: " << first_row << "\tLast row: " << last_row
              << "\tFirst col: " << first_col << "\tLast col: " << last_col << std::endl;
    */

    std::vector<CollisionTile> collision_tiles;
    for (int row = first_row; row <= last_row; row++)
    {
        for (int col = first_col; col <= last_col; col++)
        {
            collision_tiles.push_back(CollisionTile(row, col, tiles[row][col].tile_type));
        }
    }

    return collision_tiles;

}

std::vector<Map::CollisionTile> Map::getCollidingTiles(const MapRect &rect) const
{
    const int first_row = rect.top() / Game::tile_size;
    const int last_row = (rect.bottom() / Game::tile_size) >= num_rows ? num_rows-1 : (rect.bottom() / Game::tile_size); // TODO Maybe find a more elegant solution to this
    const int first_col = rect.left() / Game::tile_size;
    const int last_col = rect.right() / Game::tile_size;

    std::vector<CollisionTile> collision_tiles;
    for (int row = first_row; row <= last_row; row++)
    {
        for (int col = first_col; col <= last_col; col++)
        {
            collision_tiles.push_back(CollisionTile(row, col, tiles[row][col].tile_type));
        }
    }

    return collision_tiles;

}

void Map::draw(Graphics &graphics)
{
    for (size_t row = 0; row < tiles.size(); row++)
    {
        for (size_t col = 0; col < tiles[row].size(); col++)
        {
            if (tiles[row][col].sprite)
            {
                tiles[row][col].sprite->setPosition(col * 16, row * 16);
                graphics.window->draw(*tiles[row][col].sprite);
            }
        }
    }
}
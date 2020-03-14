#ifndef MAP_H
#define MAP_H

#include <boost/shared_array.hpp>
#include <vector>

#include "HorizGravBlock.hpp"
#include "MapRect.hpp"

class Map
{
    public:
        enum TileType
        {
            AIR_TILE,
            WALL_TILE
        };
        struct CollisionTile {
            CollisionTile(int row, int col, TileType tile_type) :
                row(row),
                col(col),
                tile_type(tile_type) {};

            int row, col;
            TileType tile_type;
        };

        static Map* createTestMap(Graphics &graphics);
        
        static Map* loadMapFile(const std::string file_path, std::vector<HorizGravBlock*> &grav_boxes, Graphics &graphics);

        //std::vector<CollisionTile> getCollidingTiles(const sf::IntRect &rect) const;
        std::vector<CollisionTile> getCollidingTiles(const MapRect &rect) const;

        //void update(int elapsed_time);
        void draw(Graphics &graphics);

    private:
        struct Tile
        {
            Tile(TileType tile_type=AIR_TILE, boost::shared_ptr<sf::Sprite> sprite=boost::shared_ptr<sf::Sprite>()) :
            tile_type(tile_type),
            sprite(sprite) {};

            TileType tile_type;
            boost::shared_ptr<sf::Sprite> sprite;

        };
        std::vector<std::vector<Tile> > tiles;
        sf::Texture texture;
        std::vector<sf::Texture> textures;

};

#endif
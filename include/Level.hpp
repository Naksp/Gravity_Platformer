#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

#include "Graphics.hpp"
#include "HorizGravBlock.hpp"
#include "Map.hpp"
#include "Orb.hpp"
#include "Player.hpp"
#include "VertGravBlock.hpp"
#include "GravityObject.hpp"

class Level
{
    public:
        Level(const std::string level_path, int level_num, Player &player, Graphics &graphics);
        ~Level();

        Map* getMap();

        // Gravity change
        void setGravityLeft(Player &player);
        void setGravityRight(Player &player);
        void setGravityUp(Player &player);
        void setGravityDown(Player &player);

        int getGravity();

        void start(Player &player);

        int update(Player &player, sf::Time time);

        void draw(Graphics &graphics);

    private:
        //void updateHGravBlocks(Player &player, sf::Time time);

        void updateGravBlocks(Player &player, sf::Time time);

    public:
        enum Gravity {UP, DOWN, LEFT, RIGHT};

    private:
        const int level_num;
        Map* map;
        std::vector<sf::Texture> textures;

        Gravity gravity;

        sf::Vector2f *player_spawn;
        std::vector<sf::IntRect>* player_collision;
    
        //std::vector<HorizGravBlock*> *h_grav_blocks;
        //std::vector<VertGravBlock*> *v_grav_blocks;

        std::vector<std::unique_ptr<GravityObject>> *grav_objects;
        //std::unique_ptr<GravityObject> g_block;
        GravityObject* g_block;

        Orb *orb;
};

#endif
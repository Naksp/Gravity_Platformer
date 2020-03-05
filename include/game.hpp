#ifndef GAME_H
#define GAME_H

#include "graphics.hpp"
#include "input.hpp"
#include "map.hpp"
#include "player.hpp"

struct Game
{
    public:
        // Constructor
        Game();

        // Desctructor
        ~Game();

    private:
        // Handle input
        void processInput(Input input);

        // Update game
        void update(sf::Time time);

        // Draw frame to screen
        void draw();
        
        // Main event loop for game
        void eventLoop();

    public:
        static const int tile_size = 16;
    // Private variables
    private:
        // Grahics
        Graphics *graphics;

        // Player
        Player *player;

        Map *map;
};

#endif
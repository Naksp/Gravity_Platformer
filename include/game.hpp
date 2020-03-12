#ifndef GAME_H
#define GAME_H

#include "graphics.hpp"
#include "input.hpp"
#include "level.hpp"
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
        void initPlayer();
        void initPlayer(int x_pos, int y_pos);

        void initLevels();

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

        // Levels
        Level *level;
        int current_level;

        enum Gamestate {STARTING, RUNNING, WON};

        Gamestate state;
};

#endif
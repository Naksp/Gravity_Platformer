#ifndef GAME_H
#define GAME_H

#include "Graphics.hpp"
#include "Input.hpp"
#include "Level.hpp"
#include "Map.hpp"
#include "Player.hpp"

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

        void loadNextLevel();
        void loadLevel(int level);

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
        enum Gravity {UP, DOWN, LEFT, RIGHT};

    // Private variables
    private:
        // Debug
        bool debug;
        // Grahics
        Graphics *graphics;

        // Player
        Player *player;

        // Levels
        //Level *level;
        std::vector<Level*> levels;
        u_int current_level;

        enum Gamestate {STARTING, RUNNING, WON};

        Gamestate state;

        Gravity gravity;
};

#endif
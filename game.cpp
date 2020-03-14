#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "include/game.hpp"
#include "include/graphics.hpp"
#include "include/input.hpp"
#include "include/map.hpp"
#include "include/player.hpp"

// Constructor
Game::Game()
{
    debug = false;

    state = STARTING;

    // Set up Grahics
    graphics = new Graphics(1280, 960, 60, "Simple Platformer");

    // Set up Player
    initPlayer();

    // Set up levels
    initLevels();

    // Start Event Loop
    eventLoop();
}

// Destructor
Game::~Game()
{
    delete graphics;
    delete player;
}

void Game::initPlayer()
{
    player = new Player(tile_size, graphics->view->getSize().y - tile_size, *graphics);
}

void Game::initPlayer(int x_pos, int y_pos)
{
    player = new Player(x_pos, y_pos, *graphics);
}

void Game::initLevels()
{
    std::fstream level_list_file;
    level_list_file.open("./levels/level_list");

    if (level_list_file.is_open())
    {

        std::string level_path;
        while (std::getline(level_list_file, level_path))
        {
            levels.push_back(new Level(level_path, *player, *graphics));
        }
    }

    current_level = 0;
}

void Game::loadNextLevel()
{
    if (current_level == levels.size()-1)
    {
        graphics->window->close();
        return;
    }
    loadLevel(++current_level);
}

void Game::loadLevel(int level)
{
    if (level < 0)
    {
        return;
    }
    current_level = level;
    state = RUNNING;
    levels[current_level]->start(*player);
    player->respawn();
}

// Process keyboard input and move player
void Game::processInput(Input input)
{
    if (state == RUNNING)
    {
        if (input.isKeyHeld(sf::Keyboard::A) && input.isKeyHeld(sf::Keyboard::D))
        {
            player->stopMoving();
        }
        else if (input.isKeyHeld(sf::Keyboard::A))
        {
            player->startMovingLeft();
        }
        else if (input.isKeyHeld(sf::Keyboard::D))
        {
            player->startMovingRight();
        }
        else if (input.isKeyHeld(sf::Keyboard::W))
        {
            player->startMovingUp();
        }
        else if (input.isKeyHeld(sf::Keyboard::S))
        {
            player->startMovingDown();
        }
        else
        {
            player->stopMoving();
        }
    }
    
    // Jump
    if (input.wasKeyPressed(sf::Keyboard::Space))
    {
        if (state == RUNNING)
        {
            player->startJump();
        }
        else if (state == WON)
        {
            loadNextLevel();
        }
    }
    else if (input.wasKeyReleased(sf::Keyboard::Space))
    {
        player->stopJump();
    }

    // Player Rotation
    if (input.wasKeyPressed(sf::Keyboard::J))
    {
        //player->setGravityLeft();
        levels[current_level]->setGravityLeft(*player);
    }
    else if (input.wasKeyPressed(sf::Keyboard::K))
    {
        //player->setGravityDown();
        levels[current_level]->setGravityDown(*player);
    }
    else if (input.wasKeyPressed(sf::Keyboard::L))
    {
        //player->setGravityRight();
        levels[current_level]->setGravityRight(*player);
    }
    else if (input.wasKeyPressed(sf::Keyboard::I))
    {
        //player->setGravityUp();
        levels[current_level]->setGravityUp(*player);
    }

}

// Update game state
void Game::update(sf::Time frameTime)
{
    // TODO: Change this value back
    //sf::Time testTime = sf::milliseconds(16);
    //player->update(testTime, *map);
    player->update(frameTime, *levels[current_level]->getMap());
    if (levels[current_level]->update(*player))
    {
        state = WON;
    }
}

// Draw game objects
void Game::draw()
{
    graphics->clear();
    levels[current_level]->draw(*graphics);
    player->draw(*graphics);
    if (state == WON)
    {
        graphics->winMessage(50, 50);
    }
    if (debug)
    {
        player->drawCollision(*graphics);
        graphics->debugInfo(*player->getPosition(), levels[current_level]->getGravity());
    }
    graphics->display();
}

// Main event loop for game
void Game::eventLoop()
{
    Input input;
    sf::Event event;

    sf::Clock frameClock;
    sf::Time frameTime;

    sf::sleep(sf::milliseconds(16));

    state = RUNNING;

    // Event Loop
    while (graphics->window->isOpen())
    {
        input.beginNewFrame();
        frameTime = frameClock.restart();

        // Handle events
        while (graphics->window->pollEvent(event))
        {
            switch (event.type)
            {
                // Key Pressed
                case sf::Event::KeyPressed:
                    input.keyDownEvent(event);
                    if (input.wasKeyPressed(sf::Keyboard::Escape))
                    {
                        graphics->window->close();
                    }
                    if (input.wasKeyPressed(sf::Keyboard::R))
                    {
                        player->respawn();
                    }
                    if (input.wasKeyPressed(sf::Keyboard::Tilde))
                    {
                        debug = !debug;
                    }
                    if (input.wasKeyPressed(sf::Keyboard::Num2))
                    {
                        loadNextLevel();
                    }
                    if (input.wasKeyPressed(sf::Keyboard::Num1))
                    {
                        loadLevel(current_level-1);
                    }
                    break;

                // Key Released
                case sf::Event::KeyReleased:
                    input.keyUpEvent(event);
                    break;

                // Game closed
                case sf::Event::Closed:
                    graphics->window->close();
                    break;

                default:
                    break;
            }
        }

        processInput(input);
        update(frameTime);
        draw();
    }
}
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "include/game.hpp"
#include "include/graphics.hpp"
#include "include/input.hpp"
#include "include/map.hpp"
#include "include/player.hpp"

// Constructor
Game::Game()
{
    // Set up Grahics
    graphics = new Graphics(1280, 960, 60, "Test 3");

    // Set up Player
    initPlayer();

    // Start Event Loop
    eventLoop();
}

// Destructor
Game::~Game()
{
    delete graphics;
    delete player;
    delete map;
}

void Game::initPlayer()
{
    player = new Player(0, graphics->view->getSize().y - 16, *graphics);
}

void Game::initPlayer(int x_pos, int y_pos)
{
    player = new Player(x_pos, y_pos, *graphics);
}

// Process keyboard input and move player
void Game::processInput(Input input)
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
    else
    {
        player->stopMoving();
    }
    
    // Jump
    if (input.wasKeyPressed(sf::Keyboard::Space))
    {
        player->startJump();
    }
    else if (input.wasKeyReleased(sf::Keyboard::Space))
    {
        player->stopJump();
    }
}

// Update game state
void Game::update(sf::Time frameTime)
{
    // TODO: Change this value back
    //sf::Time testTime = sf::milliseconds(16);
    //player->update(testTime, *map);
    player->update(frameTime, *map);
}

// Draw game objects
void Game::draw()
{
    graphics->clear();
    map->draw(*graphics);
    player->draw(*graphics);
    //player->drawCollision(*graphics);
    graphics->display();
}

// Main event loop for game
void Game::eventLoop()
{
    Input input;
    sf::Event event;

    sf::Clock frameClock;
    sf::Time frameTime;

    //map = Map::createTestMap(*graphics); 
    map = Map::loadMapFile("./maps/test_map", *graphics);
    sf::sleep(sf::milliseconds(16));

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
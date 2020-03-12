#include <SFML/Graphics.hpp>
#include <iostream>

#include "include/graphics.hpp"

namespace
{
    const sf::Color background_color = sf::Color(50,50,50,255);
}

Graphics::Graphics(int width, int height, int frame_rate, const std::string &header) :
    width(width),
    height(height),
    frame_rate(frame_rate)
{
    // Set up window
    window = new sf::RenderWindow(sf::VideoMode(width, height), header);
    window->setFramerateLimit(frame_rate);

    view = new sf::View(sf::Vector2f(160, 120), sf::Vector2f(320, 240));
    window->setView(*view);

    if (!font.loadFromFile("./fonts/Rubik-Medium.ttf"))
    {
        std::cout << "CAN'T LOAD FONT FILE" << std::endl;
    }
    win_message.setFont(font);
    win_message.setString("WIN!");
    win_message.setCharacterSize(32);
    win_message.setStyle(sf::Text::Regular);
    win_message.setColor(sf::Color::Yellow);
}

Graphics::~Graphics()
{
    delete window;
}

sf::Texture Graphics::loatTexture(const std::string &file_path)
{
    if (sprite_sheets.count(file_path) == 0)
    {
        std::cout << "Loading texture from " << file_path << " ..." << std::endl;
        if (!(sprite_sheets[file_path].loadFromFile(file_path)))
        {
            std::cerr << "ERROR: Failed to load texture!" << std::endl;
        }
    }
    else
    {
        std::cout << "Loading " << file_path << " from cache." << std::endl;
    }

    return sprite_sheets[file_path];
}

void Graphics::clear()
{
    window->clear(background_color);
}

void Graphics::display()
{
    window->display();
}

void Graphics::winMessage(int x, int y)
{
    win_message.setPosition(x, y);
    window->draw(win_message);
}
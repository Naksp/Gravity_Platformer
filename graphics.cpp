#include <SFML/Graphics.hpp>
#include <iostream>

#include "include/Graphics.hpp"

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

    view = new sf::View(sf::Vector2f(160, 120), sf::Vector2f(352, 272));
    window->setView(*view);

    if (!font.loadFromFile("./fonts/Rubik-Medium.ttf"))
    {
        std::cout << "CAN'T LOAD FONT FILE" << std::endl;
    }
    win_message.setFont(font);
    win_message.setString("WIN!");
    win_message.setCharacterSize(32);
    win_message.setStyle(sf::Text::Regular);
    win_message.setFillColor(sf::Color::Yellow);

    debug_text.setFont(font);
    debug_text.setCharacterSize(10);
    debug_text.setStyle(sf::Text::Regular);
    debug_text.setFillColor(sf::Color::Red);
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

void Graphics::debugInfo(sf::Vector2f &pos, int grav)
{
    std::string debug_message;
    std::string x_pos = std::to_string(pos.x);
    std::string y_pos = std::to_string(pos.y);
    std::string gravity = std::to_string(grav);

    debug_message = "Position:\t" + x_pos + ", " + y_pos + "\n" +
                    "Gravity: " + gravity;
            
    debug_text.setString(debug_message);
    debug_text.setPosition(0, 0);

    window->draw(debug_text);
}
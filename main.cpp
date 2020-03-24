#include <SFML/Graphics.hpp>
#include <iostream>

#include "include/Game.hpp"

// TODO: Add java-doc comments for all files

// Main function
int main(int argc, char *argv[])
{
    std::cout << "+++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    std::cout << "Starting simple_platformer...";

    // Set starting level
    uint start_level = 0;
    if (argc > 1)
    {
        start_level = (int) std::atoi(argv[1]);
        std::cout << " From level " << start_level;
    }

    std::cout << std::endl;
    std::cout << "+++++++++++++++++++++++++++++++++++++++++++" << std::endl;

    // Start game
    Game game(start_level);

    std::cout << "+++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    std::cout << "Exiting simple_platformer..." << std::endl;
    std::cout << "+++++++++++++++++++++++++++++++++++++++++++" << std::endl;

    return 0;
}
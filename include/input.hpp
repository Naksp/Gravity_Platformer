#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <SFML/Graphics.hpp>

class Input
{
    public:
        // Reset key data
        void beginNewFrame();
        
        // Key press/release event handlers
        void keyDownEvent(const sf::Event &event);
        void keyUpEvent(const sf::Event &event);

        // Key state functions
        bool wasKeyPressed(sf::Keyboard::Key key);
        bool wasKeyReleased(sf::Keyboard::Key key);
        bool isKeyHeld(sf::Keyboard::Key key);

    private:
        // Key state maps
        std::map<sf::Keyboard::Key, bool> held_keys;
        std::map<sf::Keyboard::Key, bool> pressed_keys;
        std::map<sf::Keyboard::Key, bool> released_keys;
};

#endif
#include "include/input.hpp"

void Input::beginNewFrame()
{
    pressed_keys.clear();
    released_keys.clear();
}

void Input::keyDownEvent(const sf::Event &event)
{
    pressed_keys[event.key.code] = true;
    held_keys[event.key.code] = true;
}

void Input::keyUpEvent(const sf::Event &event)
{
    released_keys[event.key.code] = true;
    held_keys[event.key.code] = false;
}

bool Input::wasKeyPressed(sf::Keyboard::Key key)
{
    return pressed_keys[key];
}

bool Input::wasKeyReleased(sf::Keyboard::Key key)
{
    return released_keys[key];
}

bool Input::isKeyHeld(sf::Keyboard::Key key)
{
    return held_keys[key];
}
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "include/mapRect.hpp"

MapRect::MapRect(int left, int top, int width, int height)
{
    int_rect = new sf::IntRect(left, top, width, height);

}

MapRect::~MapRect()
{
    delete int_rect;
}

int MapRect::left() const
{
    return int_rect->left;
}

int MapRect::right() const
{
    return int_rect->left + int_rect->width;
}

int MapRect::top() const
{
    return int_rect->top;
}

int MapRect::bottom() const
{
    return int_rect->top + int_rect->height;
}

int MapRect::width() const
{
    return int_rect->width;
}

int MapRect::height() const
{
    return int_rect->height;
}

sf::RectangleShape MapRect::toRectangle(sf::Color outlineColor) const
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(int_rect->width, int_rect->height));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineThickness(1);
    rectangle.setOutlineColor(outlineColor);

    return rectangle;
}
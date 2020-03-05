#include <SFML/Graphics/Rect.hpp>

#include "include/mapRect.hpp"

MapRect::MapRect(int left, int right, int width, int height)
{
    int_rect = new sf::IntRect(left, right, width, height);

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
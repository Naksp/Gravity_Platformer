#ifndef MAP_RECT_H
#define MAP_RECT_H

#include <SFML/Graphics/Rect.hpp>

struct MapRect : public sf::IntRect
{
    public:
        MapRect(int left, int top, int width, int height);
        ~MapRect();

        int left() const;
        int right() const;
        int top() const;
        int bottom() const;

        int width() const;
        int height() const;

        sf::RectangleShape toRectangle(sf::Color outlineColor) const;

    private:
        const sf::IntRect *int_rect;

};

#endif
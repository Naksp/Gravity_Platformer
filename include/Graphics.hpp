#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>

class Graphics
{
    public:
        // Constructor
        Graphics(int width, int height, int frame_rate, const std::string &header);

        // Destructor
        ~Graphics();

        sf::Texture loatTexture(const std::string &file_path);

        // Clear window
        void clear();
        
        // Display window
        void display();

        void winMessage(int x, int y);

        void debugInfo(sf::Vector2f &pos, sf::Vector2f &vel, sf::Vector2f &acc, int grav);

        void drawRect(sf::IntRect &rect, sf::Color color) const;

        // Return debug state
        bool debugState() const;

        void setDebug(bool state);

        void toggleDebug();

    public:
        // Window object
        sf::RenderWindow *window;
        sf::View *view;

    private:
        // Window properties
        int width, height;
        int frame_rate;

        std::map<std::string, sf::Texture> sprite_sheets; 

        sf::Text win_message;
        sf::Font font;

        bool debug;
         
        sf::Text debug_text;

};

#endif
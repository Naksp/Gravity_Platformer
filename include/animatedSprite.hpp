#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <SFML/Graphics.hpp>

#include "graphics.hpp"

class AnimatedSprite : public sf::Drawable
{
    public:
        /**
         * Constructor for AnimatedSprite class
         * 
         * @param file_path Path to texture file
         * @param fps Frames/second of animation
         * @param num_frames Number of frames in animation
         * @param start_frame Frame number of first frame of animation
         * @param tile_size Size in pixels of each sprite on sprite sheet
         * @param graphics Graphics object containing AnimatedSprite
         */
        AnimatedSprite(const std::string &file_path, int fps, int num_frames, int start_frame, int tile_size, Graphics &graphics);

        // Destructor
        ~AnimatedSprite();

        /**
         * Update AnimatedSprite animation
         * 
         * @param elapsed_time_ms Time between previous and current frame
         */ 
        void update(int elapsed_time_ms);

        // Set Sprite direction and offset
        void faceRight();
        // Set Sprite direction and offset
        void faceLeft();

        /**
         * Set sprite position
         * 
         * @param x New x position
         * @param y New y position
         */
        void setPosition(float x, float y);

        /**
         * Set sprite position
         * 
         * @param vec New position vector
         */
        void setPosition(sf::Vector2f &vec);

    private:
        // Draw function
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    private:
        // Sprite variables
        sf::IntRect *int_rect;
        sf::Sprite sprite;
        sf::Texture texture;
        int tile_size;
        float x_offset, y_offset;

        // Animatin frame variables
        const int frame_time;
        const int num_frames;
        int start_frame;
        int current_frame;
        int elapsed_time;   // Time since last frame change
};

#endif
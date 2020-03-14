#include <SFML/Graphics.hpp>
#include <iostream>

#include "./include/AnimatedSprite.hpp"
#include "./include/Graphics.hpp"

namespace
{
    const float sprite_scale_factor = 1.f;
}

// Constructor
AnimatedSprite::AnimatedSprite(const std::string &file_path, int fps, int num_frames, int start_frame, int row, int tile_size, Graphics &graphics) :
    tile_size(tile_size),
    frame_time(1000 / fps),
    num_frames(num_frames),
    start_frame(start_frame),
    current_frame(0),
    elapsed_time(0)
{
    // Load texture
    texture = graphics.loatTexture(file_path);

    int_rect = new sf::IntRect(start_frame * tile_size, row * tile_size, tile_size, tile_size);

    // Create sprite
    sprite.setTexture(texture);
    sprite.setTextureRect(*int_rect);
    sprite.setScale(sprite_scale_factor, sprite_scale_factor);

    // Set sprite offset
    x_offset = 0;
    //y_offset = -1 * (texture.getSize().y * sprite_scale_factor);
    y_offset = 0;
    sprite.move(0.f, y_offset);
}

// Destructor
AnimatedSprite::~AnimatedSprite()
{
    delete int_rect;
}

// Update sprite animation frame
void AnimatedSprite::update(int elapsed_time_ms)
{
    elapsed_time += elapsed_time_ms;
    if (elapsed_time > frame_time)
    {
        ++current_frame;
        elapsed_time = 0;
        if (current_frame < num_frames)
        {
            int_rect->left += tile_size;
        }
        else
        {
            int_rect->left -= tile_size * (num_frames - 1);
            current_frame = 0;
        }
        sprite.setTextureRect(*int_rect);
    }
}

// Set sprite direction and offset
void AnimatedSprite::faceRight()
{
    sprite.setScale(sprite_scale_factor, sprite_scale_factor);
    x_offset = 0;
}

// Set sprite direction and offset
void AnimatedSprite::faceLeft()
{
    sprite.setScale(sprite_scale_factor * -1, sprite_scale_factor);
    x_offset = tile_size * sprite_scale_factor;
}

// Set sprite position
void AnimatedSprite::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
    sprite.move(x_offset, y_offset);
}

// Set sprite position
void AnimatedSprite::setPosition(sf::Vector2f &vec)
{
    sprite.setPosition(vec);
    sprite.move(x_offset, y_offset);
}

void AnimatedSprite::setRotation(float angle)
{
    sprite.setRotation(angle);
    x_offset = (angle >= 90 && angle <= 180) ? tile_size * sprite_scale_factor : 0;
    y_offset = (angle >= 180) ? tile_size * sprite_scale_factor : 0;
}

// Draw function
void AnimatedSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //std::cout << "AS draw" << std::endl;
    target.draw(sprite, states);
}
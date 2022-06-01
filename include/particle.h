#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.h"
#include "window.h"
#include "utility.h"

struct ParticleData
{
    std::vector<uint8_t> color;
    float rotationSpeed;
    float speed;
    float scale;
    float parallax; 
};

class Particle
{
public:
    Particle(SDL_Texture* texture, 
             const Vect<int64_t> startPos, const float angle,
             const ParticleData& data);
    ~Particle();

    void operator=(const Particle&) = delete;

    void update(Window& window);
    void render(Window& window, const Vect<int64_t> renderOffset);

private:
    SDL_Texture* texture;

    Vect<int64_t> pos;
    float angle;

    const float moveAngle;
    const ParticleData data;
    const Vect<uint32_t> size;
};
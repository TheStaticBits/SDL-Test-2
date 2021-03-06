#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Window;
#include "vector.h"

struct ParticleData
{
    std::vector<uint8_t> color;
    float rotationSpeed;
    float speed;
    int sideLength;
    float parallax; 
};

class Particle
{
public:
    Particle(SDL_Texture* texture, 
             const Vect<float> startPos, 
             const float startAngle, const float moveAngle,
             const ParticleData& data);
    ~Particle();

    void operator=(const Particle&) = delete;

    void update(Window& window, const Vect<uint32_t> baseSize);
    void render(Window& window, const Vect<int64_t> renderOffset);

private:
    SDL_Rect getRect(const Vect<int64_t> renderOffset) const;
    Vect<int32_t> inBox(const SDL_Rect rect, Vect<uint32_t> uSize);

    void wrap(Window& window, const Vect<uint32_t> baseSize);

    inline static const float ROTATE_EDGES = 0.5f;

    SDL_Texture* texture;

    Vect<float> pos;
    float angle;

    const float moveAngle;
    const ParticleData data;
};
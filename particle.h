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
             const Vect<float> startPos, 
             const float startAngle, const float moveAngle,
             const ParticleData& data);
    ~Particle();

    void operator=(const Particle&) = delete;

    void update(Window& window, 
                const Vect<int64_t> renderOffset, 
                const Vect<uint32_t> size);
    void render(Window& window, const Vect<int64_t> renderOffset);

private:
    SDL_Rect getRenderRect(const Vect<int64_t> renderOffset) const;
    void setRenderRect(const Vect<float> newPos, const Vect<int64_t> renderOffset);

    inline const Vect<int> getEdge(const SDL_Rect rect);
    inline const Vect<int> getEdgeScaled(const SDL_Rect rect);

    const Vect<uint32_t> offscreenDirs(const SDL_Rect rect, const Vect<int> size, const Vect<int> edge);

    inline static const float ROTATE_EDGES = 0.5f;

    SDL_Texture* texture;

    Vect<float> pos;
    float angle;

    const float moveAngle;
    const ParticleData data;
    const Vect<uint32_t> size;
};
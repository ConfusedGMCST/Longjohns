#ifndef LONGJOHN_H
#define LONGJOHN_H

#include <array>
#include <string>
#include <SDL3/SDL.h>

class Longjohn {
public:
    std::array<float, 2> position;
    std::array<float, 2> size;
    std::string sprite;
    SDL_Texture* texture;

    void render(SDL_Renderer* renderer) const;
};

#endif
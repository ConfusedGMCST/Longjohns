#ifndef LONGJOHN_H
#define LONGJOHN_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf>
#include <array>
#include <string>

class Label {
public:
    std::array<float, 2> position;
    std::string text;
    SDL_Surface* textSurface;
    
    void render(std::string text, TTF_Font* font) const;
}

#endif
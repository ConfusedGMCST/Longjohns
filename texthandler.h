#ifndef TEXTHANDLER_H
#define TEXTHANDLER_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <array>
#include <string>

class Label {
public:
    std::array<float, 2> position;
    SDL_Texture* texture = nullptr;
    float w = 0;
    float h = 0;

    void updateText(const std::string& newText, TTF_Font* font, SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer) const;
    void cleanup();
};

#endif
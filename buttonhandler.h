#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <array>
#include <functional>
#include "texthandler.h"

class Button {
public:
    std::array<float, 2> position;
    std::array<float, 2> size = {100.0f, 30.0f};
    SDL_Color bkgColor = {200, 200, 200, 255};
    
    Label label;
    std::function<void()> onClick; // The "lambda" storage

    void init(const std::string& text, TTF_Font* font, SDL_Renderer* renderer, std::function<void()> callback);
    void handleEvent(SDL_Event* event);
    void render(SDL_Renderer* renderer);
};

#endif
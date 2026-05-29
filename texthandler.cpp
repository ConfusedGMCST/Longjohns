#include "texthandler.h"

//finish later
void Label::render(std::string text, TTF_Font* font, SDL_Renderer renderer) const {
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, 0, {255, 255, 255, 255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);
    SDL_FRect dest = {position[0], position[1], 0, 0};
}
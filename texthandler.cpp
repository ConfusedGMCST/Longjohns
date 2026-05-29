#include "texthandler.h"

void Label::updateText(const std::string& newText, TTF_Font* font, SDL_Renderer* renderer) {
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, newText.c_str(), 0, white);
    if (!textSurface) return;

    w = (float)textSurface->w;
    h = (float)textSurface->h;

    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);
}

void Label::render(SDL_Renderer* renderer) const {
    if (!texture) return;

    SDL_FRect dest = {position[0], position[1], w, h};
    SDL_RenderTexture(renderer, texture, NULL, &dest);
}

void Label::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
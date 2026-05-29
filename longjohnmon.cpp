#include "longjohn.h"

void Longjohn::render(SDL_Renderer* renderer) const
{
    if (!texture) {
        return;
    }
    SDL_FRect dst;

    dst.x = position[0];
    dst.y = position[1];
    dst.w = size[0];
    dst.h = size[1];

    SDL_RenderTexture(renderer, texture, NULL, &dst);
}
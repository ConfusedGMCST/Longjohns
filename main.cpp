#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_main.h>
#include "longjohn.h"
#include <string>
#include <vector>

static void cleanup(SDL_Window *window, SDL_Renderer *renderer, const std::vector<SDL_Texture*> &textures)
{
    for (SDL_Texture *itr : textures) {
        SDL_DestroyTexture(itr);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* load_texture(const std::string &path, SDL_Renderer *renderer)
{
    SDL_Surface *surface = IMG_Load(path.c_str());

    if (!surface) {
        SDL_Log("IMG_Load failed: %s", SDL_GetError());
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (!texture) {
        SDL_Log("Texture creation failed: %s", SDL_GetError());
        return nullptr;
    }

    return texture;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    std::vector<SDL_Texture*> textures = {};

    Longjohn test;
    test.sprite = "assets/tim.png";

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "SDL3 Boilerplate",
        800,
        600,
        SDL_WINDOW_RESIZABLE
    );
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    SDL_Texture *texture = load_texture(test.sprite, renderer);
    textures.push_back(texture);

    bool running = true;

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN &&
                event.key.key == SDLK_ESCAPE) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);
    }

    cleanup(window, renderer, )

    SDL_Quit();
    return 0;
}
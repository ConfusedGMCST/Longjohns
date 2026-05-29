#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_main.h>
#include "longjohn.h"
#include <string>
#include <vector>

SDL_Texture* load_texture(const std::string &path, SDL_Renderer *renderer);

static void cleanup(SDL_Window *window, SDL_Renderer *renderer, const std::vector<Longjohn> &longjohns, TTF_Font* font)
{
    for (const Longjohn& itr : longjohns) {
        SDL_DestroyTexture(itr.texture);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

//example color --> SDL_MapRGB(fmt, nullptr, 50, 50, 50)
// static void setBkg(SDL_Window *window, Uint32 color) {
// 	SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
//     const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(windowSurface->format);
// 	SDL_FillSurfaceRect(windowSurface, nullptr, color);
// 	SDL_UpdateWindowSurface(window);
// }

void renderLongjohns(SDL_Renderer* renderer, const std::vector<Longjohn>& longjohns)
{
    for (const Longjohn& itr : longjohns) {
        itr.render(renderer);
    }
}

Longjohn init_longjohn(std::string sprite_path, SDL_Renderer* renderer, float posX, float posY, float sizeX, float sizeY) {
    Longjohn new_john;

    new_john.position = {posX, posY};
    new_john.size = {sizeX, sizeY};
    new_john.sprite = sprite_path;

    new_john.texture = load_texture(sprite_path, renderer);

    return new_john;
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

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    TTF_Init();

    //default pokemon size is 320x320 for this project, so use that along with window size for positions
    int winX = 800, winY = 600;
    int ljSize = 320;

    SDL_Window *window = SDL_CreateWindow(
        "Longjohnemon",
        winX,
        winY,
        // SDL_WINDOW_RESIZABLE
        0
    );
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    std::vector<Longjohn> longjohns;

    //pokemon initialization

    Longjohn tim = init_longjohn("assets/tim.png", renderer, 0, winY - ljSize, ljSize, ljSize);
    Longjohn gabe = init_longjohn("assets/gabe.png", renderer, winX - ljSize, 0, ljSize, ljSize);

    longjohns.push_back(gabe);
    longjohns.push_back(tim);

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

        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 120);
        SDL_RenderClear(renderer);

        renderLongjohns(renderer, longjohns);

        SDL_RenderPresent(renderer);
    }

    cleanup(window, renderer, longjohns);
    return 0;
}

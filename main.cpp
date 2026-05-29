#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_main.h>
#include "longjohn.h"
#include "texthandler.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

SDL_Texture* load_texture(const std::string &path, SDL_Renderer *renderer);

static void cleanup(SDL_Window *window, SDL_Renderer *renderer, const std::vector<Longjohn> &longjohns, TTF_Font* font, std::vector<Label> &labels)
{
    for (const Longjohn& itr : longjohns) {
        if (itr.texture) SDL_DestroyTexture(itr.texture);
    }

    for (Label& itr : labels) {
        itr.cleanup();
    }

    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void renderLongjohns(SDL_Renderer* renderer, const std::vector<Longjohn>& longjohns)
{
    for (const Longjohn& itr : longjohns) {
        itr.render(renderer);
    }
}

void renderLabels(SDL_Renderer* renderer, const std::vector<Label>& labels) {
    for (const Label& itr : labels) {
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
    return texture;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO)) return 1;
    if (!TTF_Init()) return 1;

    TTF_Font* gameFont = TTF_OpenFont("fonts/RobotoMono-Regular.ttf", 24);
    if (!gameFont) return 1;

    int winX = 800, winY = 600;
    int ljSize = 320;

    SDL_Window *window = SDL_CreateWindow("Longjohnemon", winX, winY, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    std::vector<Longjohn> longjohns;
    std::vector<Label> labels;

    Longjohn tim = init_longjohn("assets/tim.png", renderer, 0, winY - ljSize - 30, ljSize, ljSize);
    Longjohn gabe = init_longjohn("assets/gabe.png", renderer, winX - ljSize, 0, ljSize, ljSize);

    longjohns.push_back(gabe);
    longjohns.push_back(tim);

    Label gLabel;
    gLabel.position = {450.0f, 320.0f};
    labels.push_back(gLabel);

    Label tLabel;
    tLabel.position = {0, 570.0f};
    labels.push_back(tLabel);

    labels[0].updateText("GABE", gameFont, renderer);
    labels[1].updateText("TIM", gameFont, renderer);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_RenderClear(renderer);

        renderLongjohns(renderer, longjohns);
        renderLabels(renderer, labels);

        SDL_RenderPresent(renderer);
    }

    cleanup(window, renderer, longjohns, gameFont, labels);
    return 0;
}
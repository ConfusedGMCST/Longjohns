#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_main.h>
#include "longjohn.h"
#include "texthandler.h"
#include "buttonhandler.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

SDL_Texture* load_texture(const std::string &path, SDL_Renderer *renderer);

static void cleanup(SDL_Window *window, SDL_Renderer *renderer, const std::vector<Longjohn> &longjohns, TTF_Font* font, std::vector<Label> &labels, std::vector<Button> &buttons)
{
    for (const Longjohn& itr : longjohns) {
        if (itr.texture) SDL_DestroyTexture(itr.texture);
    }
    for (Label& itr : labels) {
        itr.cleanup();
    }
    for (Button& btn : buttons) {
        btn.label.cleanup();
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

void renderButtons(SDL_Renderer* renderer, std::vector<Button>& buttons) {
    for (Button& btn : buttons) {
        btn.render(renderer);
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
    if (!surface) return nullptr;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}

std::string label_string(std::string name, float health, int level) {
    return name + " Health:" + std::to_string(health) + " LVL:" + std::to_string(level);
}

void attackMenu(std::vector<Button>& buttons, TTF_Font* font, SDL_Renderer *renderer) {
    Button moveOne;
    Button moveTwo;
    Button moveThree;
    Button moveFour;

    float menuX = 410, menuY = 540;

    moveOne.position = {menuX - moveOne.size[0] / 2, menuY - moveOne.size[1] / 2};
    moveTwo.position = {menuX + moveTwo.size[0] / 2, menuY - moveOne.size[1] / 2};
    moveThree.position = {menuX - moveOne.size[0] / 2, menuY + moveOne.size[1] / 2};
    moveFour.position = {menuX + moveOne.size[0] / 2, menuY + moveOne.size[1] / 2};

    moveOne.init("test1", font, renderer, []() { SDL_Log("Move 1 selected"); });
    moveTwo.init("test2", font, renderer, []() { SDL_Log("Move 2 selected"); });
    moveThree.init("test3", font, renderer, []() { SDL_Log("Move 3 selected"); });
    moveFour.init("test4", font, renderer, []() { SDL_Log("Move 4 selected"); });

    buttons.push_back(moveOne);
    buttons.push_back(moveTwo);
    buttons.push_back(moveThree);
    buttons.push_back(moveFour);
}

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    bool attackMenuUp = false;

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
    std::vector<Button> buttons;

    Longjohn tim = init_longjohn("assets/tim.png", renderer, 0, winY - ljSize - 30, ljSize, ljSize);
    Longjohn gabe = init_longjohn("assets/gabe.png", renderer, winX - ljSize, 0, ljSize, ljSize);

    longjohns.push_back(tim);
    longjohns.push_back(gabe);

    Label gLabel; gLabel.position = {450.0f, 320.0f}; labels.push_back(gLabel);
    Label tLabel; tLabel.position = {0, 570.0f}; labels.push_back(tLabel);

    labels[0].updateText(label_string("GABE", gabe.health, gabe.level), gameFont, renderer);
    labels[1].updateText(label_string("TIM", tim.health, tim.level), gameFont, renderer);

    // Initialize main Attack button
    Button attackBtn;
    attackBtn.position = {220, 570};
    attackBtn.init("ATTACK", gameFont, renderer, [&]() {
        if (!attackMenuUp) {
            attackMenu(buttons, gameFont, renderer);
            attackMenuUp = true;
        } else {
            // Cleanup textures for buttons we are about to remove
            for (size_t i = 1; i < buttons.size(); ++i) {
                buttons[i].label.cleanup();
            }
            // Keep only the first button (the ATTACK button itself)
            buttons.resize(1);
            attackMenuUp = false;
        }
    });

    // attackBtn is at index 0
    buttons.push_back(attackBtn);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) running = false;
            
            // Pass events backwards to handle UI layering correctly
            for (int i = buttons.size() - 1; i >= 0; --i) {
                buttons[i].handleEvent(&event);
            }
        }

        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_RenderClear(renderer);

        renderLongjohns(renderer, longjohns);
        renderLabels(renderer, labels);
        renderButtons(renderer, buttons);

        SDL_RenderPresent(renderer);
    }

    cleanup(window, renderer, longjohns, gameFont, labels, buttons);
    return 0;
}
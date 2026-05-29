#include "buttonhandler.h"

void Button::init(const std::string& text, TTF_Font* font, SDL_Renderer* renderer, std::function<void()> callback) {
    onClick = callback;
    
    // Initialize the label inside the button
    label.updateText(text, font, renderer);
    
    // Center the label inside the 100x50 box
    label.position[0] = position[0] + (size[0] - label.w) / 2.0f;
    label.position[1] = position[1] + (size[1] - label.h) / 2.0f;
}

void Button::handleEvent(SDL_Event* event) {
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mx = event->button.x;
        float my = event->button.y;

        // Check if mouse is inside button bounds
        if (mx >= position[0] && mx <= position[0] + size[0] &&
            my >= position[1] && my <= position[1] + size[1]) {
            if (onClick) {
                onClick(); // Execute the passed lambda
            }
        }
    }
}

void Button::render(SDL_Renderer* renderer) {
    SDL_FRect rect = {position[0], position[1], size[0], size[1]};
    
    // Draw button background
    SDL_SetRenderDrawColor(renderer, bkgColor.r, bkgColor.g, bkgColor.b, bkgColor.a);
    SDL_RenderFillRect(renderer, &rect);
    
    // Draw button border (black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &rect);
    
    // Draw the text label
    label.render(renderer);
}
#pragma once

#include <iostream>
#include <printf.h>
#include <functional>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

const int HOVERDARKENOFFSET = 20;

struct colorbutton
{
    float x,y,w,h;
    SDL_Color ButtonColor;
    SDL_Renderer* buttonRenderer;
    
    // blank lambda; for use later!
    std::function<void()> onClickFunc;

    colorbutton();

    // structInit, define all necessary struct values
    void structInit(SDL_Renderer* newButtonRenderer, float nx, float ny, float nw, float nh, SDL_Color NButtonColor);

    // handleFrame, check for clicks & hover!
    void handleFrame(bool HandleHover);
};

colorbutton::colorbutton(){

};

void colorbutton::structInit(SDL_Renderer* newButtonRenderer, float nx, float ny, float nw, float nh, SDL_Color NButtonColor){
    ButtonColor = NButtonColor;
    x = nx; y = ny; w = nw; h = nh; // define X, Y, W, H
    buttonRenderer = newButtonRenderer;
}

void colorbutton::handleFrame(bool HandleHover){
    Uint8 OldR, OldG, OldB, OldA;
    SDL_GetRenderDrawColor(buttonRenderer, &OldR, &OldG, &OldB, &OldA);

    bool isHovering;

    float mx, my;
    
    Uint32 mouseState = SDL_GetMouseState(&mx, &my);

    if (mx >= x && mx <= x+w && my >= y && my <= y + h){
        isHovering = true;
    } // check for hovering

    if (isHovering == true){
        SDL_SetRenderDrawColor(buttonRenderer, 
        std::abs(ButtonColor.r-HOVERDARKENOFFSET), 
        std::abs(ButtonColor.g-HOVERDARKENOFFSET),
        std::abs(ButtonColor.b-HOVERDARKENOFFSET), 
        ButtonColor.a);
    } else if (isHovering == false) {
        SDL_SetRenderDrawColor(buttonRenderer, 
        std::abs(ButtonColor.r), 
        std::abs(ButtonColor.g),
        std::abs(ButtonColor.b), 
        ButtonColor.a);
    } // handle hover colour!

    SDL_FRect rect = {x, y, w, h};
    SDL_RenderFillRect(buttonRenderer, &rect);

    SDL_SetRenderDrawColor(buttonRenderer, OldR, OldG, OldB, OldA);

    if (mouseState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT && isHovering == true)){
        if (onClickFunc){
            onClickFunc();
        }
    }
}
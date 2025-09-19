#pragma once

#include <iostream>
#include <printf.h>
#include <functional>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

struct textbutton
{
    float x,y,w,h;
    SDL_Color ButtonColor;
    SDL_Color ButtonTextColor;
    SDL_Renderer* buttonRenderer;
    std::string buttonText;

    TTF_Font* font = TTF_OpenFont("fonts/Roboto/Roboto-VariableFont_wdth,wght.ttf", 64);
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    
    // blank lambda; for use later!
    std::function<void()> onClickFunc;

    textbutton();

    // structInit, define all necessary struct values
    void structInit(SDL_Renderer* newButtonRenderer, std::string newButtonText, float nx, float ny, float nw, float nh, SDL_Color NButtonColor, SDL_Color NButtonTextColor);

    // handleFrame, check for clicks & hover!
    void handleFrame(bool HandleHover, SDL_Window* window);
};

textbutton::textbutton(){

};

void textbutton::structInit(SDL_Renderer* newButtonRenderer, std::string newButtonText, float nx, float ny, float nw, float nh, SDL_Color NButtonColor,  SDL_Color NButtonTextColor){
    ButtonColor = NButtonColor;
    x = nx; y = ny; w = nw; h = nh; // define X, Y, W, H
    buttonRenderer = newButtonRenderer;
    buttonText = newButtonText;
    ButtonTextColor = NButtonTextColor;

    textSurface = TTF_RenderText_Blended(font, buttonText.c_str(), buttonText.size(), ButtonTextColor);
    textTexture = SDL_CreateTextureFromSurface(buttonRenderer, textSurface);
}

void textbutton::handleFrame(bool HandleHover, SDL_Window* window){
    int HOVERDARKENOFFSET = 20;

    Uint8 OldR, OldG, OldB, OldA;
    SDL_GetRenderDrawColor(buttonRenderer, &OldR, &OldG, &OldB, &OldA);

    bool isHovering = false;

    float mx, my;
    Uint32 mouseState = SDL_GetMouseState(&mx, &my);

    int ww, wh;
    SDL_GetWindowSizeInPixels(window, &ww, &wh);

    mx *= 1280/(float)ww;
    my *= 720/(float)wh;

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
    SDL_FRect textRect = {x+(w/2)/2, y+(h/2)/2, w/2, h/2};
    SDL_RenderFillRect(buttonRenderer, &rect); // draw the rectangle

    SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);

    SDL_RenderTexture(buttonRenderer, textTexture, NULL, &textRect);

    SDL_SetRenderDrawColor(buttonRenderer, OldR, OldG, OldB, OldA);

    if (mouseState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT && isHovering == true)){
        if (onClickFunc){
            onClickFunc();
        }
    }
}
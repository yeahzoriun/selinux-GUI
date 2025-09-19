#pragma once

#include <iostream>
#include <printf.h>
#include <functional>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

struct text
{
    float x,y,w,h;

    std::string textString;
    SDL_Color textColor;
    SDL_Renderer* textRenderer;

    TTF_Font* font = TTF_OpenFont("fonts/Roboto/Roboto-VariableFont_wdth,wght.ttf", 64);
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;

    text();

    void structInit(SDL_Renderer* newTextRenderer, float nx, float ny, float nh, std::string newTextString, SDL_Color newTextColor);
    void handleFrame();
};

text::text(){

}

void text::structInit(SDL_Renderer* newTextRenderer, float nx, float ny, float nh, std::string newTextString, SDL_Color newTextColor){
    x = nx; y = ny; h = nh;
    w = (h*0.40)*newTextString.size();
    textRenderer = newTextRenderer;
    textString = newTextString;
    textColor = newTextColor;

    textSurface = TTF_RenderText_Blended(font, textString.c_str(), textString.size(), textColor);
    textTexture = SDL_CreateTextureFromSurface(textRenderer, textSurface);
}

void text::handleFrame(){
    SDL_FRect textRect = {x,y,w,h};
    SDL_RenderTexture(textRenderer, textTexture, NULL, &textRect);
}
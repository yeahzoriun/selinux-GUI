#pragma once

#include <iostream>
#include <printf.h>
#include <functional>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

struct rectangle
{
    float x, y, w, h;
    SDL_Color rectangleColor;
    SDL_Renderer* rectangleRenderer;

    rectangle();
    ~rectangle();

    // structInit, initialize all necessary struct values.
    void structInit(SDL_Renderer* newRectangleRenderer, float nx, float ny, float nw, float nh, SDL_Color newRectangleColor);

    // handleFrame, render that frame.
    void handleFrame();
};

rectangle::rectangle(){

}

rectangle::~rectangle(){
    
}

void rectangle::structInit(SDL_Renderer* newRectangleRenderer, float nx, float ny, float nw, float nh, SDL_Color newRectangleColor){
    x = nx; y = ny; w = nw; h = nh;
    rectangleColor = newRectangleColor;
    rectangleRenderer = newRectangleRenderer;
}

void rectangle::handleFrame(){
    Uint8 oldR, oldG, oldB, oldA;
    SDL_GetRenderDrawColor(rectangleRenderer, &oldR, &oldG, &oldB, &oldA);

    SDL_SetRenderDrawColor(rectangleRenderer, rectangleColor.r, rectangleColor.g, rectangleColor.b, rectangleColor.a);

    SDL_FRect rect = {x,y,w,h};
    SDL_RenderFillRect(rectangleRenderer, &rect);

    SDL_SetRenderDrawColor(rectangleRenderer, oldR, oldG, oldB, oldA);
}
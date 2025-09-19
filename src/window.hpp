#pragma once

#include <iostream>
#include <printf.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

struct window
{
    std::string Title;
    int Width;
    int Height;

    bool running = true;

    SDL_Window* windowOBJ;
    SDL_Renderer* rendererOBJ;

    window(std::string NTitle, int NWidth, int NHeight);
};

window::window(std::string NTitle, int NWidth, int NHeight){
    windowOBJ = SDL_CreateWindow(NTitle.c_str(), NWidth, NHeight, 0);
    if (!windowOBJ){
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Window didn't create");
    }
    rendererOBJ = SDL_CreateRenderer(windowOBJ, NULL);
    if (!rendererOBJ){
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Renderer didn't create");
    }
    SDL_SetRenderLogicalPresentation(rendererOBJ, NWidth, NHeight, SDL_LOGICAL_PRESENTATION_STRETCH);
}
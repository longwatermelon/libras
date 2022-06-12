#include "ras.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


void ras_init_sdl()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
}


void ras_quit_sdl()
{
    TTF_Quit();
    SDL_Quit();
}


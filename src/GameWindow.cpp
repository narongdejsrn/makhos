//
//  GameWindow.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 8/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/GameWindow.hpp"

using namespace makhos;

GameWindow::GameWindow() {
    initializeVideo();
}
    
void GameWindow::destroy() {
    
    SDL_DestroyWindow(window);
    
    
    TTF_Quit();
    SDL_Quit();
};
    
void GameWindow::initializeVideo() {
    // Try to init SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Could not init: %s\n", SDL_GetError());
        throw SDL_INIT_ERROR;
    }
    
    //Set texture filtering to linear
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
    {
        printf( "Warning: Linear texture filtering not enabled!" );
    }
    
    window = SDL_CreateWindow("Makhos", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created!: %s", SDL_GetError());
        throw WINDOW_CREATE_ERROR;
    }
    
    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        throw TTF_INIT_ERROR;
    }
}

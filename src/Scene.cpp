//
//  Scene.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 8/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/Scene.hpp"
#include <iostream>

using namespace makhos;

Scene::Scene(SDL_Window* window, GameLoop *gameLoop) {
    this->window = window;
    this->gameLoop = gameLoop;
    this->createRenderer();
    std::cout << "Game Loop ID (scene): " << gameLoop << '\n';
}

void Scene::createRenderer() {
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! %s\n", SDL_GetError());
        throw RENDERER_CREATE_ERROR;
    }
    
    SDL_SetRenderDrawColor( this->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    printf("renderer completed\n");
    std::cout << "renderer id: " << renderer << '\n';
}



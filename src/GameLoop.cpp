//
//  GameLoop.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 8/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/GameLoop.hpp"
#include "../include/MainMenu.hpp"
#include "../include/SinglePlayerMenu.hpp"
#include "../include/GameEvent.hpp"
#include "../include/GameRoom.hpp"

using namespace makhos;

GameLoop::GameLoop() {
    gameWindow = new GameWindow();
    GameEvent *events = GameEvent::getInstance();
    
    bool quit = false;
    SDL_Event e;
    
    std::cout << "Gameloop ID (main):" << this << '\n';
//    currentScene = new MainMenu(gameWindow->window, this);
    currentScene = new GameRoom(gameWindow->window, this);
    
    while (!quit) {
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            
            events->notify(&e);
        }
        
        currentScene->draw();
    }
    
    delete currentScene;
    gameWindow->destroy();
}

void GameLoop::changeScene(GameScenes sceneName) {
    delete currentScene;
    
    switch (sceneName) {
        case SINGLE_PLAYER_MENU:
            currentScene = new SinglePlayerMenu(gameWindow->window, this);
            break;
        case GAME_ROOM:
            currentScene = new GameRoom(gameWindow->window, this);
            break;
        default:
            currentScene = new MainMenu(gameWindow->window, this);
            break;
    }
}

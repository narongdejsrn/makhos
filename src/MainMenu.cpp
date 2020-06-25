//
//  MainMenu.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 8/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/MainMenu.hpp"
#include "../include/TextButton.hpp"
#include "../include/GameLoop.hpp"

using namespace makhos;

TextButton* btn1;
TextButton* btn2;

MainMenu::MainMenu(SDL_Window* Cwindow, GameLoop *gameLoop): Scene(Cwindow, gameLoop) {
//    this->gameLoop = gameLoop;
    std::cout << "Game Loop ID (main menu): " << gameLoop << '\n';
    
    btn1 = new TextButton(this->renderer);
    btn1->x = 20;
    btn1->y = 20;
    btn1->text = "Single Player";
    btn1->setFont("fonts/GlacialIndifference-Regular.ttf", 30);
    btn1->backgroundColor = {0, 255, 255, 0};
    btn1->backgroundHoverColor = {150, 150, 255, 0};
    btn1->backgroundClickColor = {200, 200, 255, 0};
    btn1->clickEvent = std::bind(&MainMenu::changeToSinglePlayerMenu, this);
    
    btn2 = new TextButton(this->renderer);
    btn2->x = 20;
    btn2->y = 100;
    btn2->text = "Multiplayer";
    btn2->setFont("fonts/GlacialIndifference-Regular.ttf", 30);
    btn2->backgroundColor = {0, 255, 255, 0};
    btn2->backgroundHoverColor = {150, 150, 255, 0};
    btn2->backgroundClickColor = {200, 200, 255, 0};
}

void MainMenu::changeToSinglePlayerMenu() {
    printf("Lets change scene\n");
    std::cout << "Change c gameloop id:" << gameLoop << '\n';
    gameLoop->changeScene(SINGLE_PLAYER_MENU);
}

void MainMenu::draw() {
    SDL_RenderClear( this->renderer );
    btn1->draw();
    btn2->draw();
    
    SDL_SetRenderDrawColor( this->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderPresent( this->renderer );
}

MainMenu::~MainMenu() {
    SDL_DestroyRenderer( this->renderer );
    delete btn1;
    delete btn2;
}

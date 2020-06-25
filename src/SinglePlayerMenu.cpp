//
//  SinglePlayerMenu.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 9/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/SinglePlayerMenu.hpp"
#include "../include/GameLoop.hpp"

using namespace makhos;

SinglePlayerMenu::SinglePlayerMenu(SDL_Window* Cwindow, GameLoop *gameLoop): Scene(Cwindow, gameLoop) {
    std::cout << "called this spm" << '\n';
    std::cout << "renderer id: " << renderer << '\n';
    
    btn1 = new TextButton(this->renderer);
    btn1->x = 20;
    btn1->y = 20;
    btn1->text = "Easy";
    btn1->setFont("fonts/GlacialIndifference-Regular.ttf", 30);
    btn1->backgroundColor = {0, 255, 255, 0};
    btn1->backgroundHoverColor = {150, 150, 255, 0};
    btn1->backgroundClickColor = {200, 200, 255, 0};
    btn1->clickEvent = std::bind(&SinglePlayerMenu::goToGameRoom, this);
    
    btn2 = new TextButton(this->renderer);
    btn2->x = 20;
    btn2->y = 100;
    btn2->text = "Medium";
    btn2->setFont("fonts/GlacialIndifference-Regular.ttf", 30);
    btn2->backgroundColor = {0, 255, 255, 0};
    btn2->backgroundHoverColor = {150, 150, 255, 0};
    btn2->backgroundClickColor = {200, 200, 255, 0};
    
    btn3 = new TextButton(this->renderer);
    btn3->x = 20;
    btn3->y = 200;
    btn3->text = "Hard";
    btn3->setFont("fonts/GlacialIndifference-Regular.ttf", 30);
    btn3->backgroundColor = {0, 255, 255, 0};
    btn3->backgroundHoverColor = {150, 150, 255, 0};
    btn3->backgroundClickColor = {200, 200, 255, 0};
    printf("init spm completed\n");
}

void SinglePlayerMenu::goToGameRoom() {
    gameLoop->changeScene(GAME_ROOM);
}

void SinglePlayerMenu::draw() {
    SDL_RenderClear( this->renderer );
    
    btn1->draw();
    btn2->draw();
    btn3->draw();
    
    SDL_SetRenderDrawColor( this->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderPresent( this->renderer );
}

SinglePlayerMenu::~SinglePlayerMenu() {
    SDL_DestroyRenderer( this->renderer );
    delete btn1;
    delete btn2;
    delete btn3;
}

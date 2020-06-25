//
//  MainMenu.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 8/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef MainMenu_hpp
#define MainMenu_hpp

#include <stdio.h>
#include "Scene.hpp"
#include "TextButton.hpp"

namespace makhos {

class MainMenu: public Scene {
public:
    MainMenu(SDL_Window* window, GameLoop *gameLoop);
    ~MainMenu();
    void draw();
    void changeToSinglePlayerMenu();
private:
    TextButton *btn1;
    TextButton *btn2;
};

}

#endif /* MainMenu_hpp */

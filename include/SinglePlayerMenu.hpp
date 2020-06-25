//
//  SinglePlayerMenu.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 9/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef SinglePlayerMenu_hpp
#define SinglePlayerMenu_hpp

#include "Scene.hpp"
#include "TextButton.hpp"


namespace makhos {

class SinglePlayerMenu: public Scene {
public:
    SinglePlayerMenu(SDL_Window* window, GameLoop *gameLoop);
    ~SinglePlayerMenu();
    void draw();
protected:
    void goToGameRoom();
private:
    TextButton *btn1;
    TextButton *btn2;
    TextButton *btn3;

};

}

#endif /* SinglePlayerMenu_hpp */

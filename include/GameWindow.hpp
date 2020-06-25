//
//  GameWindow.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 8/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef GameWindow_hpp
#define GameWindow_hpp

#include <stdio.h>
#include "Constants.hpp"
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

namespace makhos {

class GameWindow {
public:
    GameWindow();
    SDL_Window* window;
    void destroy();
private:
    void initializeVideo();
};

}

#endif /* GameWindow_hpp */

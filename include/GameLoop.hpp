//
//  GameLoop.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 8/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef GameLoop_hpp
#define GameLoop_hpp

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>

#include "Constants.hpp"
#include "Scene.hpp"
#include "GameWindow.hpp"

namespace makhos {

class GameLoop {
public:
    void changeScene(GameScenes sceneName);
    GameLoop();
private:
    Scene *currentScene;
    GameWindow *gameWindow;
};

}

#endif /* GameLoop_hpp */

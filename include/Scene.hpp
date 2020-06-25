//
//  Scene.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 8/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <SDL2/SDL.h>

#include "Constants.hpp"

namespace makhos {

class GameLoop;

class Scene {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    Scene(SDL_Window* window, GameLoop* sgameLoop);
    virtual ~Scene() {};
    virtual void draw() {};
    
protected:
    GameLoop *gameLoop;
    
private:
    void createRenderer();
};

}
#endif /* Scene_hpp */

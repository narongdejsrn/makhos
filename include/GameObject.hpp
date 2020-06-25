//
//  GameObject.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 11/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include <functional>
#include <string>

#include "ZTexture.hpp"
#include "GameEvent.hpp"
#include "Primitive.hpp"

namespace makhos {

class GameObject: public EventObserver {
public:
    GameObject(ZTexture *texture);
    GameObject(Primitive *primitive);
    
    void draw();
    void draw(SDL_Rect *clip);
    
    void setPosition(int x, int y);
    void attachClickEvent(std::function<void()> clickEvent);
    void handleEvent(SDL_Event *e);
    int getX();
    int getY();
    
    std::string name;
    
private:
    std::function<void()> clickEvent;
    
    ZTexture *texture = nullptr;
    Primitive *primitive = nullptr;
    
    int getWidth();
    int getHeight();
    
    int x, y;
};

}

#endif /* GameObject_hpp */

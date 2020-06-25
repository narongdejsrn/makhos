//
//  TextButton.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 8/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef TextButton_hpp
#define TextButton_hpp

#include <stdio.h>
#include <string>
#include <functional>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "Constants.hpp"
#include "ZTexture.hpp"
#include "GameEvent.hpp"

namespace makhos {

enum ButtonStatus
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

class TextButton: public EventObserver {
public:
    SDL_Renderer* renderer;
    int x, y;
    std::string text;
    ZTexture textTexture = NULL;
    SDL_Color textColor;
    SDL_Color backgroundColor;
    SDL_Color backgroundHoverColor;
    SDL_Color backgroundClickColor;
    std::function<void()> clickEvent;
    
    TextButton(SDL_Renderer* mRenderer);
    ~TextButton();
    void draw();
    void setFont(const char * fontPath, int fontSize);
    void handleEvent( SDL_Event *event);
    
private:
    TTF_Font* font = nullptr;
    ButtonStatus mouseStatus = BUTTON_SPRITE_MOUSE_OUT;
    GameEvent *gameEvent;
};

}

#endif /* TextButton_hpp */

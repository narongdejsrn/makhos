//
//  Primitive.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 12/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef Primitive_hpp
#define Primitive_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <functional>

namespace makhos {

enum PrimitiveType {
    RECTANGLE = 0
};

class Primitive  {
public:
    Primitive(SDL_Renderer *renderer, PrimitiveType type, int w, int h, int x, int y);
    void draw();
    int getWidth();
    int getHeight();
    void setColor(int r, int g, int b, int a);
private:
    SDL_Rect fillRect;
    SDL_Renderer *renderer;
    PrimitiveType primitiveType;
    int x, y;
    int w, h;
    int r, g, b, a;
};

}

#endif /* Primitive_hpp */

//
//  Primitive.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 12/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/Primitive.hpp"
#include "iostream"

using namespace makhos;

Primitive::Primitive(SDL_Renderer *renderer, PrimitiveType primitiveType, int w, int h, int x, int y) {
    this->w = w;
    this->h = h;
    this->x = x;
    this->y = y;
    this->renderer = renderer;
    this->primitiveType = primitiveType;
    switch (primitiveType) {
        case RECTANGLE:
            fillRect = { x, y, w, h};
            break;
    }
}

void Primitive::setColor(int r, int g, int b, int a){
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

int Primitive::getWidth() {
    return this->w;
}

int Primitive::getHeight() {
    return this->h;
}

void Primitive::draw() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor( this->renderer, r, g, b, a );
    switch (this->primitiveType) {
        case RECTANGLE:
            SDL_RenderFillRect( this->renderer, &fillRect );
            break;
    }
}

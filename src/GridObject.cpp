//
//  GridObject.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 13/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/GridObject.hpp"

using namespace makhos;

GridObject::GridObject(ZTexture *texture): GameObject(texture) {
    actionable = false;
}

void GridObject::draw(SDL_Renderer *renderer) {
    GameObject::draw();
    
    if(actionable) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        Primitive rect = Primitive(renderer, RECTANGLE, 56, 56, this->position.col * 56, this->position.row * 56);
        rect.setColor(128, 128, 128, 240);
        rect.draw();
    }
}

GridObject::GridObject(Primitive *primitive): GameObject(primitive) {}

BoardPosition GridObject::getPosition() {
    return BoardPosition{(getY()) / 56, (getX()) / 56};
}

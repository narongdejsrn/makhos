//
//  PawnObject.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 11/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/PawnObject.hpp"

using namespace makhos;

PawnObject::PawnObject(ZTexture *texture, PawnType pawnType): GameObject(texture) {
    this->pawnType = pawnType;
    this->isKing = false;
    this->dead = false;
}

BoardPosition PawnObject::getPosition() {
    return BoardPosition{(getY() - 5) / 56, (getX() - 5) / 56};
}

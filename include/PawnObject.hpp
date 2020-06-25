//
//  PawnObject.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 11/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef PawnObject_hpp
#define PawnObject_hpp

#include <stdio.h>
#include "Constants.hpp"
#include "ZTexture.hpp"
#include "GameObject.hpp"
#include "Checker.hpp"

namespace makhos {

enum PawnType {
    RED_PAWN = 0,
    BLUE_PAWN = 1,
};

class PawnObject: public GameObject {
public:
    PawnObject();
    PawnObject(ZTexture *texture, PawnType pawnType);
    PawnType pawnType;
    BoardPosition getPosition();
    
    BoardPosition position;
    bool isKing;
    int dead;
};

}

#endif /* PawnObject_hpp */

//
//  GridObject.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 13/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef GridObject_hpp
#define GridObject_hpp

#include <stdio.h>
#include "GameObject.hpp"
#include "Constants.hpp"
#include "Primitive.hpp"
#include "Checker.hpp"

namespace makhos {

enum GridType {
    WHITE_GRID = 0,
    BLACK_GRID = 1
};

class GridObject: public GameObject {
public:
    GridObject(ZTexture *zTexture);
    GridObject(Primitive *primitive);
    BoardPosition getPosition();
    
    void draw(SDL_Renderer *renderer);
    GridType gridType;
    BoardPosition position;
    bool actionable = false;
    
};

}


#endif /* GridObject_hpp */

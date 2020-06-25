//
//  Globals.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 9/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef Globals_hpp
#define Globals_hpp

#include <stdio.h>

namespace makhos {
enum GameScenes
{
    MAIN_MENU = 0,
    SINGLE_PLAYER_MENU = 1,
};

extern GameScenes CURRENT_SCENE;

}

#endif /* Globals_hpp */


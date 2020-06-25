#pragma once
//
//  Constants.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 8/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef Constants_hpp
#define Constants_hpp

namespace makhos {

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// ERROR
extern const int SDL_INIT_ERROR;
extern const int WINDOW_CREATE_ERROR;
extern const int RENDERER_CREATE_ERROR;
extern const int TTF_INIT_ERROR;
extern const int TTF_FONT_ERROR;
extern const int TTF_FONT_RENDER_ERROR;
extern const int ASSET_LOAD_ERROR;

enum GameScenes
{
    MAIN_MENU = 0,
    SINGLE_PLAYER_MENU = 1,
    GAME_ROOM = 2
};

struct TempBoardPosition {
    int col;
    int row;
};

}

#endif /* Constants_hpp */

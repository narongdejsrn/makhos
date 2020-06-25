//
//  ZTexture.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 9/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef ZTexture_hpp
#define ZTexture_hpp

#include <stdio.h>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>

namespace makhos {

class ZTexture {
public:
    ZTexture(SDL_Renderer* renderer);
    ~ZTexture();
    
    bool createTexture(int width, int height);
    bool loadFromFile( std::string path );
    bool loadFromRenderedText( std::string textureText, TTF_Font* font, SDL_Color textColor );
    void free();
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
    void setBlendMode( SDL_BlendMode bleeding );
    void setAlpha( Uint8 alpha );
    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void setRenderSize(int x, int y);
    
    int getWidth();
    int getHeight();
    
private:
    SDL_Texture* zTexture;
    SDL_Renderer* renderer;
    int zHeight;
    int zWidth;
    
    int x;
    int y;
};

}

#endif /* ZTexture_hpp */

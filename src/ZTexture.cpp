//
//  ZTexture.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 9/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/ZTexture.hpp"

using namespace makhos;

ZTexture::ZTexture(SDL_Renderer* renderer)
{
    zTexture = NULL;
    zWidth = 0;
    zHeight = 0;
    this->renderer = renderer;
}

ZTexture::~ZTexture()
{
    //Deallocate
    free();
}

bool ZTexture::createTexture(int width, int height  ) {
    free();
    
    zTexture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA5551, SDL_TEXTUREACCESS_STATIC, width, height);
    zWidth = width;
    zHeight = height;
    return true;
}

bool ZTexture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( this->renderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            zWidth = loadedSurface->w;
            zHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    zTexture = newTexture;
    return zTexture != NULL;
}

bool ZTexture::loadFromRenderedText( std::string textureText, TTF_Font* font, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Blended( font, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        zTexture = SDL_CreateTextureFromSurface( this->renderer, textSurface );
        if( zTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            zWidth = textSurface->w;
            zHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return zTexture != NULL;
}

void ZTexture::free()
{
    //Free texture if it exists
    if( zTexture != NULL )
    {
        SDL_DestroyTexture( zTexture );
        zTexture = NULL;
        zWidth = 0;
        zHeight = 0;
    }
}

void ZTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture rgb
    SDL_SetTextureColorMod( zTexture, red, green, blue );
}

void ZTexture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( zTexture, blending );
}
        
void ZTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( zTexture, alpha );
}

void ZTexture::setRenderSize(int x, int y) {
    this->zWidth = x;
    this->zHeight = y;
}

void ZTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    // Set last known position
    this->x = x;
    this->y = y;
    
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, zWidth, zHeight };

    //Set clip rendering dimensions
//    if( clip != NULL )
//    {
//        renderQuad.w = clip->w;
//        renderQuad.h = clip->h;
//    }

    //Render to screen
    SDL_RenderCopyEx( this->renderer, zTexture, clip, &renderQuad, angle, center, flip );
}

int ZTexture::getWidth()
{
    return zWidth;
}

int ZTexture::getHeight()
{
    return zHeight;
}

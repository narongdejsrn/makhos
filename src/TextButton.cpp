//
//  TextButton.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 8/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/TextButton.hpp"

using namespace makhos;

TextButton::TextButton(SDL_Renderer* mRenderer) {
    this->renderer = mRenderer;
    this->textTexture = ZTexture(this->renderer);
    this->textColor = {0, 0, 0};
    
    gameEvent = GameEvent::getInstance();
    gameEvent->attach(this);
}

void TextButton::setFont(const char * fontPath, int fontSize) {
//
    if(this->font != nullptr) {
        // Delete old font before create a new one
        TTF_CloseFont(this->font);
    }
    
    
    this->font = TTF_OpenFont(fontPath, fontSize);
    if(this->font == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError() );
        throw TTF_FONT_ERROR;
    }
}

void TextButton::draw() {
    // check if font is assign
    if(this->font == NULL) {
        printf("You must assign a font before call a draw");
        throw TTF_FONT_ERROR;
    }
    
    if( !this->textTexture.loadFromRenderedText( this->text, this->font, this->textColor ) )
    {
        printf( "Failed to render text texture!\n" );
        throw TTF_FONT_RENDER_ERROR;
    }
    
    SDL_Rect fillRect = { x, y, this->textTexture.getWidth() + 20, this->textTexture.getHeight() + 20 };

    switch (mouseStatus) {
        case BUTTON_SPRITE_MOUSE_OVER_MOTION:
            SDL_SetRenderDrawColor( this->renderer, backgroundHoverColor.r, backgroundHoverColor.g, backgroundHoverColor.b, backgroundHoverColor.a );
            break;
        case BUTTON_SPRITE_MOUSE_DOWN:
            SDL_SetRenderDrawColor( this->renderer, backgroundClickColor.r, backgroundClickColor.g, backgroundClickColor.b, backgroundClickColor.a );
            break;
        default:
            SDL_SetRenderDrawColor( this->renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a );
    }
    SDL_RenderFillRect( this->renderer, &fillRect );
    this->textTexture.render( x + 10, y + 10 );
}

void TextButton::handleEvent(SDL_Event *event) {
    if( event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
        //Get mouse position
        int mouseX, mouseY;
        SDL_GetMouseState( &mouseX, &mouseY );

        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( mouseX < x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( mouseX > x + this->textTexture.getWidth() + 20 )
        {
            inside = false;
        }
        //Mouse above the button
        else if( mouseY < y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( mouseY > y + this->textTexture.getHeight() + 20 )
        {
            inside = false;
        }

        //Mouse is outside button
        if( !inside )
        {
            mouseStatus = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( event->type )
            {
                case SDL_MOUSEMOTION:
                    mouseStatus = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                    break;
            
                case SDL_MOUSEBUTTONDOWN:
                    mouseStatus = BUTTON_SPRITE_MOUSE_DOWN;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouseStatus = BUTTON_SPRITE_MOUSE_UP;
                    if(this->clickEvent) {
                        this->clickEvent();
                    } else {
                        printf("no click event\n");
                    }
                    break;
                default:
                    std::cout << "event type: " << event->type << '\n';
                    break;
            }
        }
    }
}

TextButton::~TextButton() {
    gameEvent->detach(this);
    this->textTexture.~ZTexture();
    TTF_CloseFont(this->font);
    this->font = NULL;
}

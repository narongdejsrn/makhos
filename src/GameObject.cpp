//
//  GameObject.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 11/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/GameObject.hpp"

using namespace makhos;

GameObject::GameObject(ZTexture *texture) {
    this->texture = texture;
}

GameObject::GameObject(Primitive *primitive) {
    this->primitive = primitive;
}

void GameObject::draw() {
    if(this->texture != nullptr) {
        this->texture->render(x, y);
    }
    
    if(this->primitive != nullptr) {
        this->primitive->draw();
    }
}

void GameObject::draw(SDL_Rect *clip) {
    if(this->texture != nullptr && clip) {
        this->texture->render(x, y, clip);
    }
}

void GameObject::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void GameObject::attachClickEvent(std::function<void ()> clickEvent) {
    this->clickEvent = clickEvent;
    GameEvent *gameEvent = GameEvent::getInstance();
    gameEvent->attach(this);
}

int GameObject::getX() {
    return this->x;
}

int GameObject::getY() {
    return this->y;
}

int GameObject::getWidth() {
    if(this->texture != nullptr) {
        return this->texture->getWidth();
    }
    
    if(this->primitive != nullptr) {
        return this->primitive->getWidth();
    }
    
    return -1;
}

int GameObject::getHeight() {
    if(this->texture != nullptr) {
        return this->texture->getHeight();
    }
    
    if(this->primitive != nullptr) {
        return this->primitive->getHeight();
    }
    
    return -1;
}

void GameObject::handleEvent(SDL_Event *event){
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
        else if( mouseX > x + this->getWidth() )
        {
            inside = false;
        }
        //Mouse above the button
        else if( mouseY < y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( mouseY > y + this->getHeight() )
        {
            inside = false;
        }

        //Mouse is outside button
        if( !inside )
        {
            //
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( event->type )
            {
                case SDL_MOUSEMOTION:
//                    mouseStatus = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                    break;
            
                case SDL_MOUSEBUTTONDOWN:
//                    mouseStatus = BUTTON_SPRITE_MOUSE_DOWN;
                    break;
                case SDL_MOUSEBUTTONUP:
//                    mouseStatus = BUTTON_SPRITE_MOUSE_UP;
                    if(this->clickEvent) {
                        clickEvent();
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

//
//  GameEvent.cpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 9/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#include "../include/GameEvent.hpp"
#include <iostream>
#include <string>


using namespace makhos;

GameEvent* GameEvent::_ginstance = nullptr;
std::mutex GameEvent::_mutex;
std::stack <EventObserver *> GameEvent::attachQueue;
std::stack <EventObserver *> GameEvent::detachQueue;
 
GameEvent *GameEvent::getInstance() {
    if(_ginstance == nullptr) {
        printf("Creating new game event\n");
        std::lock_guard<std::mutex> lock(GameEvent::_mutex);
        if(_ginstance == nullptr) {
            _ginstance = new GameEvent();
        }
    }
    return _ginstance;
}

GameEvent::GameEvent() {
}

void GameEvent::attach(EventObserver *observer) {
    attachQueue.push(observer);
}

void GameEvent::detach(EventObserver *observer) {
    detachQueue.push(observer);
}

void GameEvent::notify(SDL_Event* event) {
    while(detachQueue.size() > 0) {
        _observers.remove(detachQueue.top());
        detachQueue.pop();
    }
    
    while(attachQueue.size() > 0) {
        _observers.push_back(attachQueue.top());
        attachQueue.pop();
    }
    
    std::list<EventObserver *>::iterator iterator = _observers.begin();
    while (iterator != _observers.end()) {
      (*iterator)->handleEvent(event);
      ++iterator;
    }
}



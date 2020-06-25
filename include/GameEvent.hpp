//
//  GameEvent.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 9/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef GameEvent_hpp
#define GameEvent_hpp

#include <stdio.h>
#include <list>
#include <string>
#include <iostream>
#include <stack>

#include <SDL2/SDL.h>

namespace makhos {

class EventObserver {
    public:
     virtual void handleEvent(SDL_Event* e) = 0;
};

class GameEvent {
public:
    GameEvent(GameEvent &other) = delete;
    void operator=(const GameEvent &) = delete;
    static GameEvent *getInstance();
    void attach(EventObserver *observer);
    void detach(EventObserver *observer);
    void notify(SDL_Event* e);
    
protected:
    GameEvent();
    ~GameEvent();
private:
    std::list<EventObserver *> _observers;
    static GameEvent* _ginstance;
    static std::mutex _mutex;
    static std::stack<EventObserver *> detachQueue;
    static std::stack<EventObserver *> attachQueue;
};

}

#endif /* GameEvent_hpp */

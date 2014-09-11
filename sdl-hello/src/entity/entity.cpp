//
//  entity.cpp
//  sdl-hello
//
//  Created by Dan Ristic on 9/10/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#include "entity.h"
#include "loader.h"

Entity::Entity() {
    _x = 0;
    _y = 0;
}

void Entity::loadTexture(std::string name) {
    _tex = Loader::getInstance().loadImage(name);
}

void Entity::moveTo(double x, double y) {
    _startX = _x;
    _startY = _y;
    _destX = (int)x;
    _destY = (int)y;
}

void Entity::render(SDL_Renderer *renderer) {
    if (_tex) {
        if (_x != _destX) _x += (_destX - _startX) * _speed;
        if (_y != _destY) _y += (_destY - _startY) * _speed;
        
        if (_destX - _x < 3) _x = _destX;
        if (_destY - _y < 3) _y = _destY;
        
        SDL_Rect rect;
        rect.x = _x;
        rect.y = _y;
        SDL_QueryTexture(_tex, NULL, NULL, &rect.w, &rect.h);
        SDL_RenderCopy(renderer, _tex, NULL, &rect);
    }
}

Entity::~Entity() {
    _x = -1;
    _y = -1;
    
    SDL_DestroyTexture(_tex);
    _tex = NULL;
}
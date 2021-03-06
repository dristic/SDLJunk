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
        SDL_Rect rect;
        rect.x = _x;
        rect.y = _y;
        SDL_QueryTexture(_tex, NULL, NULL, &rect.w, &rect.h);
        SDL_RenderCopy(renderer, _tex, NULL, &rect);
    }
}

void Entity::update(double delta) {
    if (_x < _destX) _x += _speed * delta;
    if (_x > _destX) _x -= _speed * delta;
    if (_y < _destY) _y += _speed * delta;
    if (_y > _destY) _y -= _speed * delta;
    
    if (_destX - _x < 2 && _destX - _x > -2) _x = _destX;
    if (_destY - _y < 2 && _destY - _y > -2) _y = _destY;
}

Entity::~Entity() {
    _x = -1;
    _y = -1;
    
    SDL_DestroyTexture(_tex);
    _tex = NULL;
}
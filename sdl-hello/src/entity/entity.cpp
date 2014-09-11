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
    _tex = Loader::getInstance().loadImage("pixel_art.bmp");
}

void Entity::render(SDL_Renderer *renderer) {
    SDL_Rect rect;
    rect.x = _x;
    rect.y = _y;
    SDL_QueryTexture(_tex, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, _tex, NULL, &rect);
}

Entity::~Entity() {
    _x = -1;
    _y = -1;
    
    SDL_DestroyTexture(_tex);
    _tex = NULL;
}
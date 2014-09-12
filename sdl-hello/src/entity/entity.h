//
//  entity.h
//  sdl-hello
//
//  Created by Dan Ristic on 9/10/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#ifndef __sdl_hello__entity__
#define __sdl_hello__entity__

#include <iostream>
#include <SDL2/SDL.h>

class Entity {
public:
    Entity();
    ~Entity();
    
    void loadTexture(std::string name);
    void moveTo(double x, double y);
    void render(SDL_Renderer *renderer);
    void update(double delta);
    
private:
    int _x;
    int _y;
    int _startX = 0;
    int _startY = 0;
    int _destX = 0;
    int _destY = 0;
    double _speed = 500;
    SDL_Texture *_tex = 0;
};

#endif /* defined(__sdl_hello__entity__) */

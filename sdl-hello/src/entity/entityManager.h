//
//  entityManager.h
//  sdl-hello
//
//  Created by thock on 8/9/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#ifndef __sdl_hello__entityManager__
#define __sdl_hello__entityManager__

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

#include "entity.h"

class EntityManager {
public:
    EntityManager();
    ~EntityManager();
    
    Entity* createEntity();
    void render(SDL_Renderer *renderer);
    void update(double delta);
    
private:
    std::vector<Entity*> _entities;
};

#endif /* defined(__sdl_hello__entityManager__) */

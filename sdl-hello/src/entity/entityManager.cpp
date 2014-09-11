//
//  entityManager.cpp
//  sdl-hello
//
//  Created by thock on 8/9/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#include "entityManager.h"

EntityManager::EntityManager() {
    
}

Entity* EntityManager::createEntity() {
    Entity* entity = new Entity();
    _entities.push_back(entity);
    return entity;
}

void EntityManager::render(SDL_Renderer *renderer) {
    int i;
    for (i = 0; i < _entities.size(); i++) {
        _entities[i]->render(renderer);
    }
}

EntityManager::~EntityManager() {
    int i;
    for (i = 0; i < _entities.size(); i++) {
        _entities[i]->~Entity();
    }
    
    _entities.resize(0);
}
//
//  Game.cpp
//  sdl-hello
//
//  Created by thock on 8/9/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#include "Game.h"
#include "bindings.h"
#include "file.h"
#include "loader.h"
#include <SDL2/SDL.h>
#include <mach-o/dyld.h>

static const uint8_t SUCCESS = 0;

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != SUCCESS) {
        std::cout << "SDL_Init failure! " << SDL_GetError() << std::endl;
        return false;
    }
    
    mainWindow = SDL_CreateWindow(title, xpos, ypos, height, width, flags);
    if(!mainWindow) {
        std::cout << "SDL_CreateWindow failure! " << SDL_GetError() << std::endl;
        return false;
    }
    
    mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    if (!mainRenderer) {
        std::cout << "SDL_CreateRenderer failure! " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "Renderer created successfully!" << std::endl;
    
    if (SUCCESS != SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255)) {
        std::cout << "SDL_SetRenderDrawColor failure! " << SDL_GetError() << std::endl;
    }
    
    std::cout << "Game running!" << std::endl;
    
    entityManager = new EntityManager();
    Loader::getInstance().initialize(mainRenderer);
    engine = jsbinding::createEngine(this);

    gameRunning = true;
    
    return true;
}

void Game::render() {
    if (gameRunning) {
        SDL_RenderClear(mainRenderer);
        
        entityManager->render(mainRenderer);
        
        SDL_RenderPresent(mainRenderer);
    }
}

bool Game::running() {
    return gameRunning;
}

void Game::update() {

}

void Game::clean() {
    std::cout << "cleaning game" << std::endl;
    SDL_DestroyWindow(mainWindow);
    SDL_DestroyRenderer(mainRenderer);
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                gameRunning = false;
                break;
            default:
                break;
        }
        
        const UInt8* currentStates = SDL_GetKeyboardState(NULL);
        
        if (currentStates[SDL_SCANCODE_R]) {
            
        } else if (currentStates[SDL_SCANCODE_SPACE]) {
            gameRunning = false;
        }
    }
}

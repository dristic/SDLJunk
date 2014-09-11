//
//  loader.cpp
//  sdl-hello
//
//  Created by Dan Ristic on 9/10/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#include "loader.h"
#include "file.h"

void Loader::initialize(SDL_Renderer *renderer, SDL_Window *window) {
    _renderer = renderer;
    _window = window;
}

SDL_Texture* Loader::loadImage(std::string name) {
    SDL_Texture* tex;
    std::string imagePath = file::GetPath(name);
    
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (!bmp) {
        std::cout << "Failed to create image. " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }
    
    tex = SDL_CreateTextureFromSurface(_renderer, bmp);
    SDL_FreeSurface(bmp);
    if (!tex) {
        std::cout << "Failed to create texture. " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        SDL_Quit();
        
        return NULL;
    }
    
    std::cout << "Created texture successfully." << std::endl;
    
    return tex;
}
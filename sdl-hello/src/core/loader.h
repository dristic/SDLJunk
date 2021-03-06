//
//  loader.h
//  sdl-hello
//
//  Created by Dan Ristic on 9/10/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#ifndef __sdl_hello__loader__
#define __sdl_hello__loader__

#include <iostream>
#include <SDL2/SDL.h>

class Loader {
public:
    static Loader& getInstance() {
        static Loader instance;
        return instance;
    }
    
    void initialize(SDL_Renderer* renderer);
    SDL_Texture* loadImage(std::string name);
    
private:
    Loader() {};
    Loader(Loader const&);
    void operator=(Loader const&);
    
    // This is owned by Game and should not be destroyed!
    SDL_Renderer* _renderer;
};

#endif /* defined(__sdl_hello__loader__) */

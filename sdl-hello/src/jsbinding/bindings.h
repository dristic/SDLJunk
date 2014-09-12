//
//  bindings.h
//  sdl-hello
//
//  Created by Dan Ristic on 9/8/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#ifndef __sdl_hello__bindings__
#define __sdl_hello__bindings__

#include "Game.h"
#include "jsengine.h"
#include <JavaScriptCore/JavaScriptCore.h>
#include <JavaScriptCore/JSValueRef.h>

namespace jsbinding {
    JSEngine* createEngine(Game *game);
    void mouseClick(JSContextRef ctx, int x, int y);
}

static JSObjectRef callback;

#endif /* defined(__sdl_hello__bindings__) */

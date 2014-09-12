//
//  jsengine.h
//  sdl-hello
//
//  Created by Dan Ristic on 9/11/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#ifndef __sdl_hello__jsengine__
#define __sdl_hello__jsengine__

#include <iostream>
#include <JavaScriptCore/JavaScriptCore.h>

class JSEngine {
public:
    JSEngine();
    ~JSEngine();
    
    JSContextRef getContext();
    std::string evaluateScript(std::string source);
    JSClassRef createClass(std::string name, const JSStaticFunction* functions);
    bool createGlobal(std::string name, const JSStaticFunction* functions, void* data);
    
private:
    JSGlobalContextRef _ctx;
    JSObjectRef _global;
};

#endif /* defined(__sdl_hello__jsengine__) */

//
//  jsengine.cpp
//  sdl-hello
//
//  Created by Dan Ristic on 9/11/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#include "jsengine.h"

JSEngine::JSEngine() {
    _ctx = JSGlobalContextCreate(NULL);

    _global = JSContextGetGlobalObject(_ctx);
    assert(_global);
}

JSContextRef JSEngine::getContext() {
    return _ctx;
}

JSClassRef JSEngine::createClass(std::string name, const JSStaticFunction* functions) {
    JSClassDefinition classDefinition = {
        0, kJSClassAttributeNone, name.c_str(), 0, 0, functions,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    JSClassRef classRef = JSClassCreate(&classDefinition);
    
    return classRef;
}

bool JSEngine::createGlobal(std::string name, const JSStaticFunction* functions, void* data) {
    JSClassRef classRef = this->createClass(name, functions);
    
    JSObjectRef classObj = JSObjectMake(_ctx, classRef, data);
    JSStringRef className = JSStringCreateWithUTF8CString(name.c_str());
    
    JSValueRef exception = NULL;
    JSObjectSetProperty(_ctx, _global, className, classObj, kJSPropertyAttributeNone, &exception);
    
    if (exception) {
        return false;
    } else {
        return true;
    }
}

std::string JSEngine::evaluateScript(std::string source) {
    std::string result;
    JSStringRef jsSource = JSStringCreateWithUTF8CString(source.c_str());
    
    std::cout << "Executing JS code." << std::endl;
    
    JSValueRef exception = NULL;
    JSValueRef resultValue = JSEvaluateScript(_ctx, jsSource, NULL, NULL, 1, &exception);
    
    if (exception) {
        JSStringRef str = JSValueToStringCopy(_ctx, exception, NULL);
        size_t errSize = JSStringGetLength(str);
        char errStr[errSize];
        JSStringGetUTF8CString(str, errStr, errSize);
        std::cout << "[ERROR] " << errStr << std::endl;
        JSStringRelease(str);
    }
    
    JSStringRef resultString = JSValueToStringCopy(_ctx, resultValue, NULL);
    size_t size = JSStringGetLength(resultString);
    char otherStr[size];
    JSStringGetUTF8CString(resultString, otherStr, size);
    result = std::string(otherStr);
    JSStringRelease(resultString);
    
    return result;
}

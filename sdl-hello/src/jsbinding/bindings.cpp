//
//  bindings.cpp
//  sdl-hello
//
//  Created by Dan Ristic on 9/8/14.
//  Copyright (c) 2014 landon. All rights reserved.
//

#include "bindings.h"
#include "file.h"
#include "JavaScriptCore/JSValueRef.h"
#include <iostream>
#include <fstream>
#include <string>

JSClassRef ConsoleClass()
{
    static JSStaticFunction staticFunctions[] = {
        { "log", console_log, kJSPropertyAttributeNone }
    };
    
    static JSClassDefinition classDefinition = {
        0, kJSClassAttributeNone, "console", 0, 0, staticFunctions,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    static JSClassRef consoleClass = JSClassCreate(&classDefinition);
    return consoleClass;
}

/**
 * The callback from JavaScriptCore.  We told JSC to call this function
 * whenever it sees "console.log".
 */
static JSValueRef console_log(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, ConsoleClass()))
        return JSValueMakeUndefined(ctx);
    
    if (argumentCount < 1)
        return JSValueMakeUndefined(ctx);
    
    // Convert the result into a string for display.
    if (!JSValueIsString(ctx, arguments[0]))
        return JSValueMakeUndefined(ctx);
    
    JSStringRef temp = JSValueToStringCopy (ctx, arguments[0], exception);
    if (exception && *exception)
        return JSValueMakeUndefined(ctx);
    
    printf("Got console log %s\n", (char*)JSStringGetCharactersPtr(temp));
    
    JSStringRelease(temp);
    
    return JSValueMakeUndefined(ctx);
}

JSClassRef EngineClass()
{
    static JSStaticFunction staticFunctions[] = {
        { "createEntity", create_entity, kJSPropertyAttributeNone }
    };
    
    static JSClassDefinition classDefinition = {
        0, kJSClassAttributeNone, "engine", 0, 0, staticFunctions,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    static JSClassRef engineClass = JSClassCreate(&classDefinition);
    return engineClass;
}

static JSValueRef create_entity(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, EngineClass()))
        return JSValueMakeUndefined(ctx);
    
    Game* game = (Game*)JSObjectGetPrivate(thisObject);
    assert(game);
    
    Entity* entity = game->entityManager->createEntity();
    
    JSClassRef entityClass = EntityClass();
    JSObjectRef entityRef = JSObjectMake(ctx, entityClass, reinterpret_cast<void*>(entity));
    
    return entityRef;
}

JSClassRef EntityClass()
{
    static JSStaticFunction staticFunctions[] = {
        { "loadTexture", load_texture, kJSClassAttributeNone },
        { "moveTo", move_to, kJSClassAttributeNone }
    };
    
    static JSClassDefinition classDefinition = {
        0, kJSClassAttributeNone, "entity", 0, 0, staticFunctions,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    static JSClassRef entityClass = JSClassCreate(&classDefinition);
    return entityClass;
}

static JSValueRef load_texture(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, EntityClass()))
        return JSValueMakeUndefined(ctx);
    
    if (argumentCount < 1)
        return JSValueMakeUndefined(ctx);
    
    // Convert the result into a string for display.
    if (!JSValueIsString(ctx, arguments[0]))
        return JSValueMakeUndefined(ctx);
    
    JSStringRef temp = JSValueToStringCopy(ctx, arguments[0], exception);
    if (exception && *exception)
        return JSValueMakeUndefined(ctx);
    
    Entity* entity = (Entity*)JSObjectGetPrivate(thisObject);
    assert(entity);
    
    char imageName[1024];
    JSStringGetUTF8CString(temp, imageName, sizeof(char[1024]));
    printf("Entity loadImage %s\n", imageName);
    
    entity->loadTexture(imageName);
    
    JSStringRelease(temp);
    
    return JSValueMakeUndefined(ctx);
}

static JSValueRef move_to(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, EntityClass()))
        return JSValueMakeUndefined(ctx);
    
    if (argumentCount < 2)
        return JSValueMakeUndefined(ctx);
    
    double x = JSValueToNumber(ctx, arguments[0], exception);
    if (exception && *exception)
        return JSValueMakeUndefined(ctx);
    
    double y = JSValueToNumber(ctx, arguments[1], exception);
    if (exception && *exception)
        return JSValueMakeUndefined(ctx);
    
    Entity* entity = (Entity*)JSObjectGetPrivate(thisObject);
    assert(entity);
    
    printf("Moving to %f, %f\n", x, y);
    
    entity->moveTo(x, y);
    
    return JSValueMakeUndefined(ctx);
}

std::string loadData() {
    std::string line;
    std::string result = "";
    std::string input = file::GetPath("myscript.js");
    std::ifstream openFile;
    openFile.open(input.c_str(), std::ios::in);
    
    if (openFile.is_open()) {
        while (std::getline(openFile, line))
        {
            result += line;
        }
        
        openFile.close();
    } else {
        std::cout << "[ERROR] Unable to open file." << std::endl;
        return "";
    }
    
    return result;
}

void doBindings(Game* game) {
    JSGlobalContextRef ctx = JSGlobalContextCreate(NULL);
    
    JSObjectRef global = JSContextGetGlobalObject(ctx);
    assert(global);
    
    JSClassRef consoleClass = ConsoleClass();
    assert(consoleClass);
    
    JSObjectRef scriptObj = JSObjectMake(ctx, consoleClass, NULL);
    assert(scriptObj);
    
    JSStringRef consoleName = JSStringCreateWithUTF8CString("console");
    JSValueRef* except = 0;
    JSObjectSetProperty(ctx, global, consoleName, scriptObj, kJSPropertyAttributeNone, except);
    
    JSClassRef engineClass = EngineClass();
    assert(engineClass);
    JSObjectRef engineObj = JSObjectMake(ctx, engineClass, reinterpret_cast<void*>(game));
    assert(engineObj);
    
    JSStringRef engineName = JSStringCreateWithUTF8CString("engine");
    JSObjectSetProperty(ctx, global, engineName, engineObj, kJSPropertyAttributeNone, NULL);
    
    std::string functionString = loadData();
    JSStringRef jsString = JSStringCreateWithUTF8CString(functionString.c_str());
    
    std::cout << "Executing JS code..." << std::endl;
    
    JSValueRef exception = NULL;
    JSValueRef ref = JSEvaluateScript(ctx, jsString, NULL, NULL, 1, &exception);
    
    if (exception) {
        JSStringRef str = JSValueToStringCopy(ctx, exception, NULL);
        
        char myStr[1024];
        JSStringGetUTF8CString(str, myStr, sizeof(char[1024]));
        
        std::cout << "Error hit" << myStr << std::endl;
        
        JSStringRelease(str);
    }
    
    JSStringRef strRef = JSValueToStringCopy(ctx, ref, NULL);
    char otherStr[1024];
    JSStringGetUTF8CString(strRef, otherStr, sizeof(char[1024]));
    
    JSStringRelease(strRef);
}


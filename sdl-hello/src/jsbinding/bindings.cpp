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

static JSClassRef entityClass = NULL;

/**
 * The callback from JavaScriptCore.  We told JSC to call this function
 * whenever it sees "console.log".
 */
static JSValueRef console_log(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception)
{
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

static JSValueRef on_mouse_down(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception) {
    if (argumentCount < 1)
        return JSValueMakeUndefined(ctx);
    
    JSValueProtect(ctx, arguments[0]);
    callback = JSValueToObject(ctx, arguments[0], NULL);
    
    return JSValueMakeUndefined(ctx);
}

void jsbinding::mouseClick(JSContextRef ctx, int x, int y) {
    JSValueRef xRef = JSValueMakeNumber(ctx, (double)x);
    JSValueRef yRef = JSValueMakeNumber(ctx, (double)y);
    
    const JSValueRef args[] = { xRef, yRef };
    
    if (JSObjectIsFunction(ctx, callback)) {
        JSObjectCallAsFunction(ctx, callback, NULL, 2, args, NULL);
    } else {
        std::cout << "No callback defined." << std::endl;
    }
}

static JSValueRef create_entity(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception)
{
    Game* game = (Game*)JSObjectGetPrivate(thisObject);
    assert(game);
    
    Entity* entity = game->entityManager->createEntity();
    
    JSObjectRef entityRef = JSObjectMake(ctx, entityClass, reinterpret_cast<void*>(entity));
    
    return entityRef;
}

static JSValueRef load_texture(JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef thisObject, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception)
{
    if (!JSValueIsObjectOfClass(ctx, thisObject, entityClass))
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
    if (!JSValueIsObjectOfClass(ctx, thisObject, entityClass))
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

std::string loadJavaScript() {
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

JSEngine* jsbinding::createEngine(Game *game) {
    JSEngine* engine = new JSEngine();
    
    // Create console class
    const JSStaticFunction consoleFunctions[] = {
        { "log", console_log, kJSPropertyAttributeNone }
    };
    engine->createGlobal("console", consoleFunctions, NULL);
    
    // Create engine class
    const JSStaticFunction engineFunctions[] = {
        { "createEntity", create_entity, kJSPropertyAttributeNone },
        { "onMouseDown", on_mouse_down, kJSPropertyAttributeNone }
    };
    engine->createGlobal("engine", engineFunctions, reinterpret_cast<void*>(game));
    
    // Create entity class
    const JSStaticFunction entityFunctions[] = {
        { "loadTexture", load_texture, kJSClassAttributeNone },
        { "moveTo", move_to, kJSClassAttributeNone }
    };
    
    entityClass = engine->createClass("entity", entityFunctions);
    
    // Finally, run our script file
    engine->evaluateScript(loadJavaScript());
    
    return engine;
}

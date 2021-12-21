/*

YarpJS is the object from which most other yarpjs objects inherit basic properties.

*/

#ifndef YARP_JS_H
#define YARP_JS_H


#include <nan.h>
#include <string>


/*

    Two macros that implement NAN boilerplate code for creating
    - YARPJS_BASE: a new object from scratch
    - YARPJS_INIT: a new object inheriting from a parent

    These two macros are used to instantiate new yarpjs object in a way that NAN recognizes.
    In particular...
    - YARPJS_BASE: the new JS object is created with prototype the standard JS Object object.
    - YARPJS_INIT(_AAA,_js_name,_AAA_Parent): the new JS object is created with prototype the YARPJS _AAA_Parent.

*/
#define YARPJS_BASE(_AAA,_js_name)                                                                   \
static Nan::Persistent<v8::FunctionTemplate> constructor;                                            \
static NAN_MODULE_INIT(Init) {                                                                       \
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);                       \
    tpl->SetClassName(Nan::New(_js_name).ToLocalChecked());                                          \
    tpl->InstanceTemplate()->SetInternalFieldCount(1);                                               \
    setJSMethods(tpl);                                                                               \
    constructor.Reset(tpl);                                                                          \
    Nan::Set(target, Nan::New(_js_name).ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());   \
}                                                                                                    \
static NAN_METHOD(New) {                                                                             \
    if (info.IsConstructCall()) {                                                                    \
        _AAA* obj = new _AAA(info);                                                                  \
        obj->Wrap(info.This());                                                                      \
        info.GetReturnValue().Set(info.This());                                                      \
    } else {                                                                                         \
        const int argc = 1;                                                                          \
        v8::Local<v8::Value> argv[argc] = {info[0]};                                                 \
        v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(constructor)).ToLocalChecked();     \
        info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext(), argc, argv).ToLocalChecked()); \
    }                                                                                                \
}

#define YARPJS_INIT(_AAA,_js_name,_AAA_Parent)                                                       \
static Nan::Persistent<v8::FunctionTemplate> constructor;                                            \
static NAN_MODULE_INIT(Init) {                                                                       \
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);                       \
    v8::Local<v8::FunctionTemplate> cons = Nan::New(_AAA_Parent::constructor);                       \
    tpl->Inherit(cons);                                                                              \
    tpl->SetClassName(Nan::New(_js_name).ToLocalChecked());                                          \
    tpl->InstanceTemplate()->SetInternalFieldCount(1);                                               \
    setJSMethods(tpl);                                                                               \
    constructor.Reset(tpl);                                                                          \
    Nan::Set(target, Nan::New(_js_name).ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());   \
}                                                                                                    \
static NAN_METHOD(New) {                                                                             \
    if (info.IsConstructCall()) {                                                                    \
        _AAA* obj = new _AAA(info);                                                                  \
        obj->Wrap(info.This());                                                                      \
        info.GetReturnValue().Set(info.This());                                                      \
    } else {                                                                                         \
        const int argc = 1;                                                                          \
        v8::Local<v8::Value> argv[argc] = {info[0]};                                                 \
        v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(constructor)).ToLocalChecked();     \
        info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext(), argc, argv).ToLocalChecked()); \
    }                                                                                                \
}




class YarpJS : public Nan::ObjectWrap {
private:


    // Function in charge of registering the node.js methods (exposed to the user).
    // Called when creating the object.
    // Here empty because this is the root object and is not designed to be used
    // directly but only inherited from.

    static void setJSMethods(v8::Local<v8::FunctionTemplate> &tpl)
    {}

public:

    // empty constructor
    explicit YarpJS()
    {}

    // "copy" constructor (i.e. copy from another object)
    explicit YarpJS(const Nan::FunctionCallbackInfo<v8::Value> &info)
    {}

    // destructor
    ~YarpJS()
    {}


    // NAN Boilerplate code to create the object.
    YARPJS_BASE(YarpJS,"YarpJS")
};




#endif

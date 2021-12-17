

/*
    Wrapper for the yarp Bottle object. 
*/



#ifndef YARP_JS_BOTTLE_H
#define YARP_JS_BOTTLE_H


#include <nan.h>
#include <string>

#include "YarpJS_Wrapper.h"
#include <yarp/os/Bottle.h>


class YarpJS_Bottle: public YarpJS_Wrapper<yarp::os::Bottle>
{
private:

    /*  setJSMethods(v8::Local<v8::FunctionTemplate> &tpl)
    
        Function in charge of registering the node.js methods (exposed to the user).
        Called when creating the object.
    
        The signature is 
        
        Nan::SetPrototypeMethod(tpl, "<JS function name>", <C++ function name>);

        with 
        - <JS function name> the name of the function (method) to associate the JS object.

        - <C++ function name> handle to the C++ class method to call when the JS function is called.
    */

    static void setJSMethods(v8::Local<v8::FunctionTemplate> &tpl)
    {
        Nan::SetPrototypeMethod(tpl, "copy", Copy);
        Nan::SetPrototypeMethod(tpl, "toArray", ToArray);
        Nan::SetPrototypeMethod(tpl, "toString", ToString);
        Nan::SetPrototypeMethod(tpl, "toObject", ToArray);
        Nan::SetPrototypeMethod(tpl, "fromString", FromString);
        Nan::SetPrototypeMethod(tpl, "getObjType", GetObjType);
    }

public:

    // Create the Bottle from a string (using the yarp implementation)
    void fromString(const Nan::FunctionCallbackInfo<v8::Value> &info)
    {
        v8::String::Utf8Value _bottle_string(v8::Isolate::GetCurrent(), info[0]->ToString(Nan::GetCurrentContext()).ToLocalChecked());
        this->getYarpObj()->fromString(*_bottle_string);
    }

    // "Translating" the current Bottle to a standard javascript array
    static void toArray(const yarp::os::Bottle *bObj, v8::Local<v8::Array> &bArr);


    // Copy Constructor from a node.js bottle. 
    explicit YarpJS_Bottle(const Nan::FunctionCallbackInfo<v8::Value> &info)
    {
        this->setYarpObj(new yarp::os::Bottle());
        this->fromString(info);
    }



    // Copy Constructor from a yarp Bottle. 
    explicit YarpJS_Bottle(yarp::os::Bottle &bottle)
    {
        this->setYarpObj(new yarp::os::Bottle(bottle));
    }

    // Destructor
    ~YarpJS_Bottle()
    {}

    

    // Actual 
    static NAN_METHOD(Copy);
    static NAN_METHOD(ToArray);
    static NAN_METHOD(ToString);
    static NAN_METHOD(FromString);
    static NAN_METHOD(GetObjType);



    // NAN Stuff
    YARPJS_INIT(YarpJS_Bottle,"Bottle",YarpJS)
};






#endif

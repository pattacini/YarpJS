

#ifndef YARPJS_BUFFERED_PORT_SOUND_JS_H
#define YARPJS_BUFFERED_PORT_SOUND_JS_H


#include <nan.h>

#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Sound.h>

#include "YarpJS_BufferedPort_Bottle.h"
#include "YarpJS_Sound.h"

#include <stdio.h>
#include <string>


class YarpJS_BufferedPort_Sound :  public YarpJS_BufferedPort<yarp::sig::Sound> 
{

private:

    static void setJSMethods(v8::Local<v8::FunctionTemplate> &tpl)
    {
        Nan::SetPrototypeMethod(tpl,"open",Open);
        Nan::SetPrototypeMethod(tpl,"close",Close);
        Nan::SetPrototypeMethod(tpl,"onRead",SetOnReadCallback);
        Nan::SetPrototypeMethod(tpl,"write",Write);
        Nan::SetPrototypeMethod(tpl,"prepare",Prepare);

	//HERE
        Nan::SetPrototypeMethod(tpl,"setStrict",SetStrict);
        Nan::SetPrototypeMethod(tpl,"writeStrict",WriteStrict); //not working
        Nan::SetPrototypeMethod(tpl,"waitForWrite",WaitForWrite);
        
        // Nan::SetPrototypeMethod(tpl,"onRPC",SetOnRPCCallback);
        // Nan::SetPrototypeMethod(tpl,"reply",Reply);

    }
    
    std::string                         port_name;


public:

    virtual void _callback_onRead(std::vector<v8::Local<v8::Value> > &tmp_arguments);

    explicit YarpJS_BufferedPort_Sound(const Nan::FunctionCallbackInfo<v8::Value> &info)
    {
    }

    explicit YarpJS_BufferedPort_Sound()
    {}


    ~YarpJS_BufferedPort_Sound()
    {}


    // NAN Stuff
    static NAN_METHOD(Open);
    static NAN_METHOD(Close);
    static NAN_METHOD(SetOnReadCallback);
    static NAN_METHOD(Write);
    static NAN_METHOD(Prepare);

    //HERE
    static NAN_METHOD(SetStrict);
    static NAN_METHOD(WriteStrict); //not working
    static NAN_METHOD(WaitForWrite);

    // static NAN_METHOD(SetOnRPCCallback);
    // static NAN_METHOD(Reply);

    YARPJS_INIT(YarpJS_BufferedPort_Sound,"BufferedPortSound",YarpJS)
    
};



#endif



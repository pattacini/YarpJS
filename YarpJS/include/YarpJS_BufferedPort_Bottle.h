

#ifndef YARPJS_BUFFERED_PORT_BOTTLE_JS_H
#define YARPJS_BUFFERED_PORT_BOTTLE_JS_H


#include <nan.h>

#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>

#include "YarpJS_BufferedPort.h"

#include <stdio.h>
#include <string>



#include "YarpJS_Bottle.h"



// rpc replier

class _YarpJS_RPCReplier : public yarp::os::PortReader {

    YarpJS_Callback<_YarpJS_RPCReplier>             *callback;

    yarp::os::Bottle                                datum;
    yarp::os::Bottle                                reply_datum;


    yarp::os::Mutex                                 mutex_reply;


    // internal read callback
    virtual bool read(yarp::os::ConnectionReader& connection) {
        
        bool ok = datum.read(connection);
        if (!ok) return false;


        callback->callCallback();

        //wait for reply
        mutex_reply.lock();

        // reply 
        yarp::os::ConnectionWriter *returnToSender = connection.getWriter();
        if (returnToSender!=NULL) {
            reply_datum.write(*returnToSender);
        }

        reply_datum.clear();
        return true;
    }


    virtual void _callback_onRead(std::vector<v8::Local<v8::Value> > &tmp_arguments)
    {
        const int argc = 1;   
        v8::Local<v8::Value> argv[argc] = {Nan::New<v8::String>(this->datum.toString()).ToLocalChecked()};
        v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(YarpJS_Bottle::constructor)).ToLocalChecked();
        tmp_arguments.push_back(cons->NewInstance(Nan::GetCurrentContext(), argc, argv).FromMaybe(argv[0]));
    }


public:
    _YarpJS_RPCReplier()
    {
        mutex_reply.lock();
        callback = new YarpJS_Callback<_YarpJS_RPCReplier>(this,&_YarpJS_RPCReplier::_callback_onRead);
    }


    ~_YarpJS_RPCReplier()
    {
        mutex_reply.unlock();
    }


    void reply(yarp::os::Bottle &_reply_datum)
    {
        // if we are waiting for a reply 
        if(!mutex_reply.tryLock())
            reply_datum.copy(_reply_datum);

        mutex_reply.unlock();
    }

    void setCallback(const Nan::FunctionCallbackInfo<v8::Value> &info)
    {
        callback->setCallback(info);
    }


};





class YarpJS_BufferedPort_Bottle :  public YarpJS_BufferedPort<yarp::os::Bottle> 
{
private:

    _YarpJS_RPCReplier RPCReplier;

    static void setJSMethods(v8::Local<v8::FunctionTemplate> &tpl)
    {
        Nan::SetPrototypeMethod(tpl,"open",Open);
        Nan::SetPrototypeMethod(tpl,"close",Close);
        Nan::SetPrototypeMethod(tpl,"onRead",SetOnReadCallback);
        Nan::SetPrototypeMethod(tpl,"write",Write);
        Nan::SetPrototypeMethod(tpl,"prepare",Prepare);
        
        Nan::SetPrototypeMethod(tpl,"onRPC",SetOnRPCCallback);
        Nan::SetPrototypeMethod(tpl,"reply",Reply);

    }
    
    std::string                             port_name;


public:

    virtual void _callback_onRead(std::vector<v8::Local<v8::Value> > &tmp_arguments);

    explicit YarpJS_BufferedPort_Bottle(const Nan::FunctionCallbackInfo<v8::Value> &info)
    {

        this->setReplier(RPCReplier);

    }

    explicit YarpJS_BufferedPort_Bottle()
    {

        this->setReplier(RPCReplier);

    }


    ~YarpJS_BufferedPort_Bottle()
    {}


    // NAN Stuff
    static NAN_METHOD(Open);
    static NAN_METHOD(Close);
    static NAN_METHOD(SetOnReadCallback);
    static NAN_METHOD(Write);
    static NAN_METHOD(Prepare);
    static NAN_METHOD(SetOnRPCCallback);
    static NAN_METHOD(Reply);

    YARPJS_INIT(YarpJS_BufferedPort_Bottle,"BufferedPortBottle",YarpJS)
};



#endif

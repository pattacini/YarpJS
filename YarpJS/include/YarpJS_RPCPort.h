

#ifndef YARPJS_RPCPORT_JS_H
#define YARPJS_RPCPORT_JS_H


#include <nan.h>

#include <yarp/os/Port.h>

#include <yarp/os/Bottle.h>

#include <stdio.h>
#include <string>

#include "YarpJS_Bottle.h"
#include "YarpJS_Callback.h"




// reader

class _YarpJS_PortReplier : public yarp::os::PortReader {

    YarpJS_Callback<_YarpJS_PortReplier>             *callback;

    yarp::os::Bottle                                datum;
    yarp::os::Bottle                                reply_datum;

    yarp::os::Mutex                                 mutex_reply;

    bool                                            isClosing;


    // internal read callback
    virtual bool read(yarp::os::ConnectionReader& connection) {
        
        bool ok = datum.read(connection);
        if (!ok) return false;


        callback->callCallback();

        //wait for reply
        mutex_reply.lock();


        // it it is closing just close
        if( this->isClosing )
            return true;

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
    _YarpJS_PortReplier()
    {
        isClosing = false;
        mutex_reply.lock();
        callback = new YarpJS_Callback<_YarpJS_PortReplier>(this,&_YarpJS_PortReplier::_callback_onRead);
    }


    ~_YarpJS_PortReplier()
    {
        this->interrupt();
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

    void interrupt()
    {

        this->isClosing=true;
        this->mutex_reply.unlock();

    }

};




// writer 


class YarpJS_RPCPort;

class _YarpJS_PortWriteReplier {

    YarpJS_Callback<_YarpJS_PortWriteReplier>       *callback;

    YarpJS_RPCPort                                  &parent;

    yarp::os::Bottle                                datum;
    yarp::os::Bottle                                reply_datum;

    bool                                            isClosing;



    virtual void _callback_onReplyReceived(std::vector<v8::Local<v8::Value> > &tmp_arguments)
    {
        const int argc = 1;   
        v8::Local<v8::Value> argv[argc] = {Nan::New<v8::String>(this->reply_datum.toString()).ToLocalChecked()};
        v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(YarpJS_Bottle::constructor)).ToLocalChecked();
        tmp_arguments.push_back(cons->NewInstance(Nan::GetCurrentContext(), argc, argv).FromMaybe(argv[0]));
    }

    virtual void _internal_Write();

public:
    _YarpJS_PortWriteReplier(YarpJS_RPCPort &_parent)
        :parent(_parent)
    {
        isClosing = false;
        callback = new YarpJS_Callback<_YarpJS_PortWriteReplier>(this,&_YarpJS_PortWriteReplier::_callback_onReplyReceived,&_YarpJS_PortWriteReplier::_internal_Write);
    }


    ~_YarpJS_PortWriteReplier()
    {
    }

    void write(yarp::os::Bottle _datum)
    {
        datum = _datum;
        callback->callCallback();
    }


    void setCallback(const Nan::FunctionCallbackInfo<v8::Value> &info)
    {
        callback->setCallback(info);
    }

    void interrupt()
    {
        this->callback->interrupt();
    }


};


class YarpJS_RPCPort :  public YarpJS,
                        public yarp::os::Port
{
private:

    // void setWriter(_YarpJS_PortWriteReplier _onWriteReplier) {
    //     _onWriteReplier.setParent(this);
    // }

protected:

    _YarpJS_PortReplier             onReadReplier;
    _YarpJS_PortWriteReplier        *onWriteReplier;


    static void setJSMethods(v8::Local<v8::FunctionTemplate> &tpl)
    {

        Nan::SetPrototypeMethod(tpl,"open",Open);
        Nan::SetPrototypeMethod(tpl,"close",Close);
        Nan::SetPrototypeMethod(tpl,"onRead",SetOnReadCallback);
        Nan::SetPrototypeMethod(tpl,"write",Write);
        Nan::SetPrototypeMethod(tpl,"writeWithReply",WriteWithReply);

        Nan::SetPrototypeMethod(tpl,"reply",Reply);

        Nan::SetPrototypeMethod(tpl,"onReplyFromWrite",SetOnWriteCallback);
        
    }
    
    std::string                             port_name;

public:

    explicit YarpJS_RPCPort()
    {     
        this->setReader(onReadReplier);
        onWriteReplier = new _YarpJS_PortWriteReplier(*this);
    }

    explicit YarpJS_RPCPort(const Nan::FunctionCallbackInfo<v8::Value> &info)
    {     
        this->setReader(onReadReplier);
        onWriteReplier = new _YarpJS_PortWriteReplier(*this);
    }


    ~YarpJS_RPCPort()
    {
        this->interrupt();
        this->close();
    }


    // NAN Stuff
    static NAN_METHOD(Open);
    static NAN_METHOD(Close);
    static NAN_METHOD(SetOnReadCallback);
    static NAN_METHOD(Write);
    static NAN_METHOD(WriteWithReply);
    static NAN_METHOD(Reply);


    static NAN_METHOD(SetOnWriteCallback);
    

    YARPJS_INIT(YarpJS_RPCPort,"RPCPort",YarpJS)


    void _internal_Write(yarp::os::Bottle &a, yarp::os::Bottle &b)
    {
        this->write(a,b);
    }

};






#endif

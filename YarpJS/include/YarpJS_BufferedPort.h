

#ifndef YARPJS_BUFFERED_PORT_JS_H
#define YARPJS_BUFFERED_PORT_JS_H


#include <nan.h>

#include <yarp/os/BufferedPort.h>

#include <stdio.h>
#include <string>


#include "YarpJS_Callback.h"







template<class T>
class YarpJS_BufferedPort : public YarpJS,
                            public yarp::os::BufferedPort<T>
{
protected:

    T        datum;

    YarpJS_Callback<YarpJS_BufferedPort<T> >         *onReadCallback;

    virtual void onRead(T &_datum)
    {
        onReadCallback->callCallback(_datum);
    }

public:

    typedef T datumType;

    virtual void _callback_onRead(std::vector<v8::Local<v8::Value> > &tmp_arguments) = 0;

    virtual void latchDatum(T &datumToLatch)
    {
        datum = datumToLatch;
    }

    explicit YarpJS_BufferedPort()
    {     
        onReadCallback = new YarpJS_Callback<YarpJS_BufferedPort<T> >(this,&YarpJS_BufferedPort<T>::_callback_onRead);

        this->useCallback();
    }

    explicit YarpJS_BufferedPort(const Nan::FunctionCallbackInfo<v8::Value> &info)
    {    
        onReadCallback = new YarpJS_Callback<YarpJS_BufferedPort<T> >(this,&YarpJS_BufferedPort<T>::prepareCallback);

        this->useCallback();
    }


    ~YarpJS_BufferedPort()
    {
        this->interrupt();
        this->close();
    }


};



#endif



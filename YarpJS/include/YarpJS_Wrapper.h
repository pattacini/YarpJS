        

#ifndef YARP_JS_WRAPPER_H
#define YARP_JS_WRAPPER_H

#include <nan.h>
#include <string>
#include "YarpJS.h"


template <class T>
class YarpJS_Wrapper : public YarpJS {
private:

    T                   *yarpObj;
    bool                is_owner; // delete the yarpObj only if it is owner of it

    
    static void setJSMethods(v8::Local<v8::FunctionTemplate> &tpl)
    {}

public:

    explicit YarpJS_Wrapper()
    {
        yarpObj = NULL;
        is_owner = false;
    }

    explicit YarpJS_Wrapper(const Nan::FunctionCallbackInfo<v8::Value> &info):
        YarpJS_Wrapper()
    {}

    ~YarpJS_Wrapper()
    {
        if(is_owner && yarpObj!=NULL)
        {
            delete yarpObj;
        }
    }


    T* getYarpObj()
    {
        return yarpObj;
    }


    void setYarpObj(T *_yarpObj, bool _is_owner=true)
    {
        if (is_owner && yarpObj!=NULL)
            delete yarpObj;

        is_owner = _is_owner;

        yarpObj = _yarpObj;
    }



};




#endif

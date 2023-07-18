

#ifndef YARPJS_CALLBACK_H
#define YARPJS_CALLBACK_H


#include <nan.h>

#include <mutex>
#include "YarpJS.h"


#include <stdio.h>
#include <string>
#include <vector>



template <class T>
class YarpJS_Callback {
private:

    typedef void (T::*PrepareCallback)(std::vector<v8::Local<v8::Value> > &tmp_arguments);
    typedef void (T::*Internal)();

    T                                                       *parent;
    PrepareCallback                                         prepareCallback;

    Internal                                                internal;

    Nan::Callback                                           *callback;
    uv_work_t                                               work_req;

    uv_async_t                                              async;



    static void _internal_async(uv_async_t *handle);

    static void _internal_worker(uv_work_t *req);
    static void _internal_worker_end(uv_work_t *req, int status);


    std::mutex                                 mutex_callback;

    

public:

    void callCallback();
    void callCallback(typename T::datumType &_datum);

    void setCallback(const Nan::FunctionCallbackInfo<v8::Value> &info);
    void setCallback(const v8::Value &info);


    explicit YarpJS_Callback(T *_parent, PrepareCallback _prepareCallback, Internal _internal = NULL)
        :parent(_parent), prepareCallback(_prepareCallback), internal(_internal)

    {    
        this->work_req.data = this;
        this->async.data = this;

        callback = NULL;
    }


    ~YarpJS_Callback()
    {
        this->interrupt();
    }


    void interrupt()
    {
        uv_cancel((uv_req_t*) &this->work_req);
        mutex_callback.unlock();

        if(callback!=NULL)
            delete callback;
    }
};





template <class T>
void YarpJS_Callback<T>::_internal_async(uv_async_t *handle)
{
    YarpJS_Callback<T> *tmp_this = static_cast<YarpJS_Callback<T> *>(handle->data);

    uv_queue_work(uv_default_loop(),&tmp_this->work_req,YarpJS_Callback::_internal_worker,YarpJS_Callback::_internal_worker_end);
}



template <class T>
void YarpJS_Callback<T>::_internal_worker(uv_work_t *req)
{

    YarpJS_Callback<T> *tmp_this = static_cast<YarpJS_Callback<T> *>(req->data);

    if (tmp_this->internal != NULL)
        (tmp_this->parent->*(tmp_this->internal))();

}

template <class T>
void YarpJS_Callback<T>::_internal_worker_end(uv_work_t *req, int status)
{

    if (status == UV_ECANCELED)
      return;


    YarpJS_Callback<T> *tmp_this = static_cast<YarpJS_Callback<T> *>(req->data);

    Nan::HandleScope scope;

    std::vector<v8::Local<v8::Value> > tmp_arguments;
    
    (tmp_this->parent->*(tmp_this->prepareCallback))(tmp_arguments);
    
    tmp_this->callback->Call(tmp_arguments.size(),tmp_arguments.data());

    tmp_this->mutex_callback.unlock();
}


template <class T>
void YarpJS_Callback<T>::callCallback()
{
    mutex_callback.lock();

    if(callback!=NULL)
        uv_async_send( &(this->async) );
    else
        mutex_callback.unlock();
}

template <class T>
void YarpJS_Callback<T>::callCallback(typename T::datumType &_datum)
{
    mutex_callback.lock();

    parent->latchDatum(_datum);

    if(callback!=NULL)
        uv_async_send( &(this->async) );
    else
        mutex_callback.unlock();
}



template <class T>
void YarpJS_Callback<T>::setCallback(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    mutex_callback.lock();

    if(callback != NULL)
      delete callback;
    else
        uv_async_init(uv_default_loop(), &(this->async), YarpJS_Callback::_internal_async);


    callback = new Nan::Callback(info[0].As<v8::Function>());

    mutex_callback.unlock();

}





#endif




#ifndef YARP_JS_IMAGE_H
#define YARP_JS_IMAGE_H


#include <nan.h>
#include <string>

#include "YarpJS_Wrapper.h"
#include <yarp/sig/Image.h>

#include <yarp/cv/Cv.h>

#define PNG     0
#define JPG     1

#define DEFAULT_COMPRESSION JPG


class YarpJS_Image: public YarpJS_Wrapper<yarp::sig::Image>
{
private:

    int                             compression_type;
    cv::Mat                         internalImage;
    std::vector<unsigned char>      internalBuffer;

    bool                            isCompressed;

    static void setJSMethods(v8::Local<v8::FunctionTemplate> &tpl)
    {
        Nan::SetPrototypeMethod(tpl,"copy",Copy);
        Nan::SetPrototypeMethod(tpl,"toBinary",ToBinary);
        Nan::SetPrototypeMethod(tpl,"getCompressionType",GetCompressionType);
        Nan::SetPrototypeMethod(tpl,"getObjType", GetObjType);

        Nan::SetPrototypeMethod(tpl,"getHeight", GetHeight);
        Nan::SetPrototypeMethod(tpl,"getWidth", GetWidth);

    }

    void compress(int compression_quality = 10);

public:


    explicit YarpJS_Image(const Nan::FunctionCallbackInfo<v8::Value> &info)
        :compression_type(DEFAULT_COMPRESSION),isCompressed(false)
    {
        
    }


    explicit YarpJS_Image(yarp::sig::Image &image, int _compression_type=DEFAULT_COMPRESSION)
        :compression_type(_compression_type),isCompressed(false)
    {           
        this->setYarpObj(new yarp::sig::Image(image));
    }

    explicit YarpJS_Image(int _compression_type=DEFAULT_COMPRESSION)
        :compression_type(_compression_type),isCompressed(false)
    {}



    ~YarpJS_Image()
    {}


    static NAN_METHOD(Copy);
    static NAN_METHOD(ToBinary);
    static NAN_METHOD(GetCompressionType);
    static NAN_METHOD(GetObjType);


    static NAN_METHOD(GetHeight);
    static NAN_METHOD(GetWidth);

    // NAN Stuff
    YARPJS_INIT(YarpJS_Image,"Image",YarpJS)
};






#endif





#include <stdio.h>
#include <string>

#include "YarpJS_Image.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


using namespace v8;

Nan::Persistent<v8::FunctionTemplate>  YarpJS_Image::constructor;

//typedef cv::Mat (*toCvMatFunc)(yarp::sig::ImageOf<yarp::sig::PixelRgb>&);
template <typename T>
using toCvMatFunc = cv::Mat (*)(yarp::sig::ImageOf<T>&);

void YarpJS_Image::compress(int compression_quality)
{
  static std::map<int,toCvMatFunc<yarp::sig::PixelRgb>> yarpCode2toCvMat = {
//      {VOCAB_PIXEL_MONO        , &yarp::cv::toCvMat<yarp::sig::PixelMono>},
//      {VOCAB_PIXEL_MONO16      , &yarp::cv::toCvMat<yarp::sig::PixelMono16>},
//      {VOCAB_PIXEL_MONO_SIGNED , &yarp::cv::toCvMat<yarp::sig::PixelMonoSigned>},
//      {VOCAB_PIXEL_MONO_FLOAT  , &yarp::cv::toCvMat<yarp::sig::PixelFloat>},
//      {VOCAB_PIXEL_INT         , &yarp::cv::toCvMat<yarp::sig::PixelInt>},
//      {VOCAB_PIXEL_BGR         , &yarp::cv::toCvMat<yarp::sig::PixelBgr>},
      {VOCAB_PIXEL_RGB         , &yarp::cv::toCvMat<yarp::sig::PixelRgb>},
//      {VOCAB_PIXEL_RGB_SIGNED  , &yarp::cv::toCvMat<yarp::sig::PixelRgbSigned>},
//      {VOCAB_PIXEL_RGB_FLOAT   , &yarp::cv::toCvMat<yarp::sig::PixelRgbFloat>},
//      {VOCAB_PIXEL_RGB_INT     , &yarp::cv::toCvMat<yarp::sig::PixelRgbInt>},
//      {VOCAB_PIXEL_HSV         , &yarp::cv::toCvMat<yarp::sig::PixelHsv>},
//      {VOCAB_PIXEL_HSV_FLOAT   , &yarp::cv::toCvMat<yarp::sig::PixelHsvFloat>},
//      {VOCAB_PIXEL_RGBA        , &yarp::cv::toCvMat<yarp::sig::PixelRgba>},
//      {VOCAB_PIXEL_BGRA        , &yarp::cv::toCvMat<yarp::sig::PixelBgra>}
  };

  if(!this->isCompressed)
  {
    yarp::sig::ImageOf<yarp::sig::PixelRgb>* pixelTypedImage = static_cast<yarp::sig::ImageOf<yarp::sig::PixelRgb>*>(this->getYarpObj());
    internalImage = yarpCode2toCvMat.at(this->getYarpObj()->getPixelCode())(*pixelTypedImage);
    std::vector<int> p;
    std::string encodeString;
    if(compression_type == PNG)
    {
      p.push_back(cv::IMWRITE_PNG_COMPRESSION);
      p.push_back(0);
      encodeString = ".png";
    }
    else
    {
      p.push_back(cv::IMWRITE_JPEG_QUALITY);
      p.push_back(compression_quality);
      encodeString = ".jpg";
    }

    cv::imencode(encodeString,internalImage, internalBuffer, p);

    this->isCompressed = true;
  }
}



NAN_METHOD(YarpJS_Image::Copy) {

  YarpJS_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info.This());

  YarpJS_Image* target = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());
  obj->getYarpObj()->copy(*(target->getYarpObj()));

  obj->isCompressed = false;

}


NAN_METHOD(YarpJS_Image::ToBinary) {

  YarpJS_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info.This());

  int compression_quality = info[0]->IntegerValue(Nan::GetCurrentContext()).FromMaybe(0);

  if(info[0]->IsUndefined())
    obj->compress();
  else
    obj->compress(compression_quality);

  info.GetReturnValue().Set(Nan::CopyBuffer((char *) &obj->internalBuffer
    [0], obj->internalBuffer.size()*sizeof(unsigned char)).ToLocalChecked());


}


NAN_METHOD(YarpJS_Image::GetCompressionType) {

  YarpJS_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info.This());

  std::string compression_type_string = (obj->compression_type == PNG) ? "png":"jpg";

  info.GetReturnValue().Set(Nan::New(compression_type_string.c_str()).ToLocalChecked());

}


NAN_METHOD(YarpJS_Image::GetObjType) {

  info.GetReturnValue().Set(Nan::New("image").ToLocalChecked());

}




NAN_METHOD(YarpJS_Image::GetHeight) {
  YarpJS_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info.This());

  int height = obj->getYarpObj()->height();
  info.GetReturnValue().Set(height);
}


NAN_METHOD(YarpJS_Image::GetWidth) {
  YarpJS_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info.This());

  int width = obj->getYarpObj()->width();
  info.GetReturnValue().Set(width);
}



#include <stdio.h>
#include <string>

#include "YarpJS_Image.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


using namespace v8;

Nan::Persistent<v8::FunctionTemplate>  YarpJS_Image::constructor;



void YarpJS_Image::compress(int compression_quality)
{
  if(!this->isCompressed)
  {
    internalImage = cv::cvarrToMat((IplImage *) this->getYarpObj()->getIplImage());
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

  YarpJS_Image* target = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info[0]->ToObject());
  obj->getYarpObj()->copy(*(target->getYarpObj()));

  obj->isCompressed = false;

}


NAN_METHOD(YarpJS_Image::ToBinary) {

  YarpJS_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info.This());

  int compression_quality = info[0]->IntegerValue();

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

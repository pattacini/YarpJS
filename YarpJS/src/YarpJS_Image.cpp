

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
  static const std::map<int,int> yarpCode2colorConvCode {
      {VOCAB_PIXEL_MONO,cv::COLOR_GRAY2BGR},
      {VOCAB_PIXEL_RGB,cv::COLOR_RGB2BGR},
      {VOCAB_PIXEL_RGBA,cv::COLOR_RGBA2BGRA},
      {VOCAB_PIXEL_HSV,cv::COLOR_HSV2BGR_FULL},
      {VOCAB_PIXEL_ENCODING_BAYER_GRBG8,cv::COLOR_BayerGR2BGR},
      {VOCAB_PIXEL_ENCODING_BAYER_BGGR8,cv::COLOR_BayerBG2BGR},
      {VOCAB_PIXEL_ENCODING_BAYER_GBRG8,cv::COLOR_BayerGB2BGR},
      {VOCAB_PIXEL_ENCODING_BAYER_RGGB8,cv::COLOR_BayerRG2BGR},
      {VOCAB_PIXEL_YUV_420,cv::COLOR_YUV2BGR_I420},
      {VOCAB_PIXEL_YUV_422,cv::COLOR_YUV2BGR_Y422}
  };

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

    int imgPixelCode = this->getYarpObj()->getPixelCode();
    if (imgPixelCode != VOCAB_PIXEL_BGR
        && imgPixelCode != VOCAB_PIXEL_BGRA) {
        auto conversionCodePtr = yarpCode2colorConvCode.find(imgPixelCode);
        if (conversionCodePtr != yarpCode2colorConvCode.end()) {
            cv::cvtColor(internalImage, internalImage, conversionCodePtr->second, 0);
        } else {
            throw std::out_of_range("YarpJS_Image::compress(int compression_quality): pixel code not supported");
        }
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

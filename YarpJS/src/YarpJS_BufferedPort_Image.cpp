



#include "YarpJS_BufferedPort_Image.h"



using namespace v8;





Nan::Persistent<v8::FunctionTemplate>  YarpJS_BufferedPort_Image::constructor;




void YarpJS_BufferedPort_Image::_callback_onRead(std::vector<v8::Local<v8::Value> > &tmp_arguments)
{
    // create a new YarpJS_Image
    v8::Local<v8::Value> argv[1] = {Nan::New(Nan::Null)};
    v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(YarpJS_Image::constructor)).ToLocalChecked();
    v8::Local<v8::Object> tmpImgJS = cons->NewInstance(Nan::GetCurrentContext(), 1, argv).ToLocalChecked();

    YarpJS_Image *tmpImg = Nan::ObjectWrap::Unwrap<YarpJS_Image>(tmpImgJS);
    
    tmpImg->setYarpObj(new yarp::sig::Image(this->datum));
    tmp_arguments.push_back(tmpImgJS);
}




NAN_METHOD(YarpJS_BufferedPort_Image::Prepare) {

  YarpJS_BufferedPort_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Image>(info.This());


  // create a new YarpJS_Image
  v8::Local<v8::Value> argv[1] = {Nan::New(Nan::Null)};
  v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(YarpJS_Image::constructor)).ToLocalChecked();
  v8::Local<v8::Object> bPreparedJS = cons->NewInstance(Nan::GetCurrentContext(), 1, argv).ToLocalChecked();

  YarpJS_Image *bPrepared = Nan::ObjectWrap::Unwrap<YarpJS_Image>(bPreparedJS);
  
  // do not give ownership of the bottle
  bPrepared->setYarpObj(&obj->prepare(),false);


  info.GetReturnValue().Set(bPreparedJS);
}



NAN_METHOD(YarpJS_BufferedPort_Image::Open) {

  YarpJS_BufferedPort_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Image>(info.This());
  
  // std::string _port_name = info[0]->IsUndefined() ? "" : Nan::To<std::string>(info[0]).FromJust();
  v8::String::Utf8Value _port_name(v8::Isolate::GetCurrent(),info[0]->ToString(Nan::GetCurrentContext()).ToLocalChecked());


  bool isOpen = obj->open(*_port_name);

  info.GetReturnValue().Set(isOpen);
}


NAN_METHOD(YarpJS_BufferedPort_Image::Close) {

  YarpJS_BufferedPort_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Image>(info.This());
  obj->close();
}




NAN_METHOD(YarpJS_BufferedPort_Image::SetOnReadCallback) {

  YarpJS_BufferedPort_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Image>(info.This());
  obj->onReadCallback->setCallback(info);

}


// NAN_METHOD(YarpJS_BufferedPort_Image::SetOnRPCCallback) {

//   YarpJS_BufferedPort_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Image>(info.This());
//   obj->RPCReplier.setCallback(info);

// }




NAN_METHOD(YarpJS_BufferedPort_Image::Write) {

  YarpJS_BufferedPort_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Image>(info.This());

  obj->write();
}






// NAN_METHOD(YarpJS_BufferedPort_Image::Reply) {


//   YarpJS_BufferedPort_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Image>(info.This());

//   // get the reply bottle
//   YarpJS_Image* target = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info[0]->ToObject());

//   obj->RPCReplier.reply(*(target->getYarpObj()));

// }

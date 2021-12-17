



#include "YarpJS_BufferedPort_Sound.h"



using namespace v8;





Nan::Persistent<v8::FunctionTemplate>  YarpJS_BufferedPort_Sound::constructor;




void YarpJS_BufferedPort_Sound::_callback_onRead(std::vector<v8::Local<v8::Value> > &tmp_arguments)
{
    // create a new YarpJS_Sound
    v8::Local<v8::Value> argv[1] = {Nan::New(Nan::Null)};
    v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(YarpJS_Sound::constructor)).ToLocalChecked();
    v8::Local<v8::Object> tmpImgJS = cons->NewInstance(Nan::GetCurrentContext(), 1, argv).ToLocalChecked();

    YarpJS_Sound *tmpImg = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(tmpImgJS);
    
    tmpImg->setYarpObj(new yarp::sig::Sound(this->datum));
    tmp_arguments.push_back(tmpImgJS);
}






NAN_METHOD(YarpJS_BufferedPort_Sound::Prepare) {

  YarpJS_BufferedPort_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Sound>(info.This());


  // create a new YarpJS_Sound
  v8::Local<v8::Value> argv[1] = {Nan::New(Nan::Null)};
  v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(YarpJS_Sound::constructor)).ToLocalChecked();
  v8::Local<v8::Object> bPreparedJS = cons->NewInstance(Nan::GetCurrentContext(), 1, argv).ToLocalChecked();

  YarpJS_Sound *bPrepared = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(bPreparedJS);
  
  // do not give ownership of the bottle
  bPrepared->setYarpObj(&obj->prepare(),false);

  info.GetReturnValue().Set(bPreparedJS);
  
}




NAN_METHOD(YarpJS_BufferedPort_Sound::Open) {

  YarpJS_BufferedPort_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Sound>(info.This());
  
  // std::string _port_name = info[0]->IsUndefined() ? "" : Nan::To<std::string>(info[0]).FromJust();
  v8::String::Utf8Value _port_name(v8::Isolate::GetCurrent(),info[0]->ToString(Nan::GetCurrentContext()).ToLocalChecked());


  bool isOpen = obj->open(*_port_name);

  info.GetReturnValue().Set(isOpen);
}


NAN_METHOD(YarpJS_BufferedPort_Sound::Close) {

  YarpJS_BufferedPort_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Sound>(info.This());
  obj->close();
}




NAN_METHOD(YarpJS_BufferedPort_Sound::SetOnReadCallback) {

  YarpJS_BufferedPort_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Sound>(info.This());
  obj->onReadCallback->setCallback(info);

}


// NAN_METHOD(YarpJS_BufferedPort_Sound::SetOnRPCCallback) {

//   YarpJS_BufferedPort_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Sound>(info.This());
//   obj->RPCReplier.setCallback(info);

// }




NAN_METHOD(YarpJS_BufferedPort_Sound::Write) {

  YarpJS_BufferedPort_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Sound>(info.This());

  obj->write();
}


NAN_METHOD(YarpJS_BufferedPort_Sound::SetStrict) {

  YarpJS_BufferedPort_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Sound>(info.This());

  //HERE
  obj->setStrict();
}

//not working
NAN_METHOD(YarpJS_BufferedPort_Sound::WriteStrict) { 

  YarpJS_BufferedPort_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Sound>(info.This());

//HERE
  obj->writeStrict();
}

NAN_METHOD(YarpJS_BufferedPort_Sound::WaitForWrite) {

  YarpJS_BufferedPort_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Sound>(info.This());

  obj->waitForWrite();
}


// NAN_METHOD(YarpJS_BufferedPort_Sound::Reply) {


//   YarpJS_BufferedPort_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Sound>(info.This());

//   // get the reply bottle
//   YarpJS_Sound* target = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info[0]->ToObject());

//   obj->RPCReplier.reply(*(target->getYarpObj()));

// }

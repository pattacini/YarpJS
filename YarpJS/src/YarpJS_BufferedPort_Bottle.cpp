



#include "YarpJS_BufferedPort_Bottle.h"
#include "YarpJS_Bottle.h"



using namespace v8;








Nan::Persistent<v8::FunctionTemplate>  YarpJS_BufferedPort_Bottle::constructor;




void YarpJS_BufferedPort_Bottle::_callback_onRead(std::vector<v8::Local<v8::Value> > &tmp_arguments)
{
    const int argc = 1;   
    v8::Local<v8::Value> argv[argc] = {Nan::New<String>(this->datum.toString()).ToLocalChecked()};
    v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(YarpJS_Bottle::constructor)).ToLocalChecked();
    tmp_arguments.push_back(cons->NewInstance(Nan::GetCurrentContext(), argc, argv).ToLocalChecked());
}




NAN_METHOD(YarpJS_BufferedPort_Bottle::Open) {

  YarpJS_BufferedPort_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Bottle>(info.This());
  
  // std::string _port_name = info[0]->IsUndefined() ? "" : Nan::To<std::string>(info[0]).FromJust();
  v8::String::Utf8Value _port_name(info[0]->ToString());


  bool isOpen = obj->open(*_port_name);

  info.GetReturnValue().Set(isOpen);
}


NAN_METHOD(YarpJS_BufferedPort_Bottle::Close) {

  YarpJS_BufferedPort_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Bottle>(info.This());
  obj->close();
}




NAN_METHOD(YarpJS_BufferedPort_Bottle::SetOnReadCallback) {

  YarpJS_BufferedPort_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Bottle>(info.This());
  obj->onReadCallback->setCallback(info);

}


NAN_METHOD(YarpJS_BufferedPort_Bottle::SetOnRPCCallback) {

  YarpJS_BufferedPort_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Bottle>(info.This());
  obj->RPCReplier.setCallback(info);

}




NAN_METHOD(YarpJS_BufferedPort_Bottle::Write) {

  YarpJS_BufferedPort_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Bottle>(info.This());

  obj->write();
}





NAN_METHOD(YarpJS_BufferedPort_Bottle::Prepare) {

  YarpJS_BufferedPort_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Bottle>(info.This());


  // create a new YarpJS_Bottle
  v8::Local<v8::Value> argv[1] = {Nan::New(Nan::Null)};
  v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(YarpJS_Bottle::constructor)).ToLocalChecked();
  v8::Local<v8::Object> bPreparedJS = cons->NewInstance(Nan::GetCurrentContext(), 1, argv).ToLocalChecked();

  YarpJS_Bottle *bPrepared = Nan::ObjectWrap::Unwrap<YarpJS_Bottle>(bPreparedJS);
  
  // do not give ownership of the bottle
  bPrepared->setYarpObj(&obj->prepare(),false);

  info.GetReturnValue().Set(bPreparedJS);
}




NAN_METHOD(YarpJS_BufferedPort_Bottle::Reply) {


  YarpJS_BufferedPort_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Bottle>(info.This());

  // get the reply bottle
  YarpJS_Bottle* target = Nan::ObjectWrap::Unwrap<YarpJS_Bottle>(info[0]->ToObject(Nan::GetCurrentContext()));

  obj->RPCReplier.reply(*(target->getYarpObj()));

}

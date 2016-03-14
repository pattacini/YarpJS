



#include "YarpJS_BufferedPort_Bottle.h"
#include "YarpJS_Bottle.h"



using namespace v8;





Nan::Persistent<v8::FunctionTemplate>  YarpJS_BufferedPort_Bottle::constructor;




void YarpJS_BufferedPort_Bottle::prepareCallback(std::vector<v8::Local<v8::Value> > &tmp_arguments)
{
    const int argc = 1;   
    v8::Local<v8::Value> argv[argc] = {Nan::New<String>(this->datum.toString()).ToLocalChecked()};
    v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(YarpJS_Bottle::constructor)).ToLocalChecked();
    tmp_arguments.push_back(cons->NewInstance(argc,argv));
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
  obj->_setCallback(info);

}




NAN_METHOD(YarpJS_BufferedPort_Bottle::Write) {

  YarpJS_BufferedPort_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Bottle>(info.This());

  v8::String::Utf8Value _port_name(info[0]->ToString());
  
  obj->write();
}





NAN_METHOD(YarpJS_BufferedPort_Bottle::Prepare) {

  YarpJS_BufferedPort_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_BufferedPort_Bottle>(info.This());


  // create a new YarpJS_Bottle
  v8::Local<v8::Value> argv[1] = {Nan::New(Nan::Null)};
  v8::Local<v8::Function> cons = Nan::GetFunction(Nan::New(YarpJS_Bottle::constructor)).ToLocalChecked();
  v8::Local<v8::Object> bPreparedJS = cons->NewInstance(1,argv);

  YarpJS_Bottle *bPrepared = Nan::ObjectWrap::Unwrap<YarpJS_Bottle>(bPreparedJS);
  
  bPrepared->setYarpObj(&obj->prepare());

  info.GetReturnValue().Set(bPreparedJS);
}





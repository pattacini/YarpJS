



#include "YarpJS_RPCPort.h"

#include "YarpJS_Bottle.h"



using namespace v8;


Nan::Persistent<v8::FunctionTemplate>  YarpJS_RPCPort::constructor;




NAN_METHOD(YarpJS_RPCPort::Open) {

  YarpJS_RPCPort* obj = Nan::ObjectWrap::Unwrap<YarpJS_RPCPort>(info.This());
  
  // std::string _port_name = info[0]->IsUndefined() ? "" : Nan::To<std::string>(info[0]).FromJust();
  v8::String::Utf8Value _port_name(info[0]->ToString());


  bool isOpen = obj->open(*_port_name);

  info.GetReturnValue().Set(isOpen);
  
}


NAN_METHOD(YarpJS_RPCPort::Close) {

  YarpJS_RPCPort* obj = Nan::ObjectWrap::Unwrap<YarpJS_RPCPort>(info.This());

  obj->onReadReplier.interrupt();
  obj->interrupt();
  obj->onWriteReplier->interrupt();
  obj->close();

}




NAN_METHOD(YarpJS_RPCPort::SetOnReadCallback) {

  YarpJS_RPCPort* obj = Nan::ObjectWrap::Unwrap<YarpJS_RPCPort>(info.This());
  obj->onReadReplier.setCallback(info);

}




NAN_METHOD(YarpJS_RPCPort::Write) {

  YarpJS_RPCPort* obj = Nan::ObjectWrap::Unwrap<YarpJS_RPCPort>(info.This());

  // get the reply bottle
  // YarpJS_Bottle* target = Nan::ObjectWrap::Unwrap<YarpJS_Bottle>(info[0]->ToObject());
  YarpJS_Wrapper<yarp::os::Portable>* target = Nan::ObjectWrap::Unwrap< YarpJS_Wrapper<yarp::os::Portable> >
  (info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());

  obj->write( *(target->getYarpObj()) );

}




NAN_METHOD(YarpJS_RPCPort::SetOnWriteCallback) {

  YarpJS_RPCPort* obj = Nan::ObjectWrap::Unwrap<YarpJS_RPCPort>(info.This());
  obj->onWriteReplier->setCallback(info);

}


void _YarpJS_PortWriteReplier::_internal_Write()
{
    parent._internal_Write(datum,reply_datum);
}

NAN_METHOD(YarpJS_RPCPort::WriteWithReply) {

  YarpJS_RPCPort* obj = Nan::ObjectWrap::Unwrap<YarpJS_RPCPort>(info.This());

  // get the reply bottle
  // YarpJS_Bottle* target = Nan::ObjectWrap::Unwrap<YarpJS_Bottle>(info[0]->ToObject());
  YarpJS_Wrapper<yarp::os::Bottle>* target = Nan::ObjectWrap::Unwrap< YarpJS_Wrapper<yarp::os::Bottle> >
  (info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());

  // obj->write( *(target->getYarpObj()) , obj->onReadReplier2 );
  yarp::os::Bottle b;
  obj->onWriteReplier->write( *(target->getYarpObj()) );

  // fprintf(stdout, "Ricevuto: %s\n",b.toString().c_str() );
}







NAN_METHOD(YarpJS_RPCPort::Reply) {

  YarpJS_RPCPort* obj = Nan::ObjectWrap::Unwrap<YarpJS_RPCPort>(info.This());

  // get the reply bottle
  YarpJS_Bottle* target = Nan::ObjectWrap::Unwrap<YarpJS_Bottle>(info[0]->ToObject(Nan::GetCurrentContext()));

  obj->onReadReplier.reply( *(target->getYarpObj()) );

}

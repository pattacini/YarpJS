

#include <stdio.h>
#include <string>

#include "YarpJS_Network.h"


using namespace v8;


// NAN declaration of the YarpJS_Network constructor. 
// Necessary for NAN usage. 

Nan::Persistent<v8::FunctionTemplate>  YarpJS_Network::constructor;



// YarpJS_Network::Init
// Initializing the network object

NAN_METHOD(YarpJS_Network::Network_Init) {

  YarpJS_Network* obj = Nan::ObjectWrap::Unwrap<YarpJS_Network>(info.This());
  obj->getYarpObj()->init();

}


// YarpJS_Network::Fini
// Closing the network object

NAN_METHOD(YarpJS_Network::Fini) {

  YarpJS_Network* obj = Nan::ObjectWrap::Unwrap<YarpJS_Network>(info.This());
  obj->getYarpObj()->fini();

}



// YarpJS_Network::Connect
// Connects two ports. Currently only TCP is supported (it is trivial to extend to other carriers).

NAN_METHOD(YarpJS_Network::Connect) {

  YarpJS_Network* obj = Nan::ObjectWrap::Unwrap<YarpJS_Network>(info.This());

  v8::String::Utf8Value param1(v8::Isolate::GetCurrent(),info[0]->ToString(Nan::GetCurrentContext()).ToLocalChecked());
  v8::String::Utf8Value param2(v8::Isolate::GetCurrent(),info[1]->ToString(Nan::GetCurrentContext()).ToLocalChecked());

  // convert it to string
  std::string _src = std::string(*param1);    
  std::string _dst = std::string(*param2);    

  obj->getYarpObj()->connect(_src,_dst);

}


// YarpJS_Network::Disconnect
// Disconnect two ports


NAN_METHOD(YarpJS_Network::Disconnect) {


  YarpJS_Network* obj = Nan::ObjectWrap::Unwrap<YarpJS_Network>(info.This());

  v8::String::Utf8Value param1(v8::Isolate::GetCurrent(),info[0]->ToString(Nan::GetCurrentContext()).ToLocalChecked());
  v8::String::Utf8Value param2(v8::Isolate::GetCurrent(),info[1]->ToString(Nan::GetCurrentContext()).ToLocalChecked());

  // convert it to string
  std::string _src = std::string(*param1);    
  std::string _dst = std::string(*param2);    

  obj->getYarpObj()->disconnect(_src,_dst);

}


// YarpJS_Network::List
// Returns a list of all ports opened on the yarp network

NAN_METHOD(YarpJS_Network::List) {


  YarpJS_Network* obj = Nan::ObjectWrap::Unwrap<YarpJS_Network>(info.This());

  yarp::os::Bottle query, response;
  query.addString("bot");
  query.addString("list");
  
  obj->getYarpObj()->write(obj->getYarpObj()->getNameServerName().c_str(),query,response);

  v8::Local<v8::Array> bArr = Nan::New<v8::Array>(0);

  int idx_arr = 0;

  if (!(response.get(0).asString()=="ports"))
    fprintf(stdout,"Unknown response from name server\n");
  else
  {  
    for (int i=1; i<response.size(); i++) {
      yarp::os::Bottle *port_info = response.get(i).asList();
      if (port_info!=NULL && port_info->check("name"))
        Nan::Set(bArr, idx_arr++, Nan::New(port_info->find("name").asString().c_str()).ToLocalChecked() );
    } 
  }

  info.GetReturnValue().Set(bArr);
}

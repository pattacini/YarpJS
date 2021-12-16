

#include <stdio.h>
#include <string>

#include "YarpJS_Bottle.h"





using namespace v8;

Nan::Persistent<v8::FunctionTemplate>  YarpJS_Bottle::constructor;


void YarpJS_Bottle::toArray(const yarp::os::Bottle *bObj, v8::Local<v8::Array> &bArr)
{

  const int bSize = bObj->size();

  for(int i=0; i<bSize; i++)
      if(bObj->get(i).isList())
      {
        // create the sub-array
        v8::Local<v8::Array> sub_bArr = Nan::New<v8::Array>(bObj->get(i).asList()->size());
        YarpJS_Bottle::toArray(bObj->get(i).asList(), sub_bArr);
        Nan::Set(bArr, i, sub_bArr);
      }
      else if(bObj->get(i).isString())
        Nan::Set(bArr, i, Nan::New(bObj->get(i).toString().c_str()).ToLocalChecked() );
      else if(bObj->get(i).isDouble() || bObj->get(i).isInt())
        Nan::Set(bArr, i, Nan::New(bObj->get(i).asDouble()) );

}



NAN_METHOD(YarpJS_Bottle::Copy) {

  YarpJS_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_Bottle>(info.This());
  
  YarpJS_Bottle* target = Nan::ObjectWrap::Unwrap<YarpJS_Bottle>(info[0]->ToObject(Nan::GetCurrentContext()));
  obj->getYarpObj()->copy(*(target->getYarpObj()));

}

NAN_METHOD(YarpJS_Bottle::ToString) {

  YarpJS_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_Bottle>(info.This());

  info.GetReturnValue().Set(Nan::New(obj->getYarpObj()->toString()).ToLocalChecked());



}

NAN_METHOD(YarpJS_Bottle::ToArray) {

  YarpJS_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_Bottle>(info.This());

  // create the array
  v8::Local<v8::Array> bArr = Nan::New<v8::Array>(obj->getYarpObj()->size());

  // fill the array
  YarpJS_Bottle::toArray(obj->getYarpObj(),bArr);

  info.GetReturnValue().Set(bArr);


}




NAN_METHOD(YarpJS_Bottle::FromString) {

  YarpJS_Bottle* obj = Nan::ObjectWrap::Unwrap<YarpJS_Bottle>(info.This());

  obj->fromString(info);

  info.GetReturnValue().Set(info.This());
}



NAN_METHOD(YarpJS_Bottle::GetObjType) {

  info.GetReturnValue().Set(Nan::New("bottle").ToLocalChecked());
}

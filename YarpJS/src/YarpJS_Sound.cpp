

#include <stdio.h>
#include <string>

#include "YarpJS_Sound.h"

#include <yarp/os/NetInt16.h>

#define SAMPLE_RATE 16000
//#define SAMPLE_RATE 88200

// #define NUM_SECONDS 0.1
#define BIT_DEPTH 16
// #define NUM_SAMPLES ((int)(SAMPLE_RATE*NUM_SECONDS))
#define NUM_SAMPLES 16000
//#define NUM_SAMPLES 88200

unsigned char pulsecode[ NUM_SAMPLES ];


using namespace v8;

Nan::Persistent<v8::FunctionTemplate>  YarpJS_Sound::constructor;




NAN_METHOD(YarpJS_Sound::Copy) {

  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());

  YarpJS_Sound* target = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info[0]->ToObject(Nan::GetCurrentContext()));
  *obj->getYarpObj() = (*(target->getYarpObj()));

}


NAN_METHOD(YarpJS_Sound::ToBinary) {

  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());

  yarp::os::NetInt16 data[NUM_SAMPLES];
  for(int i=0; i<NUM_SAMPLES; i++)
    data[i] = obj->getYarpObj()->get(i);


  info.GetReturnValue().Set(Nan::CopyBuffer((char *) data, NUM_SAMPLES*sizeof(yarp::os::NetInt16)).ToLocalChecked());

}


NAN_METHOD(YarpJS_Sound::FromBinary) {

  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());

  Local<Object> bufferObj = info[0]->ToObject(Nan::GetCurrentContext());

  yarp::os::NetInt16 *data = (yarp::os::NetInt16*) node::Buffer::Data(bufferObj);


  obj->getYarpObj()->resize(NUM_SAMPLES,1);

  obj->getYarpObj()->setFrequency(SAMPLE_RATE);

  for(int i=0; i<NUM_SAMPLES; i++)
    obj->getYarpObj()->set(data[i],i);

}


NAN_METHOD(YarpJS_Sound::GetObjType) {

  info.GetReturnValue().Set(Nan::New("sound").ToLocalChecked());
}

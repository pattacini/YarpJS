

#include <stdio.h>
#include <string>

#include "YarpJS_Sound.h"

#include <yarp/os/NetInt16.h>


using namespace v8;

Nan::Persistent<v8::FunctionTemplate>  YarpJS_Sound::constructor;



NAN_METHOD(YarpJS_Sound::Copy) {

  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());

  YarpJS_Sound* target = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());
  *obj->getYarpObj() = (*(target->getYarpObj()));

}


NAN_METHOD(YarpJS_Sound::ToBinary) {

  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());

  int num_samples = obj->getYarpObj()->getSamples();
  yarp::os::NetInt16 data[num_samples];
  for(int i=0; i<num_samples; i++)
    data[i] = obj->getYarpObj()->get(i);


  info.GetReturnValue().Set(Nan::CopyBuffer((char *) data, num_samples*sizeof(yarp::os::NetInt16)).ToLocalChecked());

}


NAN_METHOD(YarpJS_Sound::FromBinary) {
  if (info.Length() != 4) {
    Nan::ThrowTypeError("Expected 4 arguments: buffer, samples, frequency, channels");
    return;
  }

  if (!info[0]->IsObject() || !info[1]->IsNumber() || !info[2]->IsNumber() || !info[3]->IsNumber()) {
    Nan::ThrowTypeError("Expected arguments: buffer (Object), samples (Number), frequency (Number), channels (Number)");
    return;
  }

  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());

  Local<Object> bufferObj = info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked();
  if (!node::Buffer::HasInstance(bufferObj)) {
    Nan::ThrowTypeError("Expected a Buffer as the first argument");
    return;
  }

  int samples = info[1]->IntegerValue(Nan::GetCurrentContext()).FromJust();
  int frequency = info[2]->IntegerValue(Nan::GetCurrentContext()).FromJust();
  int channels = info[3]->IntegerValue(Nan::GetCurrentContext()).FromJust();

  yarp::os::NetInt16 *data = reinterpret_cast<yarp::os::NetInt16*>(node::Buffer::Data(bufferObj));

  obj->getYarpObj()->resize(samples, channels);
  obj->getYarpObj()->setFrequency(frequency);

  for (int i = 0; i < samples; i++) {
    obj->getYarpObj()->set(data[i], i);
  }
}


// NAN_METHOD(YarpJS_Sound::FromBinary) {
//   YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());

//   Local<Object> bufferObj = info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked();
//   int samples = info[1]->IntegerValue(Nan::GetCurrentContext()).FromJust();
//   int frequency = info[2]->IntegerValue(Nan::GetCurrentContext()).FromJust();
//   int channels = info[3]->IntegerValue(Nan::GetCurrentContext()).FromJust();

//   yarp::os::NetInt16 *data = (yarp::os::NetInt16*) node::Buffer::Data(bufferObj);


//   obj->getYarpObj()->resize(samples,channels);

//   obj->getYarpObj()->setFrequency(frequency);

//   for(int i=0; i<samples; i++)
//     obj->getYarpObj()->set(data[i],i);

// }


NAN_METHOD(YarpJS_Sound::GetObjType) {

  info.GetReturnValue().Set(Nan::New("sound").ToLocalChecked());
}


NAN_METHOD(YarpJS_Sound::GetFrequency) {
  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());

  int frequency = obj->getYarpObj()->getFrequency();
  info.GetReturnValue().Set(frequency);
}


NAN_METHOD(YarpJS_Sound::GetSamples) {
  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());

  int num_sampels = obj->getYarpObj()->getSamples();
  info.GetReturnValue().Set(num_sampels);
}


NAN_METHOD(YarpJS_Sound::GetBytesPerSample) {
  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());

  int bytes_per_sample = obj->getYarpObj()->getBytesPerSample();
  info.GetReturnValue().Set(bytes_per_sample);
}

NAN_METHOD(YarpJS_Sound::GetChannels) {
  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());

  int num_channels = obj->getYarpObj()->getChannels();
  info.GetReturnValue().Set(num_channels);
}


NAN_METHOD(YarpJS_Sound::SetFrequency) {
  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());
  int frequency;
  
  if (!info[0]->IsNumber() && !info[0]->IsInt32())
  {
    Nan::ThrowTypeError("Expected a number.");
    return;
  }

  frequency = info[0]->IntegerValue(Nan::GetCurrentContext()).FromJust();
  obj->getYarpObj()->setFrequency(frequency);
}


NAN_METHOD(YarpJS_Sound::Resize) {
  YarpJS_Sound* obj = Nan::ObjectWrap::Unwrap<YarpJS_Sound>(info.This());
  
  if (!info[0]->IsInt32()) {
    Nan::ThrowTypeError("Expected a number.");
    return;
  }
  
  int num_samples = Nan::To<int32_t>(info[0]).FromJust();
  int num_channels = info.Length() > 1 && info[1]->IsInt32() ? Nan::To<int32_t>(info[1]).FromJust() : 1;

  obj->getYarpObj()->resize(num_samples, num_channels);
}

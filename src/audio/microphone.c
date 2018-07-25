#include "audio/microphone.h"
#include "audio/audio.h"

Microphone* lovrMicrophoneCreate(const char* name, int samples, int sampleRate, int bitDepth, int channelCount) {
  Microphone* microphone = lovrAlloc(Microphone, lovrMicrophoneDestroy);
  if (!microphone) return NULL;

  ALCdevice* device = alcCaptureOpenDevice(name, sampleRate, lovrAudioConvertFormat(bitDepth, channelCount), samples);

  if (!device) {
    free(microphone);
    return NULL;
  }

  microphone->device = device;
  microphone->name = name ? name : alcGetString(device, ALC_CAPTURE_DEVICE_SPECIFIER);
  microphone->sampleRate = sampleRate;
  microphone->bitDepth = bitDepth;
  microphone->channelCount = channelCount;

  return microphone;
}

void lovrMicrophoneDestroy(void* ref) {
  Microphone* microphone = ref;
  alcCaptureCloseDevice(microphone->device);
  free(microphone);
}

int lovrMicrophoneGetBitDepth(Microphone* microphone) {
  return microphone->bitDepth;
}

int lovrMicrophoneGetChannelCount(Microphone* microphone) {
  return microphone->channelCount;
}

SoundData* lovrMicrophoneGetData(Microphone* microphone) {
  if (!microphone->isRecording) {
    return NULL;
  }

  int samples = lovrMicrophoneGetSampleCount(microphone);
  if (samples == 0) {
    return NULL;
  }

  SoundData* soundData = lovrSoundDataCreate(samples, microphone->sampleRate, microphone->bitDepth, microphone->channelCount);
  alcCaptureSamples(microphone->device, soundData->blob.data, samples);
  return soundData;
}

const char* lovrMicrophoneGetName(Microphone* microphone) {
  return microphone->name;
}

int lovrMicrophoneGetSampleCount(Microphone* microphone) {
  if (!microphone->isRecording) {
    return 0;
  }

  ALCint samples;
  alcGetIntegerv(microphone->device, ALC_CAPTURE_SAMPLES, sizeof(ALCint), &samples);
  return (int) samples;
}

int lovrMicrophoneGetSampleRate(Microphone* microphone) {
  return microphone->sampleRate;
}

bool lovrMicrophoneIsRecording(Microphone* microphone) {
  return microphone->isRecording;
}

void lovrMicrophoneStartRecording(Microphone* microphone) {
  if (microphone->isRecording) {
    return;
  }

  alcCaptureStart(microphone->device);
  microphone->isRecording = true;
}

void lovrMicrophoneStopRecording(Microphone* microphone) {
  if (!microphone->isRecording) {
    return;
  }

  microphone->isRecording = false;
}

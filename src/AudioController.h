// ----------------------------------------------------------------------------
// AudioController.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H
#include <ArduinoJson.h>
#include <JsonStream.h>
#include <Array.h>
#include <Vector.h>
#include <ConstantVariable.h>
#include <Functor.h>

#include <EventController.h>

#include <AudioApparatus.h>

#include <ModularServer.h>
#include <ModularDeviceBase.h>

#include "AudioController/Constants.h"
#include "AudioController/SDInterface.h"


class AudioController : public ModularDeviceBase
{
public:
  virtual void setup();

  void playTone(size_t frequency,
    const ConstantString * const speaker_ptr);
  void playNoise(const ConstantString * const speaker_ptr);
  void playFilteredNoise(size_t frequency,
    double bandwidth,
    const ConstantString * const speaker_ptr);

  int addTonePwm(size_t frequency,
    const ConstantString * const speaker_ptr,
    long delay,
    long period,
    long on_duration,
    long count);
  int startTonePwm(size_t frequency,
    const ConstantString * const speaker_ptr,
    long delay,
    long period,
    long on_duration);
  int addNoisePwm(const ConstantString * const speaker_ptr,
    long delay,
    long period,
    long on_duration,
    long count);
  int startNoisePwm(const ConstantString * const speaker_ptr,
    long delay,
    long period,
    long on_duration);
  int addFilteredNoisePwm(size_t frequency,
    double bandwidth,
    const ConstantString * const speaker_ptr,
    long delay,
    long period,
    long on_duration,
    long count);
  int startFilteredNoisePwm(size_t frequency,
    double bandwidth,
    const ConstantString * const speaker_ptr,
    long delay,
    long period,
    long on_duration);

private:
  modular_server::Pin pins_[audio_controller::constants::PIN_COUNT_MAX];

  modular_server::Property properties_[audio_controller::constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[audio_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[audio_controller::constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[audio_controller::constants::CALLBACK_COUNT_MAX];

  EventController<audio_controller::constants::EVENT_COUNT_MAX> event_controller_;
  AudioApparatus<audio_controller::constants::EVENT_COUNT_MAX> audio_apparatus_;

  audio_apparatus::constants::speaker_t speakerPtrToSpeaker(const ConstantString * const speaker_ptr);
  void addDirectoryToResponse(File dir,
    const char * pwd);
  void setVolume(long volume);
  void setPlaying(bool playing);

  // Handlers
  void setVolumeHandler();
  void getAudioMemoryUsageHandler();
  void getAudioMemoryUsageMaxHandler();
  void resetAudioMemoryUsageMaxHandler();
  void getAudioProcessorUsageHandler();
  void getAudioProcessorUsageMaxHandler();
  void resetAudioProcessorUsageMaxHandler();
  void getSDCardInfoHandler();
  void getAudioPathsHandler();
  void playPathHandler();
  void playToneHandler();
  void playToneAtHandler();
  void playNoiseHandler();
  void playNoiseAtHandler();
  void playFilteredNoiseHandler();
  void playFilteredNoiseAtHandler();
  void stopHandler();
  void isPlayingHandler();
  void getLastAudioPathPlayedHandler();
  void getPositionHandler();
  void getLengthHandler();
  void getPercentCompleteHandler();
  void addTonePwmHandler();
  void addTonePwmAtHandler();
  void startTonePwmHandler();
  void startTonePwmAtHandler();
  void addNoisePwmHandler();
  void addNoisePwmAtHandler();
  void startNoisePwmHandler();
  void startNoisePwmAtHandler();
  void addFilteredNoisePwmHandler();
  void addFilteredNoisePwmAtHandler();
  void startFilteredNoisePwmHandler();
  void startFilteredNoisePwmAtHandler();
  void stopPwmHandler();
  void stopAllPwmHandler();
  void isPulsingHandler();

};

#endif

// ----------------------------------------------------------------------------
// AudioController.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H
#include "Flash.h"
#include <EEPROM.h>
#include "Streaming.h"
#include "ArduinoJson.h"
#include "JsonSanitizer.h"
#include "JsonStream.h"
#include "Array.h"
#include "Vector.h"
#include "MemoryFree.h"
#include "ConstantVariable.h"
#include "SavedVariable.h"
#include "Functor.h"

#include "IndexedContainer.h"
#include "FunctorCallbacks.h"
#include "EventController.h"
#include "Wire.h"
#include "SPI.h"
#include "SD.h"
#include "SerialFlash.h"
#include "Audio.h"

#include "ModularServer.h"
#include "ModularDevice.h"

#include "utility/Constants.h"
#include "utility/SDInterface.h"


class AudioController : public ModularDevice
{
public:
  AudioController();
  virtual void setup();

  SDInterface & getSDInterface();
  bool playPath(const char * path);
  void playTone(size_t frequency, const ConstantString * const speaker_ptr);
  void playNoise(const ConstantString * const speaker_ptr);
  void stop();
  bool isPlaying();
  const char * getLastAudioPathPlayed();
  long getPosition();
  long getLength();
  bool codecEnabled();
  bool isAudioPath(const char * path);
  void updateVolume();

private:
  modular_server::Field fields_[audio_controller::constants::FIELD_COUNT_MAX];
  modular_server::Parameter parameters_[audio_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Method methods_[audio_controller::constants::METHOD_COUNT_MAX];
  modular_server::Interrupt interrupts_[modular_device::constants::INTERRUPT_COUNT_MAX];

  // EventController<audio_controller::constants::EVENT_COUNT_MAX> event_controller_;

  // IndexedContainer<audio_controller::constants::PulseInfo,
  //                  audio_controller::constants::INDEXED_PULSES_COUNT_MAX> indexed_pulses_;

  bool codec_enabled_;
  constants::audio_t audio_type_playing_;
  bool playing_;
  char path_played_[constants::STRING_LENGTH_PATH];
  SDInterface sd_interface_;
  void enableAudioCodec();
  void updatePlaying();
  void addDirectoryToResponse(File dir, const char * pwd);

  // Callbacks
  void getSDCardInfoCallback();
  void getAudioPathsCallback();
  void playPathCallback();
  void playToneCallback();
  void playNoiseCallback();
  void stopCallback();
  void isPlayingCallback();
  void getLastAudioPathPlayedCallback();
  void getPositionCallback();
  void getLengthCallback();
  void getPercentCompleteCallback();

};

#endif

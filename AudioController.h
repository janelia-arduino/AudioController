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

#include "ModularServer.h"
#include "ModularDevice.h"

#include "utility/Constants.h"


class AudioController : public ModularDevice
{
public:
  AudioController();
  virtual void setup();
  void setChannelOn(const size_t channel, const audio_controller::constants::Polarity polarity);
  void setChannelOff(const size_t channel);
  void setChannelsOn(const uint32_t channels, const audio_controller::constants::Polarity polarity);
  void setChannelsOff(const uint32_t channels);
  void setAllChannelsOn(const audio_controller::constants::Polarity polarity);
  void setAllChannelsOff();
  int addPwm(const uint32_t channels,
             const audio_controller::constants::Polarity polarity,
             const long delay,
             const long period,
             const long on_duration,
             const long count);
  int startPwm(const uint32_t channels,
               const audio_controller::constants::Polarity polarity,
               const long delay,
               const long period,
               const long on_duration);
  int addTogglePwm(const uint32_t channels,
                   const audio_controller::constants::Polarity polarity,
                   const long delay,
                   const long period,
                   const long on_duration,
                   const long count);
  int startTogglePwm(const uint32_t channels,
                     const audio_controller::constants::Polarity polarity,
                     const long delay,
                     const long period,
                     const long on_duration);
  void stopPwm(const int pwm_index);
  void stopAllPwm();
  uint32_t arrayToChannels(ArduinoJson::JsonArray & channels_array);
  audio_controller::constants::Polarity stringToPolarity(const char * string);

  // Callbacks
  virtual void startPwmCallback(int index);
  virtual void stopPwmCallback(int index);

private:
  modular_server::Field fields_[audio_controller::constants::FIELD_COUNT_MAX];
  modular_server::Parameter parameters_[audio_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Method methods_[audio_controller::constants::METHOD_COUNT_MAX];

  EventController<audio_controller::constants::EVENT_COUNT_MAX> event_controller_;

  IndexedContainer<audio_controller::constants::PulseInfo,
                   audio_controller::constants::INDEXED_PULSES_COUNT_MAX> indexed_pulses_;

  // Callbacks
  void setChannelOnCallback();
  void setChannelOffCallback();
  void setChannelsOnCallback();
  void setChannelsOffCallback();
  void setAllChannelsOnCallback();
  void setAllChannelsOffCallback();
  void addPwmCallback();
  void startPwmCallback();
  void addTogglePwmCallback();
  void startTogglePwmCallback();
  void stopPwmCallback();
  void stopAllPwmCallback();
  void setChannelsOnCallback(int index);
  void setChannelsOffCallback(int index);
  void setChannelsOnReversedCallback(int index);

};

#endif

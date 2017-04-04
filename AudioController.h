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
#include "ModularDeviceBase.h"

#include "utility/Constants.h"
#include "utility/SDInterface.h"


class AudioController : public ModularDeviceBase
{
public:
  AudioController();
  virtual void setup();

  SDInterface & getSDInterface();
  bool playPath(const char * path);
  void playTone(const size_t frequency,
                ConstantString * const speaker_ptr);
  void playToneAt(const size_t frequency,
                  ConstantString * const speaker_ptr,
                  const double volume);
  void playNoise(ConstantString * const speaker_ptr);
  void playNoiseAt(ConstantString * const speaker_ptr,
                   const double volume);
  void playFilteredNoise(const size_t frequency,
                         const double bandwidth,
                         ConstantString * const speaker_ptr);
  void playFilteredNoiseAt(const size_t frequency,
                           const double bandwidth,
                           ConstantString * const speaker_ptr,
                           const double volume);
  void stop();
  bool isPlaying();
  const char * getLastAudioPathPlayed();
  long getPosition();
  long getLength();
  int getPercentComplete();
  bool codecEnabled();
  bool pathIsAudio(const char * path);
  void updateVolume();

  int addTonePwm(const size_t frequency,
                 ConstantString * const speaker_ptr,
                 const long delay,
                 const long period,
                 const long on_duration,
                 const long count);
  int addTonePwmAt(const size_t frequency,
                   ConstantString * const speaker_ptr,
                   const double volume,
                   const long delay,
                   const long period,
                   const long on_duration,
                   const long count);
  int startTonePwm(const size_t frequency,
                   ConstantString * const speaker_ptr,
                   const long delay,
                   const long period,
                   const long on_duration);
  int startTonePwmAt(const size_t frequency,
                     ConstantString * const speaker_ptr,
                     const double volume,
                     const long delay,
                     const long period,
                     const long on_duration);
  int addNoisePwm(ConstantString * const speaker_ptr,
                  const long delay,
                  const long period,
                  const long on_duration,
                  const long count);
  int addNoisePwmAt(ConstantString * const speaker_ptr,
                    const double volume,
                    const long delay,
                    const long period,
                    const long on_duration,
                    const long count);
  int startNoisePwm(ConstantString * const speaker_ptr,
                    const long delay,
                    const long period,
                    const long on_duration);
  int startNoisePwmAt(ConstantString * const speaker_ptr,
                      const double volume,
                      const long delay,
                      const long period,
                      const long on_duration);
  int addFilteredNoisePwm(const size_t frequency,
                          const double bandwidth,
                          ConstantString * const speaker_ptr,
                          const long delay,
                          const long period,
                          const long on_duration,
                          const long count);
  int addFilteredNoisePwmAt(const size_t frequency,
                            const double bandwidth,
                            ConstantString * const speaker_ptr,
                            const double volume,
                            const long delay,
                            const long period,
                            const long on_duration,
                            const long count);
  int startFilteredNoisePwm(const size_t frequency,
                            const double bandwidth,
                            ConstantString * const speaker_ptr,
                            const long delay,
                            const long period,
                            const long on_duration);
  int startFilteredNoisePwmAt(const size_t frequency,
                              const double bandwidth,
                              ConstantString * const speaker_ptr,
                              const double volume,
                              const long delay,
                              const long period,
                              const long on_duration);
  void stopPwm(const int pwm_index);
  void stopAllPwm();
  bool isPulsing();

  // Handlers
  virtual void startPwmHandler(int index);
  virtual void stopPwmHandler(int index);

private:
  modular_server::Interrupt interrupts_[audio_controller::constants::INTERRUPT_COUNT_MAX];

  modular_server::Property properties_[audio_controller::constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[audio_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[audio_controller::constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[audio_controller::constants::CALLBACK_COUNT_MAX];

  EventController<audio_controller::constants::EVENT_COUNT_MAX> event_controller_;

  IndexedContainer<audio_controller::constants::PulseInfo,
                   audio_controller::constants::INDEXED_PULSES_COUNT_MAX> indexed_pulses_;

  bool codec_enabled_;
  audio_controller::constants::audio_t audio_type_playing_;
  bool playing_;
  char path_played_[audio_controller::constants::STRING_LENGTH_PATH];
  SDInterface sd_interface_;
  void enableAudioCodec();
  void updatePlaying();
  void addDirectoryToResponse(File dir, const char * pwd);
  ConstantString * const stringToSpeakerPtr(const char * string);
  bool pulsing_;

  // Handlers
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
  void playToneHandler(int index);
  void playNoiseHandler(int index);
  void playFilteredNoiseHandler(int index);
  void stopHandler(int index);

};

#endif

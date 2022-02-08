// ----------------------------------------------------------------------------
// AudioApparatus.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef AUDIO_APPARATUS_H
#define AUDIO_APPARATUS_H
#include <Functor.h>

#include <IndexedContainer.h>
#include <EventController.h>

#include <Audio.h>

#include "AudioController/Constants.h"
#include "AudioController/SDInterface.h"


class AudioApparatus
{
public:
  AudioApparatus();
  virtual void setup();

  bool playPath(const char * path);
  void playToneAt(size_t frequency,
    audio_apparatus::constants::speaker_t speaker,
    long volume);
  void playNoiseAt(audio_apparatus::constants::speaker_t speaker,
    long volume);
  void playFilteredNoiseAt(size_t frequency,
    double bandwidth,
    audio_apparatus::constants::speaker_t speaker,
    long volume);
  void stop();
  bool isPlaying();
  const char * getLastAudioPathPlayed();
  long getPosition();
  long getLength();
  int getPercentComplete();
  bool codecEnabled();
  bool pathIsAudio(const char * path);

  int addTonePwmAt(size_t frequency,
    audio_apparatus::constants::speaker_t speaker,
    long volume,
    long delay,
    long period,
    long on_duration,
    long count);
  int startTonePwmAt(size_t frequency,
    audio_apparatus::constants::speaker_t speaker,
    long volume,
    long delay,
    long period,
    long on_duration);
  int addNoisePwmAt(audio_apparatus::constants::speaker_t speaker,
    long volume,
    long delay,
    long period,
    long on_duration,
    long count);
  int startNoisePwmAt(audio_apparatus::constants::speaker_t speaker,
    long volume,
    long delay,
    long period,
    long on_duration);
  int addFilteredNoisePwmAt(size_t frequency,
    double bandwidth,
    audio_apparatus::constants::speaker_t speaker,
    long volume,
    long delay,
    long period,
    long on_duration,
    long count);
  int startFilteredNoisePwmAt(size_t frequency,
    double bandwidth,
    audio_apparatus::constants::speaker_t speaker,
    long volume,
    long delay,
    long period,
    long on_duration);
  void stopPwm(int pwm_index);
  void stopAllPwm();
  bool isPulsing();

  // Handlers
  virtual void startPwmHandler(int index);
  virtual void stopPwmHandler(int index);

protected:
  SDInterface sd_interface_;

  void setVolume(long volume,
    double stereo_speaker_gain = 1.0,
    double pcb_speaker_gain = 1.0);
  void setPlaying(bool playing);

private:
  const static int8_t BAD_INDEX = -1;

  EventController<audio_apparatus::constants::EVENT_COUNT_MAX> event_controller_;

  struct PulseInfo
  {
    size_t frequency;
    double bandwidth;
    audio_apparatus::constants::speaker_t speaker;
    long volume;
    EventIdPair event_id_pair;
  };

  IndexedContainer<PulseInfo,
    audio_apparatus::constants::INDEXED_PULSES_COUNT_MAX> indexed_pulses_;

  bool codec_enabled_;
  audio_apparatus::constants::audio_t audio_type_playing_;
  bool playing_;
  char path_played_[audio_apparatus::constants::STRING_LENGTH_PATH];
  bool pulsing_;

  void enableAudioCodec();
  void updatePlaying();

  // Handlers
  void playToneHandler(int index);
  void playNoiseHandler(int index);
  void playFilteredNoiseHandler(int index);
  void stopHandler(int index);

};

#endif

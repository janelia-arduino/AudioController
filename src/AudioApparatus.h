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

class AudioApparatus
{
public:
  AudioApparatus();
  virtual void setup();

  bool playPath(const char * path);
  enum speaker_t
  {
    SPEAKER_ALL,
    SPEAKER_LEFT,
    SPEAKER_RIGHT,
  };
  void playToneAt(size_t frequency,
    speaker_t speaker,
    long volume);
  void playNoiseAt(speaker_t speaker,
    long volume);
  void playFilteredNoiseAt(size_t frequency,
    double bandwidth,
    speaker_t speaker,
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
    speaker_t speaker,
    long volume,
    long delay,
    long period,
    long on_duration,
    long count);
  int startTonePwmAt(size_t frequency,
    speaker_t speaker,
    long volume,
    long delay,
    long period,
    long on_duration);
  int addNoisePwmAt(speaker_t speaker,
    long volume,
    long delay,
    long period,
    long on_duration,
    long count);
  int startNoisePwmAt(speaker_t speaker,
    long volume,
    long delay,
    long period,
    long on_duration);
  int addFilteredNoisePwmAt(size_t frequency,
    double bandwidth,
    speaker_t speaker,
    long volume,
    long delay,
    long period,
    long on_duration,
    long count);
  int startFilteredNoisePwmAt(size_t frequency,
    double bandwidth,
    speaker_t speaker,
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

private:
  enum{EVENT_COUNT_MAX=8};
  enum{INDEXED_PULSES_COUNT_MAX=4};

  const static int8_t BAD_INDEX = -1;

  EventController<EVENT_COUNT_MAX> event_controller_;

  struct PulseInfo
  {
    size_t frequency;
    double bandwidth;
    speaker_t speaker;
    long volume;
    EventIdPair event_id_pair;
  };

  IndexedContainer<PulseInfo,
    INDEXED_PULSES_COUNT_MAX> indexed_pulses_;

  enum audio_t
  {
    RAW_TYPE,
    WAV_TYPE,
    TONE_TYPE,
    NOISE_TYPE,
  };

  bool codec_enabled_;
  audio_t audio_type_playing_;
  bool playing_;
  char path_played_[STRING_LENGTH_PATH];
  SDInterface sd_interface_;
  bool pulsing_;

  void enableAudioCodec();
  void updatePlaying();
  void setVolume(long volume);
  void setPlaying(bool playing);

  // Handlers
  void playToneHandler(int index);
  void playNoiseHandler(int index);
  void playFilteredNoiseHandler(int index);
  void stopHandler(int index);

};

#endif

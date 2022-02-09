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


namespace audio_apparatus
{
extern AudioSynthNoiseWhite     g_noise_left;
extern AudioSynthNoiseWhite     g_noise_right;
extern AudioSynthWaveformSine   g_tone_left;
extern AudioSynthWaveformSine   g_tone_right;
#if !defined(__IMXRT1062__)
extern AudioPlaySdWav           g_play_sd_wav;
#endif
extern AudioFilterBiquad        g_biquad_left;
#if !defined(__IMXRT1062__)
extern AudioPlaySdRaw           g_play_sd_raw;
#endif
extern AudioFilterBiquad        g_biquad_right;
extern AudioMixer4              g_mixer_left;
extern AudioMixer4              g_mixer_right;
#if !defined(__IMXRT1062__)
extern AudioMixer4              g_mixer_dac;
#endif
extern AudioOutputI2S           g_stereo_speaker;
#if !defined(__IMXRT1062__)
extern AudioOutputAnalog        g_pcb_speaker;
#endif
extern AudioControlSGTL5000     g_sgtl5000;
}

template <uint8_t EVENT_COUNT_MAX>
class AudioApparatus
{
public:
  AudioApparatus();
  void setup(EventController<EVENT_COUNT_MAX> & event_controller);

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

  void setVolume(long volume,
    double stereo_speaker_gain = audio_apparatus::constants::stereo_speaker_gain_default,
    double pcb_speaker_gain = audio_apparatus::constants::pcb_speaker_gain_default);
  void setPlaying(bool playing);

  SDInterface & getSDInterface();

  long getAudioMemoryUsage();
  long getAudioMemoryUsageMax();
  void resetAudioMemoryUsageMax();
  double getAudioProcessorUsage();
  double getAudioProcessorUsageMax();
  void resetAudioProcessorUsageMax();

  // Handlers
  void startPwmHandler(int index);
  void stopPwmHandler(int index);

private:
  const static int8_t BAD_INDEX = -1;

  EventController<EVENT_COUNT_MAX> * event_controller_ptr_;

  SDInterface sd_interface_;

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

#include "AudioController/AudioApparatusDefinitions.h"

#endif

// ----------------------------------------------------------------------------
// AudioApparatus.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef AUDIO_APPARATUS_H
#define AUDIO_APPARATUS_H

#include <Audio.h>


class AudioApparatus
{
public:
  AudioApparatus();
  virtual void setup();

  SDInterface & getSDInterface();
  bool playPath(const char * path);
  void playTone(size_t frequency,
    const ConstantString * const speaker_ptr);
  void playToneAt(size_t frequency,
    const ConstantString * const speaker_ptr,
    long volume);
  void playNoise(const ConstantString * const speaker_ptr);
  void playNoiseAt(const ConstantString * const speaker_ptr,
    long volume);
  void playFilteredNoise(size_t frequency,
    double bandwidth,
    const ConstantString * const speaker_ptr);
  void playFilteredNoiseAt(size_t frequency,
    double bandwidth,
    const ConstantString * const speaker_ptr,
    long volume);
  void stop();
  bool isPlaying();
  const char * getLastAudioPathPlayed();
  long getPosition();
  long getLength();
  int getPercentComplete();
  bool codecEnabled();
  bool pathIsAudio(const char * path);

  int addTonePwm(size_t frequency,
    const ConstantString * const speaker_ptr,
    long delay,
    long period,
    long on_duration,
    long count);
  int addTonePwmAt(size_t frequency,
    const ConstantString * const speaker_ptr,
    long volume,
    long delay,
    long period,
    long on_duration,
    long count);
  int startTonePwm(size_t frequency,
    const ConstantString * const speaker_ptr,
    long delay,
    long period,
    long on_duration);
  int startTonePwmAt(size_t frequency,
    const ConstantString * const speaker_ptr,
    long volume,
    long delay,
    long period,
    long on_duration);
  int addNoisePwm(const ConstantString * const speaker_ptr,
    long delay,
    long period,
    long on_duration,
    long count);
  int addNoisePwmAt(const ConstantString * const speaker_ptr,
    long volume,
    long delay,
    long period,
    long on_duration,
    long count);
  int startNoisePwm(const ConstantString * const speaker_ptr,
    long delay,
    long period,
    long on_duration);
  int startNoisePwmAt(const ConstantString * const speaker_ptr,
    long volume,
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
  int addFilteredNoisePwmAt(size_t frequency,
    double bandwidth,
    const ConstantString * const speaker_ptr,
    long volume,
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
  int startFilteredNoisePwmAt(size_t frequency,
    double bandwidth,
    const ConstantString * const speaker_ptr,
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
  EventApparatus<audio_apparatus::constants::EVENT_COUNT_MAX> event_apparatus_;

  IndexedContainer<audio_apparatus::constants::PulseInfo,
    audio_apparatus::constants::INDEXED_PULSES_COUNT_MAX> indexed_pulses_;

  bool codec_enabled_;
  audio_apparatus::constants::audio_t audio_type_playing_;
  bool playing_;
  char path_played_[audio_apparatus::constants::STRING_LENGTH_PATH];
  SDInterface sd_interface_;
  bool pulsing_;

  void enableAudioCodec();
  void updatePlaying();
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
  void playToneHandler(int index);
  void playNoiseHandler(int index);
  void playFilteredNoiseHandler(int index);
  void stopHandler(int index);

};

#endif

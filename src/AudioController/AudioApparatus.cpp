// ----------------------------------------------------------------------------
// AudioApparatus.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../AudioApparatus.h"


namespace audio_controller
{
// GUItool: begin automatically generated code
AudioSynthNoiseWhite     g_noise_left;   //xy=85,251
AudioSynthNoiseWhite     g_noise_right;  //xy=89,288
AudioSynthWaveformSine   g_tone_left;    //xy=250,176
AudioSynthWaveformSine   g_tone_right;   //xy=256,215
#if !defined(__IMXRT1062__)
AudioPlaySdWav           g_play_sd_wav;  //xy=263,102
#endif
AudioFilterBiquad        g_biquad_left;  //xy=263,251
#if !defined(__IMXRT1062__)
AudioPlaySdRaw           g_play_sd_raw;  //xy=264,139
#endif
AudioFilterBiquad        g_biquad_right; //xy=265,289
AudioMixer4              g_mixer_left;   //xy=500,111
AudioMixer4              g_mixer_right;  //xy=506,187
#if !defined(__IMXRT1062__)
AudioMixer4              g_mixer_dac;    //xy=702,188
#endif
AudioOutputI2S           g_stereo_speaker; //xy=717,126
#if !defined(__IMXRT1062__)
AudioOutputAnalog        g_pcb_speaker;  //xy=890,188
#endif
AudioConnection          patchCord1(g_noise_left, g_biquad_left);
AudioConnection          patchCord2(g_noise_right, g_biquad_right);
AudioConnection          patchCord3(g_tone_left, 0, g_mixer_left, 2);
AudioConnection          patchCord4(g_tone_right, 0, g_mixer_right, 2);
#if !defined(__IMXRT1062__)
AudioConnection          patchCord5(g_play_sd_wav, 0, g_mixer_left, 0);
AudioConnection          patchCord6(g_play_sd_wav, 1, g_mixer_right, 0);
#endif
AudioConnection          patchCord7(g_biquad_left, 0, g_mixer_left, 3);
#if !defined(__IMXRT1062__)
AudioConnection          patchCord8(g_play_sd_raw, 0, g_mixer_left, 1);
AudioConnection          patchCord9(g_play_sd_raw, 0, g_mixer_right, 1);
#endif
AudioConnection          patchCord10(g_biquad_right, 0, g_mixer_right, 3);
AudioConnection          patchCord11(g_mixer_left, 0, g_stereo_speaker, 0);
#if !defined(__IMXRT1062__)
AudioConnection          patchCord12(g_mixer_left, 0, g_mixer_dac, 0);
#endif
AudioConnection          patchCord13(g_mixer_right, 0, g_stereo_speaker, 1);
#if !defined(__IMXRT1062__)
AudioConnection          patchCord14(g_mixer_right, 0, g_mixer_dac, 1);
AudioConnection          patchCord15(g_mixer_dac, g_pcb_speaker);
#endif
AudioControlSGTL5000     g_sgtl5000;     //xy=498,36
// GUItool: end automatically generated code
}

using namespace audio_controller;

AudioApparatus::AudioApparatus()
{
  codec_enabled_ = false;
  playing_ = false;
  path_played_[0] = 0;
  pulsing_ = false;
}

void AudioApparatus::setup()
{
  // Event Controller Setup
  event_controller_.setup();

  // Audio Setup
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(constants::AUDIO_MEMORY_BLOCK_COUNT);
  AudioMemoryUsageMaxReset();

  enableAudioCodec();

#if !defined(__IMXRT1062__)
  g_pcb_speaker.analogReference(constants::pcb_speaker_reference);
#endif

  // Setup SD Card
#if !defined(__IMXRT1062__)
  sd_interface_.setup();
#endif

  setVolume(25);

}

bool AudioApparatus::playPath(const char * path)
{
  bool playing = false;
#if !defined(__IMXRT1062__)
  char path_upper[constants::STRING_LENGTH_PATH];
  String(path).toUpperCase().toCharArray(path_upper,constants::STRING_LENGTH_PATH);

  bool sd_specified = false;
  char * sd_path;
  sd_path = strstr(path_upper,constants::sd_prefix);
  if (sd_path == path_upper)
  {
    sd_specified = true;
    // remove "/SD" from path
    sd_path = sd_path+strlen(constants::sd_prefix)-1;
  }

  // default to SD card if none specified
  if (!sd_specified)
  {
    sd_specified = true;
    sd_path = path_upper;
  }

  stop();

  char * raw_ext = strstr(path_upper,constants::audio_ext_raw);
  if (raw_ext != NULL)
  {
    audio_type_playing_ = constants::RAW_TYPE;
    if (sd_specified)
    {
      playing = g_play_sd_raw.play(sd_path);
    }
  }

  if (!playing)
  {
    char * wav_ext = strstr(path_upper,constants::audio_ext_wav);
    if (wav_ext != NULL)
    {
      audio_type_playing_ = constants::WAV_TYPE;
      if (sd_specified)
      {
        playing = g_play_sd_wav.play(sd_path);
      }
    }
  }

  setPlaying(playing);
  if (playing)
  {
    path_played_[0] = 0;
    strcpy(path_played_,path_upper);
  }
#endif
  return playing;
}

void AudioApparatus::playToneAt(size_t frequency,
  constants::speaker_t speaker,
  long volume)
{
  stop();
  if ((volume <= 0) || (frequency < constants::frequency_stop_threshold))
  {
    return;
  }
  audio_type_playing_ = constants::TONE_TYPE;
  if ((speaker == constants::SPEAKER_ALL) || (speaker == constants::SPEAKER_LEFT))
  {
    g_tone_left.amplitude(0);
    g_tone_left.frequency(frequency);
    g_tone_left.amplitude(1);
  }
  if ((speaker == constants::SPEAKER_ALL) || (speaker == constants::SPEAKER_RIGHT))
  {
    g_tone_right.amplitude(0);
    g_tone_right.frequency(frequency);
    g_tone_right.amplitude(1);
  }
  setVolume(volume);
  setPlaying(true);

}

void AudioApparatus::playNoiseAt(constants::speaker_t speaker,
  long volume)
{
  stop();
  if (volume <= 0)
  {
    return;
  }
  audio_type_playing_ = constants::NOISE_TYPE;
  if ((speaker == constants::SPEAKER_ALL) || (speaker == constants::SPEAKER_LEFT))
  {
    g_biquad_left.setCoefficients(constants::FILTER_STAGE_0,constants::allpass_filter_coefs);
    g_noise_left.amplitude(1);
  }
  if ((speaker == constants::SPEAKER_ALL) || (speaker == constants::SPEAKER_RIGHT))
  {
    g_biquad_right.setCoefficients(constants::FILTER_STAGE_0,constants::allpass_filter_coefs);
    g_noise_right.amplitude(1);
  }
  setVolume(volume);
  setPlaying(true);

}

void AudioApparatus::playFilteredNoiseAt(size_t frequency,
  double bandwidth,
  constants::speaker_t speaker,
  long volume)
{
  stop();
  if ((volume <= 0) || (frequency < constants::frequency_stop_threshold))
  {
    return;
  }
  audio_type_playing_ = constants::NOISE_TYPE;
  if ((speaker == constants::SPEAKER_ALL) || (speaker == constants::SPEAKER_LEFT))
  {
    g_biquad_left.setBandpass(constants::FILTER_STAGE_0,frequency,bandwidth);
    g_noise_left.amplitude(1);
  }
  if ((speaker == constants::SPEAKER_ALL) || (speaker == constants::SPEAKER_RIGHT))
  {
    g_biquad_right.setBandpass(constants::FILTER_STAGE_0,frequency,bandwidth);
    g_noise_right.amplitude(1);
  }
  setVolume(volume);
  setPlaying(true);
}

void AudioApparatus::stop()
{
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case constants::RAW_TYPE:
      {
#if !defined(__IMXRT1062__)
        g_play_sd_raw.stop();
#endif
        break;
      }
      case constants::WAV_TYPE:
      {
#if !defined(__IMXRT1062__)
        g_play_sd_wav.stop();
#endif
        break;
      }
      case constants::TONE_TYPE:
      {
        g_tone_left.amplitude(0);
        g_tone_right.amplitude(0);
      }
      case constants::NOISE_TYPE:
      {
        g_noise_left.amplitude(0);
        g_noise_right.amplitude(0);
      }
    }
    setPlaying(false);
  }
}

bool AudioApparatus::isPlaying()
{
  return playing_;
}

const char * AudioApparatus::getLastAudioPathPlayed()
{
  return path_played_;
}

long AudioApparatus::getPosition()
{
  long position = 0;
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case constants::RAW_TYPE:
      {
#if !defined(__IMXRT1062__)
        position = g_play_sd_raw.positionMillis();
#endif
        break;
      }
      case constants::WAV_TYPE:
      {
#if !defined(__IMXRT1062__)
        position = g_play_sd_wav.positionMillis();
#endif
        break;
      }
      case constants::TONE_TYPE:
      {
        break;
      }
      case constants::NOISE_TYPE:
      {
        break;
      }
    }
  }
  return position;
}

long AudioApparatus::getLength()
{
  long length = 0;
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case constants::RAW_TYPE:
      {
#if !defined(__IMXRT1062__)
        length = g_play_sd_raw.lengthMillis();
#endif
        break;
      }
      case constants::WAV_TYPE:
      {
#if !defined(__IMXRT1062__)
        length = g_play_sd_wav.lengthMillis();
#endif
        break;
      }
      case constants::TONE_TYPE:
      {
        break;
      }
      case constants::NOISE_TYPE:
      {
        break;
      }
    }
  }
  return length;
}

int AudioApparatus::getPercentComplete()
{
  long position = getPosition();
  long length = getLength();
  long percent_complete;
  if (length > 0)
  {
    percent_complete = (100*position)/length;
  }
  else if (isPlaying())
  {
    percent_complete = 0;
  }
  else
  {
    percent_complete = 100;
  }
  return percent_complete;
}

bool AudioApparatus::codecEnabled()
{
  return codec_enabled_;
}

bool AudioApparatus::pathIsAudio(const char * path)
{
  char path_upper[constants::STRING_LENGTH_PATH];
  String(path).toUpperCase().toCharArray(path_upper,constants::STRING_LENGTH_PATH);

  bool audio_path = false;
  for (unsigned int i=0;i<constants::AUDIO_EXT_COUNT;++i)
  {
    const char * audio_ext = constants::audio_exts[i];
    char * audio_ext_path = strstr(path_upper,audio_ext);
    if (audio_ext_path != NULL)
    {
      audio_path = true;
      break;
    }
  }
  return audio_path;
}

int AudioApparatus::addTonePwmAt(size_t frequency,
  constants::speaker_t speaker,
  long volume,
  long delay,
  long period,
  long on_duration,
  long count)
{
  if (indexed_pulses_.full())
  {
    return BAD_INDEX;
  }
  PulseInfo pulse_info;
  pulse_info.frequency = frequency;
  pulse_info.speaker = speaker;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::playToneHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int AudioApparatus::startTonePwmAt(size_t frequency,
  constants::speaker_t speaker,
  long volume,
  long delay,
  long period,
  long on_duration)
{
  if (indexed_pulses_.full())
  {
    return -1;
  }
  PulseInfo pulse_info;
  pulse_info.frequency = frequency;
  pulse_info.speaker = speaker;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::playToneHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int AudioApparatus::addNoisePwmAt(constants::speaker_t speaker,
  long volume,
  long delay,
  long period,
  long on_duration,
  long count)
{
  if (indexed_pulses_.full())
  {
    return BAD_INDEX;
  }
  PulseInfo pulse_info;
  pulse_info.speaker = speaker;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::playNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int AudioApparatus::startNoisePwmAt(constants::speaker_t speaker,
  long volume,
  long delay,
  long period,
  long on_duration)
{
  if (indexed_pulses_.full())
  {
    return -1;
  }
  PulseInfo pulse_info;
  pulse_info.speaker = speaker;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::playNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int AudioApparatus::addFilteredNoisePwmAt(size_t frequency,
  double bandwidth,
  constants::speaker_t speaker,
  long volume,
  long delay,
  long period,
  long on_duration,
  long count)
{
  if (indexed_pulses_.full())
  {
    return BAD_INDEX;
  }
  PulseInfo pulse_info;
  pulse_info.frequency = frequency;
  pulse_info.bandwidth = bandwidth;
  pulse_info.speaker = speaker;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::playFilteredNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int AudioApparatus::startFilteredNoisePwmAt(size_t frequency,
  double bandwidth,
  constants::speaker_t speaker,
  long volume,
  long delay,
  long period,
  long on_duration)
{
  if (indexed_pulses_.full())
  {
    return -1;
  }
  PulseInfo pulse_info;
  pulse_info.frequency = frequency;
  pulse_info.bandwidth = bandwidth;
  pulse_info.speaker = speaker;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::playFilteredNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

void AudioApparatus::stopPwm(int pwm_index)
{
  if (pwm_index < 0)
  {
    return;
  }
  if (indexed_pulses_.indexHasValue(pwm_index))
  {
    PulseInfo pulse_info = indexed_pulses_[pwm_index];
    event_controller_.remove(pulse_info.event_id_pair);
  }
}

void AudioApparatus::stopAllPwm()
{
  for (size_t i=0; i<constants::INDEXED_PULSES_COUNT_MAX; ++i)
  {
    stopPwm(i);
  }
}

bool AudioApparatus::isPulsing()
{
  return pulsing_;
}

void AudioApparatus::enableAudioCodec()
{
  pinMode(SDA,INPUT);
  pinMode(SCL,INPUT);
  if (digitalRead(SDA) && digitalRead(SCL))
  {
    // This may wait forever if the SDA & SCL pins lack
    // pullup resistors so check first
    codec_enabled_ = true;
    g_sgtl5000.enable();
  }
}

void AudioApparatus::updatePlaying()
{
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case constants::RAW_TYPE:
      {
#if !defined(__IMXRT1062__)
        setPlaying(g_play_sd_raw.isPlaying());
#endif
        break;
      }
      case constants::WAV_TYPE:
      {
#if !defined(__IMXRT1062__)
        setPlaying(g_play_sd_wav.isPlaying());
#endif
        break;
      }
      case constants::TONE_TYPE:
      {
        break;
      }
      case constants::NOISE_TYPE:
      {
        break;
      }
    }
  }
}

void AudioApparatus::startPwmHandler(int index)
{
  pulsing_ = true;
}

void AudioApparatus::stopPwmHandler(int index)
{
  stop();
  indexed_pulses_.remove(index);
  pulsing_ = !indexed_pulses_.empty();
}

void AudioApparatus::setVolume(long volume,
  double stereo_speaker_gain,
  double pcb_speaker_gain)
{
  if (codecEnabled())
  {
    g_sgtl5000.volume(((double)volume*stereo_speaker_gain)/100.0);
  }

#if !defined(__IMXRT1062__)
  double pcb_speaker_total_gain = (constants::pcb_speaker_channel_gain*constants::pcb_speaker_pre_gain*(double)volume*pcb_speaker_gain)/100.0;
  g_mixer_dac.gain(0,pcb_speaker_total_gain);
  g_mixer_dac.gain(1,pcb_speaker_total_gain);
#endif
}

void AudioApparatus::setPlaying(bool playing)
{
  playing_ = playing;
}

void AudioApparatus::playToneHandler(int index)
{
  size_t frequency = indexed_pulses_[index].frequency;
  constants::speaker_t speaker = indexed_pulses_[index].speaker;
  long volume = indexed_pulses_[index].volume;
  playToneAt(frequency,speaker,volume);
}

void AudioApparatus::playNoiseHandler(int index)
{
  constants::speaker_t speaker = indexed_pulses_[index].speaker;
  long volume = indexed_pulses_[index].volume;
  playNoiseAt(speaker,volume);
}

void AudioApparatus::playFilteredNoiseHandler(int index)
{
  size_t frequency = indexed_pulses_[index].frequency;
  double bandwidth = indexed_pulses_[index].bandwidth;
  constants::speaker_t speaker = indexed_pulses_[index].speaker;
  long volume = indexed_pulses_[index].volume;
  playFilteredNoiseAt(frequency,bandwidth,speaker,volume);
}

void AudioApparatus::stopHandler(int index)
{
  stop();
}

// ----------------------------------------------------------------------------
// AudioApparatusDefinitions.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef AUDIO_APPARATUS_DEFINITIONS_H
#define AUDIO_APPARATUS_DEFINITIONS_H


template <uint8_t EVENT_COUNT_MAX>
AudioApparatus<EVENT_COUNT_MAX>::AudioApparatus()
{
  event_controller_ptr_ = nullptr;
  codec_enabled_ = false;
  playing_ = false;
  path_played_[0] = 0;
  pulsing_ = false;
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::setup(EventController<EVENT_COUNT_MAX> & event_controller)
{
  // Event Controller Setup
  event_controller_ptr_ = &event_controller;

  // Audio Setup
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(audio_apparatus::constants::AUDIO_MEMORY_BLOCK_COUNT);
  AudioMemoryUsageMaxReset();

  enableAudioCodec();

#if !defined(__IMXRT1062__)
  audio_apparatus::g_pcb_speaker.analogReference(audio_apparatus::constants::pcb_speaker_reference);
#endif

  // Setup SD Card
#if !defined(__IMXRT1062__)
  sd_interface_.setup();
#endif

  setVolume(audio_apparatus::constants::volume_default);

}

template <uint8_t EVENT_COUNT_MAX>
bool AudioApparatus<EVENT_COUNT_MAX>::playPath(const char * path)
{
  bool playing = false;
#if !defined(__IMXRT1062__)
  char path_upper[audio_apparatus::constants::STRING_LENGTH_PATH];
  String(path).toUpperCase().toCharArray(path_upper,audio_apparatus::constants::STRING_LENGTH_PATH);

  bool sd_specified = false;
  char * sd_path;
  sd_path = strstr(path_upper,audio_apparatus::constants::sd_prefix);
  if (sd_path == path_upper)
  {
    sd_specified = true;
    // remove "/SD" from path
    sd_path = sd_path+strlen(audio_apparatus::constants::sd_prefix)-1;
  }

  // default to SD card if none specified
  if (!sd_specified)
  {
    sd_specified = true;
    sd_path = path_upper;
  }

  stop();

  char * raw_ext = strstr(path_upper,audio_apparatus::constants::audio_ext_raw);
  if (raw_ext != NULL)
  {
    audio_type_playing_ = audio_apparatus::constants::RAW_TYPE;
    if (sd_specified)
    {
      playing = audio_apparatus::g_play_sd_raw.play(sd_path);
    }
  }

  if (!playing)
  {
    char * wav_ext = strstr(path_upper,audio_apparatus::constants::audio_ext_wav);
    if (wav_ext != NULL)
    {
      audio_type_playing_ = audio_apparatus::constants::WAV_TYPE;
      if (sd_specified)
      {
        playing = audio_apparatus::g_play_sd_wav.play(sd_path);
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

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::playToneAt(size_t frequency,
  audio_apparatus::constants::speaker_t speaker,
  long volume)
{
  stop();
  if ((volume <= 0) || (frequency < audio_apparatus::constants::frequency_stop_threshold))
  {
    return;
  }
  audio_type_playing_ = audio_apparatus::constants::TONE_TYPE;
  if ((speaker == audio_apparatus::constants::SPEAKER_ALL) || (speaker == audio_apparatus::constants::SPEAKER_LEFT))
  {
    audio_apparatus::g_tone_left.amplitude(0);
    audio_apparatus::g_tone_left.frequency(frequency);
    audio_apparatus::g_tone_left.amplitude(1);
  }
  if ((speaker == audio_apparatus::constants::SPEAKER_ALL) || (speaker == audio_apparatus::constants::SPEAKER_RIGHT))
  {
    audio_apparatus::g_tone_right.amplitude(0);
    audio_apparatus::g_tone_right.frequency(frequency);
    audio_apparatus::g_tone_right.amplitude(1);
  }
  setVolume(volume);
  setPlaying(true);

}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::playNoiseAt(audio_apparatus::constants::speaker_t speaker,
  long volume)
{
  stop();
  if (volume <= 0)
  {
    return;
  }
  audio_type_playing_ = audio_apparatus::constants::NOISE_TYPE;
  if ((speaker == audio_apparatus::constants::SPEAKER_ALL) || (speaker == audio_apparatus::constants::SPEAKER_LEFT))
  {
    audio_apparatus::g_biquad_left.setCoefficients(audio_apparatus::constants::FILTER_STAGE_0,audio_apparatus::constants::allpass_filter_coefs);
    audio_apparatus::g_noise_left.amplitude(1);
  }
  if ((speaker == audio_apparatus::constants::SPEAKER_ALL) || (speaker == audio_apparatus::constants::SPEAKER_RIGHT))
  {
    audio_apparatus::g_biquad_right.setCoefficients(audio_apparatus::constants::FILTER_STAGE_0,audio_apparatus::constants::allpass_filter_coefs);
    audio_apparatus::g_noise_right.amplitude(1);
  }
  setVolume(volume);
  setPlaying(true);

}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::playFilteredNoiseAt(size_t frequency,
  double bandwidth,
  audio_apparatus::constants::speaker_t speaker,
  long volume)
{
  stop();
  if ((volume <= 0) || (frequency < audio_apparatus::constants::frequency_stop_threshold))
  {
    return;
  }
  audio_type_playing_ = audio_apparatus::constants::NOISE_TYPE;
  if ((speaker == audio_apparatus::constants::SPEAKER_ALL) || (speaker == audio_apparatus::constants::SPEAKER_LEFT))
  {
    audio_apparatus::g_biquad_left.setBandpass(audio_apparatus::constants::FILTER_STAGE_0,frequency,bandwidth);
    audio_apparatus::g_noise_left.amplitude(1);
  }
  if ((speaker == audio_apparatus::constants::SPEAKER_ALL) || (speaker == audio_apparatus::constants::SPEAKER_RIGHT))
  {
    audio_apparatus::g_biquad_right.setBandpass(audio_apparatus::constants::FILTER_STAGE_0,frequency,bandwidth);
    audio_apparatus::g_noise_right.amplitude(1);
  }
  setVolume(volume);
  setPlaying(true);
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::stop()
{
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case audio_apparatus::constants::RAW_TYPE:
      {
#if !defined(__IMXRT1062__)
        audio_apparatus::g_play_sd_raw.stop();
#endif
        break;
      }
      case audio_apparatus::constants::WAV_TYPE:
      {
#if !defined(__IMXRT1062__)
        audio_apparatus::g_play_sd_wav.stop();
#endif
        break;
      }
      case audio_apparatus::constants::TONE_TYPE:
      {
        audio_apparatus::g_tone_left.amplitude(0);
        audio_apparatus::g_tone_right.amplitude(0);
      }
      case audio_apparatus::constants::NOISE_TYPE:
      {
        audio_apparatus::g_noise_left.amplitude(0);
        audio_apparatus::g_noise_right.amplitude(0);
      }
    }
    setPlaying(false);
  }
}

template <uint8_t EVENT_COUNT_MAX>
bool AudioApparatus<EVENT_COUNT_MAX>::isPlaying()
{
  return playing_;
}

template <uint8_t EVENT_COUNT_MAX>
const char * AudioApparatus<EVENT_COUNT_MAX>::getLastAudioPathPlayed()
{
  return path_played_;
}

template <uint8_t EVENT_COUNT_MAX>
long AudioApparatus<EVENT_COUNT_MAX>::getPosition()
{
  long position = 0;
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case audio_apparatus::constants::RAW_TYPE:
      {
#if !defined(__IMXRT1062__)
        position = audio_apparatus::g_play_sd_raw.positionMillis();
#endif
        break;
      }
      case audio_apparatus::constants::WAV_TYPE:
      {
#if !defined(__IMXRT1062__)
        position = audio_apparatus::g_play_sd_wav.positionMillis();
#endif
        break;
      }
      case audio_apparatus::constants::TONE_TYPE:
      {
        break;
      }
      case audio_apparatus::constants::NOISE_TYPE:
      {
        break;
      }
    }
  }
  return position;
}

template <uint8_t EVENT_COUNT_MAX>
long AudioApparatus<EVENT_COUNT_MAX>::getLength()
{
  long length = 0;
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case audio_apparatus::constants::RAW_TYPE:
      {
#if !defined(__IMXRT1062__)
        length = audio_apparatus::g_play_sd_raw.lengthMillis();
#endif
        break;
      }
      case audio_apparatus::constants::WAV_TYPE:
      {
#if !defined(__IMXRT1062__)
        length = audio_apparatus::g_play_sd_wav.lengthMillis();
#endif
        break;
      }
      case audio_apparatus::constants::TONE_TYPE:
      {
        break;
      }
      case audio_apparatus::constants::NOISE_TYPE:
      {
        break;
      }
    }
  }
  return length;
}

template <uint8_t EVENT_COUNT_MAX>
int AudioApparatus<EVENT_COUNT_MAX>::getPercentComplete()
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

template <uint8_t EVENT_COUNT_MAX>
bool AudioApparatus<EVENT_COUNT_MAX>::codecEnabled()
{
  return codec_enabled_;
}

template <uint8_t EVENT_COUNT_MAX>
bool AudioApparatus<EVENT_COUNT_MAX>::pathIsAudio(const char * path)
{
  char path_upper[audio_apparatus::constants::STRING_LENGTH_PATH];
  String(path).toUpperCase().toCharArray(path_upper,audio_apparatus::constants::STRING_LENGTH_PATH);

  bool audio_path = false;
  for (unsigned int i=0;i<audio_apparatus::constants::AUDIO_EXT_COUNT;++i)
  {
    const char * audio_ext = audio_apparatus::constants::audio_exts[i];
    char * audio_ext_path = strstr(path_upper,audio_ext);
    if (audio_ext_path != NULL)
    {
      audio_path = true;
      break;
    }
  }
  return audio_path;
}

template <uint8_t EVENT_COUNT_MAX>
int AudioApparatus<EVENT_COUNT_MAX>::addTonePwmAt(size_t frequency,
  audio_apparatus::constants::speaker_t speaker,
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
  EventIdPair event_id_pair = event_controller_ptr_->addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::playToneHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_ptr_->addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::startPwmHandler));
  event_controller_ptr_->addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_ptr_->enable(event_id_pair);
  return index;
}

template <uint8_t EVENT_COUNT_MAX>
int AudioApparatus<EVENT_COUNT_MAX>::startTonePwmAt(size_t frequency,
  audio_apparatus::constants::speaker_t speaker,
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
  EventIdPair event_id_pair = event_controller_ptr_->addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::playToneHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_ptr_->addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::startPwmHandler));
  event_controller_ptr_->addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_ptr_->enable(event_id_pair);
  return index;
}

template <uint8_t EVENT_COUNT_MAX>
int AudioApparatus<EVENT_COUNT_MAX>::addNoisePwmAt(audio_apparatus::constants::speaker_t speaker,
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
  EventIdPair event_id_pair = event_controller_ptr_->addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::playNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_ptr_->addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::startPwmHandler));
  event_controller_ptr_->addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_ptr_->enable(event_id_pair);
  return index;
}

template <uint8_t EVENT_COUNT_MAX>
int AudioApparatus<EVENT_COUNT_MAX>::startNoisePwmAt(audio_apparatus::constants::speaker_t speaker,
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
  EventIdPair event_id_pair = event_controller_ptr_->addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::playNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_ptr_->addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::startPwmHandler));
  event_controller_ptr_->addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_ptr_->enable(event_id_pair);
  return index;
}

template <uint8_t EVENT_COUNT_MAX>
int AudioApparatus<EVENT_COUNT_MAX>::addFilteredNoisePwmAt(size_t frequency,
  double bandwidth,
  audio_apparatus::constants::speaker_t speaker,
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
  EventIdPair event_id_pair = event_controller_ptr_->addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::playFilteredNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_ptr_->addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::startPwmHandler));
  event_controller_ptr_->addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_ptr_->enable(event_id_pair);
  return index;
}

template <uint8_t EVENT_COUNT_MAX>
int AudioApparatus<EVENT_COUNT_MAX>::startFilteredNoisePwmAt(size_t frequency,
  double bandwidth,
  audio_apparatus::constants::speaker_t speaker,
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
  EventIdPair event_id_pair = event_controller_ptr_->addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::playFilteredNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_ptr_->addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::startPwmHandler));
  event_controller_ptr_->addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioApparatus<EVENT_COUNT_MAX>::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_ptr_->enable(event_id_pair);
  return index;
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::stopPwm(int pwm_index)
{
  if (pwm_index < 0)
  {
    return;
  }
  if (indexed_pulses_.indexHasValue(pwm_index))
  {
    PulseInfo pulse_info = indexed_pulses_[pwm_index];
    event_controller_ptr_->remove(pulse_info.event_id_pair);
  }
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::stopAllPwm()
{
  for (size_t i=0; i<audio_apparatus::constants::INDEXED_PULSES_COUNT_MAX; ++i)
  {
    stopPwm(i);
  }
}

template <uint8_t EVENT_COUNT_MAX>
bool AudioApparatus<EVENT_COUNT_MAX>::isPulsing()
{
  return pulsing_;
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::setVolume(long volume,
  double stereo_speaker_gain,
  double pcb_speaker_gain)
{
  if (codecEnabled())
  {
    audio_apparatus::g_sgtl5000.volume(((double)volume*stereo_speaker_gain)/100.0);
  }

#if !defined(__IMXRT1062__)
  double pcb_speaker_total_gain = (audio_apparatus::constants::pcb_speaker_channel_gain*audio_apparatus::constants::pcb_speaker_pre_gain*(double)volume*pcb_speaker_gain)/100.0;
  audio_apparatus::g_mixer_dac.gain(0,pcb_speaker_total_gain);
  audio_apparatus::g_mixer_dac.gain(1,pcb_speaker_total_gain);
#endif
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::setPlaying(bool playing)
{
  playing_ = playing;
}

template <uint8_t EVENT_COUNT_MAX>
SDInterface & AudioApparatus<EVENT_COUNT_MAX>::getSDInterface()
{
  return sd_interface_;
}

template <uint8_t EVENT_COUNT_MAX>
long AudioApparatus<EVENT_COUNT_MAX>::getAudioMemoryUsage()
{
  return AudioMemoryUsage();
}

template <uint8_t EVENT_COUNT_MAX>
long AudioApparatus<EVENT_COUNT_MAX>::getAudioMemoryUsageMax()
{
  return AudioMemoryUsageMax();
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::resetAudioMemoryUsageMax()
{
  AudioMemoryUsageMaxReset();
}

template <uint8_t EVENT_COUNT_MAX>
double AudioApparatus<EVENT_COUNT_MAX>::getAudioProcessorUsage()
{
  return AudioProcessorUsage();
}

template <uint8_t EVENT_COUNT_MAX>
double AudioApparatus<EVENT_COUNT_MAX>::getAudioProcessorUsageMax()
{
  return AudioProcessorUsageMax();
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::resetAudioProcessorUsageMax()
{
  AudioProcessorUsageMaxReset();
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::enableAudioCodec()
{
  pinMode(SDA,INPUT);
  pinMode(SCL,INPUT);
  if (digitalRead(SDA) && digitalRead(SCL))
  {
    // This may wait forever if the SDA & SCL pins lack
    // pullup resistors so check first
    codec_enabled_ = true;
    audio_apparatus::g_sgtl5000.enable();
  }
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::updatePlaying()
{
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case audio_apparatus::constants::RAW_TYPE:
      {
#if !defined(__IMXRT1062__)
        setPlaying(audio_apparatus::g_play_sd_raw.isPlaying());
#endif
        break;
      }
      case audio_apparatus::constants::WAV_TYPE:
      {
#if !defined(__IMXRT1062__)
        setPlaying(audio_apparatus::g_play_sd_wav.isPlaying());
#endif
        break;
      }
      case audio_apparatus::constants::TONE_TYPE:
      {
        break;
      }
      case audio_apparatus::constants::NOISE_TYPE:
      {
        break;
      }
    }
  }
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::startPwmHandler(int index)
{
  pulsing_ = true;
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::stopPwmHandler(int index)
{
  stop();
  indexed_pulses_.remove(index);
  pulsing_ = !indexed_pulses_.empty();
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::playToneHandler(int index)
{
  size_t frequency = indexed_pulses_[index].frequency;
  audio_apparatus::constants::speaker_t speaker = indexed_pulses_[index].speaker;
  long volume = indexed_pulses_[index].volume;
  playToneAt(frequency,speaker,volume);
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::playNoiseHandler(int index)
{
  audio_apparatus::constants::speaker_t speaker = indexed_pulses_[index].speaker;
  long volume = indexed_pulses_[index].volume;
  playNoiseAt(speaker,volume);
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::playFilteredNoiseHandler(int index)
{
  size_t frequency = indexed_pulses_[index].frequency;
  double bandwidth = indexed_pulses_[index].bandwidth;
  audio_apparatus::constants::speaker_t speaker = indexed_pulses_[index].speaker;
  long volume = indexed_pulses_[index].volume;
  playFilteredNoiseAt(frequency,bandwidth,speaker,volume);
}

template <uint8_t EVENT_COUNT_MAX>
void AudioApparatus<EVENT_COUNT_MAX>::stopHandler(int index)
{
  stop();
}

#endif

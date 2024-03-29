// ----------------------------------------------------------------------------
// AudioController.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../AudioController.h"


using namespace audio_controller;

void AudioController::setup()
{
  // Parent Setup
  ModularDeviceBase::setup();

  event_controller_.setup();
  audio_apparatus_.setup(event_controller_);

  // Reset Watchdog
  resetWatchdog();

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
    pins_);

  // Pins
#if !defined(__IMXRT1062__)
  modular_server::Pin & bnc_a_pin = modular_server_.pin(modular_device_base::constants::bnc_a_pin_name);
  bnc_a_pin.setModeDigitalOutput();
  bnc_a_pin.setValue(LOW);

  modular_server::Pin & bnc_b_pin = modular_server_.pin(modular_device_base::constants::bnc_b_pin_name);
  bnc_b_pin.setModeDigitalOutput();
  bnc_b_pin.setValue(LOW);
#endif

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
    properties_,
    parameters_,
    functions_,
    callbacks_);

  // Properties
  modular_server::Property & volume_property = modular_server_.createProperty(constants::volume_property_name,audio_apparatus::constants::volume_default);
  volume_property.setRange(constants::volume_min,constants::volume_max);
  volume_property.setUnits(constants::percent_units);
  volume_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::setVolumeHandler));

  modular_server::Property & stereo_speaker_gain_property = modular_server_.createProperty(constants::stereo_speaker_gain_property_name,audio_apparatus::constants::stereo_speaker_gain_default);
  stereo_speaker_gain_property.setRange(constants::gain_min,constants::gain_max);
  stereo_speaker_gain_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::setVolumeHandler));

#if !defined(__IMXRT1062__)
  modular_server::Property & pcb_speaker_gain_property = modular_server_.createProperty(constants::pcb_speaker_gain_property_name,audio_apparatus::constants::pcb_speaker_gain_default);
  pcb_speaker_gain_property.setRange(constants::gain_min,constants::gain_max);
  pcb_speaker_gain_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::setVolumeHandler));
#endif

  setVolumeHandler();

  modular_server::Property & playing_signal_property = modular_server_.createProperty(constants::playing_signal_property_name,constants::playing_signal_ptr_default);
  playing_signal_property.setSubset(constants::playing_signal_subset);

  // Parameters
  modular_server::Parameter & audio_path_parameter = modular_server_.createParameter(constants::audio_path_parameter_name);
  audio_path_parameter.setTypeString();

  modular_server::Parameter & frequency_parameter = modular_server_.createParameter(constants::frequency_parameter_name);
  frequency_parameter.setRange(constants::frequency_min,constants::frequency_max);
  frequency_parameter.setUnits(constants::hz_units);

  modular_server::Parameter & speaker_parameter = modular_server_.createParameter(constants::speaker_parameter_name);
  speaker_parameter.setTypeString();
  speaker_parameter.setSubset(constants::speaker_subset);

  modular_server::Parameter & delay_parameter = modular_server_.createParameter(constants::delay_parameter_name);
  delay_parameter.setRange(constants::delay_min,constants::delay_max);
  delay_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & period_parameter = modular_server_.createParameter(constants::period_parameter_name);
  period_parameter.setRange(constants::period_min,constants::period_max);
  period_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & on_duration_parameter = modular_server_.createParameter(constants::on_duration_parameter_name);
  on_duration_parameter.setRange(constants::on_duration_min,constants::on_duration_max);
  on_duration_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);

  modular_server::Parameter & pwm_index_parameter = modular_server_.createParameter(constants::pwm_index_parameter_name);
  pwm_index_parameter.setRange(0,audio_apparatus::constants::INDEXED_PULSES_COUNT_MAX-1);

  modular_server::Parameter & volume_parameter = modular_server_.createParameter(constants::volume_parameter_name);
  volume_parameter.setRange(constants::volume_min,constants::volume_max);
  volume_parameter.setUnits(constants::percent_units);

  modular_server::Parameter & bandwidth_parameter = modular_server_.createParameter(constants::bandwidth_parameter_name);
  bandwidth_parameter.setRange(constants::bandwidth_min,constants::bandwidth_max);
  bandwidth_parameter.setUnits(constants::octaves_units);

  // Functions
  modular_server::Function & get_audio_memory_usage_function = modular_server_.createFunction(constants::get_audio_memory_usage_function_name);
  get_audio_memory_usage_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getAudioMemoryUsageHandler));
  get_audio_memory_usage_function.setResultTypeLong();

  modular_server::Function & get_audio_memory_usage_max_function = modular_server_.createFunction(constants::get_audio_memory_usage_max_function_name);
  get_audio_memory_usage_max_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getAudioMemoryUsageMaxHandler));
  get_audio_memory_usage_max_function.setResultTypeLong();

  modular_server::Function & reset_audio_memory_usage_max_function = modular_server_.createFunction(constants::reset_audio_memory_usage_max_function_name);
  reset_audio_memory_usage_max_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::resetAudioMemoryUsageMaxHandler));

  modular_server::Function & get_audio_processor_usage_function = modular_server_.createFunction(constants::get_audio_processor_usage_function_name);
  get_audio_processor_usage_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getAudioProcessorUsageHandler));
  get_audio_processor_usage_function.setResultTypeDouble();

  modular_server::Function & get_audio_processor_usage_max_function = modular_server_.createFunction(constants::get_audio_processor_usage_max_function_name);
  get_audio_processor_usage_max_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getAudioProcessorUsageMaxHandler));
  get_audio_processor_usage_max_function.setResultTypeDouble();

  modular_server::Function & reset_audio_processor_usage_max_function = modular_server_.createFunction(constants::reset_audio_processor_usage_max_function_name);
  reset_audio_processor_usage_max_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::resetAudioProcessorUsageMaxHandler));

#if !defined(__IMXRT1062__)
  modular_server::Function & get_sd_card_info_function = modular_server_.createFunction(constants::get_sd_card_info_function_name);
  get_sd_card_info_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getSDCardInfoHandler));
  get_sd_card_info_function.setResultTypeObject();

  modular_server::Function & get_audio_paths_function = modular_server_.createFunction(constants::get_audio_paths_function_name);
  get_audio_paths_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getAudioPathsHandler));
  get_audio_paths_function.setResultTypeArray();
  get_audio_paths_function.setResultTypeString();

  modular_server::Function & play_path_function = modular_server_.createFunction(constants::play_path_function_name);
  play_path_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::playPathHandler));
  play_path_function.addParameter(audio_path_parameter);
#endif

  modular_server::Function & play_tone_function = modular_server_.createFunction(constants::play_tone_function_name);
  play_tone_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::playToneHandler));
  play_tone_function.addParameter(frequency_parameter);
  play_tone_function.addParameter(speaker_parameter);

  modular_server::Function & play_tone_at_function = modular_server_.createFunction(constants::play_tone_at_function_name);
  play_tone_at_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::playToneAtHandler));
  play_tone_at_function.addParameter(frequency_parameter);
  play_tone_at_function.addParameter(speaker_parameter);
  play_tone_at_function.addParameter(volume_parameter);

  modular_server::Function & play_noise_function = modular_server_.createFunction(constants::play_noise_function_name);
  play_noise_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::playNoiseHandler));
  play_noise_function.addParameter(speaker_parameter);

  modular_server::Function & play_noise_at_function = modular_server_.createFunction(constants::play_noise_at_function_name);
  play_noise_at_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::playNoiseAtHandler));
  play_noise_at_function.addParameter(speaker_parameter);
  play_noise_at_function.addParameter(volume_parameter);

  modular_server::Function & play_filtered_noise_function = modular_server_.createFunction(constants::play_filtered_noise_function_name);
  play_filtered_noise_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::playFilteredNoiseHandler));
  play_filtered_noise_function.addParameter(frequency_parameter);
  play_filtered_noise_function.addParameter(bandwidth_parameter);
  play_filtered_noise_function.addParameter(speaker_parameter);

  modular_server::Function & play_filtered_noise_at_function = modular_server_.createFunction(constants::play_filtered_noise_at_function_name);
  play_filtered_noise_at_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::playFilteredNoiseAtHandler));
  play_filtered_noise_at_function.addParameter(frequency_parameter);
  play_filtered_noise_at_function.addParameter(bandwidth_parameter);
  play_filtered_noise_at_function.addParameter(speaker_parameter);
  play_filtered_noise_at_function.addParameter(volume_parameter);

  modular_server::Function & stop_function = modular_server_.createFunction(constants::stop_function_name);
  stop_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::stopHandler));

  modular_server::Function & is_playing_function = modular_server_.createFunction(constants::is_playing_function_name);
  is_playing_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::isPlayingHandler));
  is_playing_function.setResultTypeBool();

  modular_server::Function & get_last_audio_path_played_function = modular_server_.createFunction(constants::get_last_audio_path_played_function_name);
  get_last_audio_path_played_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getLastAudioPathPlayedHandler));
  get_last_audio_path_played_function.setResultTypeString();

  modular_server::Function & get_position_function = modular_server_.createFunction(constants::get_position_function_name);
  get_position_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getPositionHandler));
  get_position_function.setResultTypeLong();

  modular_server::Function & get_length_function = modular_server_.createFunction(constants::get_length_function_name);
  get_length_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getLengthHandler));
  get_length_function.setResultTypeLong();

  modular_server::Function & get_percent_complete_function = modular_server_.createFunction(constants::get_percent_complete_function_name);
  get_percent_complete_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getPercentCompleteHandler));
  get_percent_complete_function.setResultTypeLong();

  modular_server::Function & add_tone_pwm_function = modular_server_.createFunction(constants::add_tone_pwm_function_name);
  add_tone_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::addTonePwmHandler));
  add_tone_pwm_function.addParameter(frequency_parameter);
  add_tone_pwm_function.addParameter(speaker_parameter);
  add_tone_pwm_function.addParameter(delay_parameter);
  add_tone_pwm_function.addParameter(period_parameter);
  add_tone_pwm_function.addParameter(on_duration_parameter);
  add_tone_pwm_function.addParameter(count_parameter);
  add_tone_pwm_function.setResultTypeLong();

  modular_server::Function & add_tone_pwm_at_function = modular_server_.createFunction(constants::add_tone_pwm_at_function_name);
  add_tone_pwm_at_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::addTonePwmAtHandler));
  add_tone_pwm_at_function.addParameter(frequency_parameter);
  add_tone_pwm_at_function.addParameter(speaker_parameter);
  add_tone_pwm_at_function.addParameter(volume_parameter);
  add_tone_pwm_at_function.addParameter(delay_parameter);
  add_tone_pwm_at_function.addParameter(period_parameter);
  add_tone_pwm_at_function.addParameter(on_duration_parameter);
  add_tone_pwm_at_function.addParameter(count_parameter);
  add_tone_pwm_at_function.setResultTypeLong();

  modular_server::Function & start_tone_pwm_function = modular_server_.createFunction(constants::start_tone_pwm_function_name);
  start_tone_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::startTonePwmHandler));
  start_tone_pwm_function.addParameter(frequency_parameter);
  start_tone_pwm_function.addParameter(speaker_parameter);
  start_tone_pwm_function.addParameter(delay_parameter);
  start_tone_pwm_function.addParameter(period_parameter);
  start_tone_pwm_function.addParameter(on_duration_parameter);
  start_tone_pwm_function.setResultTypeLong();

  modular_server::Function & start_tone_pwm_at_function = modular_server_.createFunction(constants::start_tone_pwm_at_function_name);
  start_tone_pwm_at_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::startTonePwmAtHandler));
  start_tone_pwm_at_function.addParameter(frequency_parameter);
  start_tone_pwm_at_function.addParameter(speaker_parameter);
  start_tone_pwm_at_function.addParameter(volume_parameter);
  start_tone_pwm_at_function.addParameter(delay_parameter);
  start_tone_pwm_at_function.addParameter(period_parameter);
  start_tone_pwm_at_function.addParameter(on_duration_parameter);
  start_tone_pwm_at_function.setResultTypeLong();

  modular_server::Function & add_noise_pwm_function = modular_server_.createFunction(constants::add_noise_pwm_function_name);
  add_noise_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::addNoisePwmHandler));
  add_noise_pwm_function.addParameter(speaker_parameter);
  add_noise_pwm_function.addParameter(delay_parameter);
  add_noise_pwm_function.addParameter(period_parameter);
  add_noise_pwm_function.addParameter(on_duration_parameter);
  add_noise_pwm_function.addParameter(count_parameter);
  add_noise_pwm_function.setResultTypeLong();

  modular_server::Function & add_noise_pwm_at_function = modular_server_.createFunction(constants::add_noise_pwm_at_function_name);
  add_noise_pwm_at_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::addNoisePwmAtHandler));
  add_noise_pwm_at_function.addParameter(speaker_parameter);
  add_noise_pwm_at_function.addParameter(volume_parameter);
  add_noise_pwm_at_function.addParameter(delay_parameter);
  add_noise_pwm_at_function.addParameter(period_parameter);
  add_noise_pwm_at_function.addParameter(on_duration_parameter);
  add_noise_pwm_at_function.addParameter(count_parameter);
  add_noise_pwm_at_function.setResultTypeLong();

  modular_server::Function & start_noise_pwm_function = modular_server_.createFunction(constants::start_noise_pwm_function_name);
  start_noise_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::startNoisePwmHandler));
  start_noise_pwm_function.addParameter(speaker_parameter);
  start_noise_pwm_function.addParameter(delay_parameter);
  start_noise_pwm_function.addParameter(period_parameter);
  start_noise_pwm_function.addParameter(on_duration_parameter);
  start_noise_pwm_function.setResultTypeLong();

  modular_server::Function & start_noise_pwm_at_function = modular_server_.createFunction(constants::start_noise_pwm_at_function_name);
  start_noise_pwm_at_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::startNoisePwmAtHandler));
  start_noise_pwm_at_function.addParameter(speaker_parameter);
  start_noise_pwm_at_function.addParameter(volume_parameter);
  start_noise_pwm_at_function.addParameter(delay_parameter);
  start_noise_pwm_at_function.addParameter(period_parameter);
  start_noise_pwm_at_function.addParameter(on_duration_parameter);
  start_noise_pwm_at_function.setResultTypeLong();

  modular_server::Function & add_filtered_noise_pwm_function = modular_server_.createFunction(constants::add_filtered_noise_pwm_function_name);
  add_filtered_noise_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::addFilteredNoisePwmHandler));
  add_filtered_noise_pwm_function.addParameter(frequency_parameter);
  add_filtered_noise_pwm_function.addParameter(bandwidth_parameter);
  add_filtered_noise_pwm_function.addParameter(speaker_parameter);
  add_filtered_noise_pwm_function.addParameter(delay_parameter);
  add_filtered_noise_pwm_function.addParameter(period_parameter);
  add_filtered_noise_pwm_function.addParameter(on_duration_parameter);
  add_filtered_noise_pwm_function.addParameter(count_parameter);
  add_filtered_noise_pwm_function.setResultTypeLong();

  modular_server::Function & add_filtered_noise_pwm_at_function = modular_server_.createFunction(constants::add_filtered_noise_pwm_at_function_name);
  add_filtered_noise_pwm_at_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::addFilteredNoisePwmAtHandler));
  add_filtered_noise_pwm_at_function.addParameter(frequency_parameter);
  add_filtered_noise_pwm_at_function.addParameter(bandwidth_parameter);
  add_filtered_noise_pwm_at_function.addParameter(speaker_parameter);
  add_filtered_noise_pwm_at_function.addParameter(volume_parameter);
  add_filtered_noise_pwm_at_function.addParameter(delay_parameter);
  add_filtered_noise_pwm_at_function.addParameter(period_parameter);
  add_filtered_noise_pwm_at_function.addParameter(on_duration_parameter);
  add_filtered_noise_pwm_at_function.addParameter(count_parameter);
  add_filtered_noise_pwm_at_function.setResultTypeLong();

  modular_server::Function & start_filtered_noise_pwm_function = modular_server_.createFunction(constants::start_filtered_noise_pwm_function_name);
  start_filtered_noise_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::startFilteredNoisePwmHandler));
  start_filtered_noise_pwm_function.addParameter(frequency_parameter);
  start_filtered_noise_pwm_function.addParameter(bandwidth_parameter);
  start_filtered_noise_pwm_function.addParameter(speaker_parameter);
  start_filtered_noise_pwm_function.addParameter(delay_parameter);
  start_filtered_noise_pwm_function.addParameter(period_parameter);
  start_filtered_noise_pwm_function.addParameter(on_duration_parameter);
  start_filtered_noise_pwm_function.setResultTypeLong();

  modular_server::Function & start_filtered_noise_pwm_at_function = modular_server_.createFunction(constants::start_filtered_noise_pwm_at_function_name);
  start_filtered_noise_pwm_at_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::startFilteredNoisePwmAtHandler));
  start_filtered_noise_pwm_at_function.addParameter(frequency_parameter);
  start_filtered_noise_pwm_at_function.addParameter(bandwidth_parameter);
  start_filtered_noise_pwm_at_function.addParameter(speaker_parameter);
  start_filtered_noise_pwm_at_function.addParameter(volume_parameter);
  start_filtered_noise_pwm_at_function.addParameter(delay_parameter);
  start_filtered_noise_pwm_at_function.addParameter(period_parameter);
  start_filtered_noise_pwm_at_function.addParameter(on_duration_parameter);
  start_filtered_noise_pwm_at_function.setResultTypeLong();

  modular_server::Function & stop_pwm_function = modular_server_.createFunction(constants::stop_pwm_function_name);
  stop_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::stopPwmHandler));
  stop_pwm_function.addParameter(pwm_index_parameter);

  modular_server::Function & stop_all_pwm_function = modular_server_.createFunction(constants::stop_all_pwm_function_name);
  stop_all_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::stopAllPwmHandler));

  modular_server::Function & is_pulsing_function = modular_server_.createFunction(constants::is_pulsing_function_name);
  is_pulsing_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::isPulsingHandler));
  is_pulsing_function.setResultTypeBool();

  // Callbacks

}

void AudioController::playTone(size_t frequency,
  const ConstantString * const speaker_ptr)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  audio_apparatus_.playToneAt(frequency,speakerPtrToSpeaker(speaker_ptr),volume);
}

void AudioController::playNoise(const ConstantString * const speaker_ptr)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  audio_apparatus_.playNoiseAt(speakerPtrToSpeaker(speaker_ptr),volume);
}

void AudioController::playFilteredNoise(size_t frequency,
  double bandwidth,
  const ConstantString * const speaker_ptr)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  audio_apparatus_.playFilteredNoiseAt(frequency,bandwidth,speakerPtrToSpeaker(speaker_ptr),volume);
}

int AudioController::addTonePwm(size_t frequency,
  const ConstantString * const speaker_ptr,
  long delay,
  long period,
  long on_duration,
  long count)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  return audio_apparatus_.addTonePwmAt(frequency,speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration,count);
}

int AudioController::startTonePwm(size_t frequency,
  const ConstantString * const speaker_ptr,
  long delay,
  long period,
  long on_duration)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  return audio_apparatus_.startTonePwmAt(frequency,speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration);
}

int AudioController::addNoisePwm(const ConstantString * const speaker_ptr,
  long delay,
  long period,
  long on_duration,
  long count)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  return audio_apparatus_.addNoisePwmAt(speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration,count);
}

int AudioController::startNoisePwm(const ConstantString * const speaker_ptr,
  long delay,
  long period,
  long on_duration)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  return audio_apparatus_.startNoisePwmAt(speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration);
}

int AudioController::addFilteredNoisePwm(size_t frequency,
  double bandwidth,
  const ConstantString * const speaker_ptr,
  long delay,
  long period,
  long on_duration,
  long count)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  return audio_apparatus_.addFilteredNoisePwmAt(frequency,bandwidth,speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration,count);
}

int AudioController::startFilteredNoisePwm(size_t frequency,
  double bandwidth,
  const ConstantString * const speaker_ptr,
  long delay,
  long period,
  long on_duration)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  return audio_apparatus_.startFilteredNoisePwmAt(frequency,bandwidth,speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration);
}

audio_apparatus::constants::speaker_t AudioController::speakerPtrToSpeaker(const ConstantString * const speaker_ptr)
{
  audio_apparatus::constants::speaker_t speaker = audio_apparatus::constants::SPEAKER_ALL;
  if (speaker_ptr == &constants::speaker_left)
  {
    speaker = audio_apparatus::constants::SPEAKER_LEFT;
  }
  else if (speaker_ptr == &constants::speaker_right)
  {
    speaker = audio_apparatus::constants::SPEAKER_RIGHT;
  }
  return speaker;
}

void AudioController::addDirectoryToResponse(File dir,
  const char * pwd)
{
  while (true)
  {
    File entry =  dir.openNextFile();
    if (!entry)
    {
      // no more files
      break;
    }
    char full_path[audio_apparatus::constants::STRING_LENGTH_PATH];
    full_path[0] = 0;
    strcat(full_path,pwd);
    strcat(full_path,entry.name());
    if (!entry.isDirectory())
    {
      bool audio_file = false;
      for (unsigned int i=0;i<audio_apparatus::constants::AUDIO_EXT_COUNT;++i)
      {
        if (strstr(full_path,audio_apparatus::constants::audio_exts[i]) != NULL)
        {
          audio_file = true;
        }
      }
      if (audio_file)
      {
        modular_server_.response().write(full_path);
      }
    }
    else
    {
      strcat(full_path,"/");
      addDirectoryToResponse(entry,full_path);
    }
    entry.close();
  }
}

void AudioController::setVolume(long volume)
{
  double stereo_speaker_gain;
  modular_server_.property(constants::stereo_speaker_gain_property_name).getValue(stereo_speaker_gain);

  double pcb_speaker_gain = audio_apparatus::constants::pcb_speaker_gain_default;

#if !defined(__IMXRT1062__)
  modular_server_.property(constants::pcb_speaker_gain_property_name).getValue(pcb_speaker_gain);
#endif

  audio_apparatus_.setVolume(volume,stereo_speaker_gain,pcb_speaker_gain);
}

void AudioController::setPlaying(bool playing)
{
  audio_apparatus_.setPlaying(playing);

  modular_server::Pin & bnc_a_pin = modular_server_.pin(modular_device_base::constants::bnc_a_pin_name);
  modular_server::Pin & bnc_b_pin = modular_server_.pin(modular_device_base::constants::bnc_b_pin_name);

  const ConstantString * playing_signal_ptr;
  modular_server_.property(constants::playing_signal_property_name).getValue(playing_signal_ptr);

  if (playing)
  {
    if (playing_signal_ptr == &constants::playing_signal_both)
    {
      bnc_a_pin.setValue(HIGH);
      bnc_b_pin.setValue(HIGH);
    }
    if (playing_signal_ptr == &constants::playing_signal_bnc_b)
    {
      bnc_b_pin.setValue(HIGH);
    }
    else
    {
      bnc_a_pin.setValue(HIGH);
    }
  }
  else
  {
    bnc_a_pin.setValue(LOW);
    bnc_b_pin.setValue(LOW);
  }
}

void AudioController::setVolumeHandler()
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);

  setVolume(volume);
}

void AudioController::getAudioMemoryUsageHandler()
{
  modular_server_.response().returnResult(audio_apparatus_.getAudioMemoryUsage());
}

void AudioController::getAudioMemoryUsageMaxHandler()
{
  modular_server_.response().returnResult(audio_apparatus_.getAudioMemoryUsageMax());
}

void AudioController::resetAudioMemoryUsageMaxHandler()
{
  AudioMemoryUsageMaxReset();
}

void AudioController::getAudioProcessorUsageHandler()
{
  modular_server_.response().returnResult(AudioProcessorUsage());
}

void AudioController::getAudioProcessorUsageMaxHandler()
{
  modular_server_.response().returnResult(AudioProcessorUsage());
}

void AudioController::resetAudioProcessorUsageMaxHandler()
{
  AudioProcessorUsageMaxReset();
}

void AudioController::getSDCardInfoHandler()
{
  modular_server::Response & response = modular_server_.response();
  SDInterface & sd_interface = audio_apparatus_.getSDInterface();
  response.writeResultKey();
  response.beginObject();
  response.write("detected",sd_interface.getDetected());
  response.write("type",sd_interface.getType());
  response.write("formatted",sd_interface.getFormatted());
  response.write("format",sd_interface.getFormat());
  response.write("volume_size",sd_interface.getVolumeSize());
  response.write("initialized",sd_interface.getInitialized());
  response.endObject();
}

void AudioController::getAudioPathsHandler()
{
  modular_server::Response & response = modular_server_.response();
  response.writeResultKey();
  response.beginArray();
#if !defined(__IMXRT1062__)
  File root = SD.open("/");
  addDirectoryToResponse(root,constants::sd_prefix);
#endif
  response.endArray();
}

void AudioController::playPathHandler()
{
  modular_server::Response & response = modular_server_.response();
  if (!audio_apparatus_.codecEnabled())
  {
    response.returnError("No audio codec chip detected.");
    return;
  }
  const char * path;
  modular_server_.parameter(constants::audio_path_parameter_name).getValue(path);
  if (!audio_apparatus_.pathIsAudio(path))
  {
    char error_str[constants::STRING_LENGTH_ERROR_MESSAGE];
    error_str[0] = 0;
    strcat(error_str,"Invalid audio path: ");
    strcat(error_str,path);
    response.returnError(error_str);
    return;
  }
  bool playing = audio_apparatus_.playPath(path);
  if (!playing)
  {
    char error_str[constants::STRING_LENGTH_ERROR_MESSAGE];
    error_str[0] = 0;
    strcat(error_str,"Unable to find audio path: ");
    strcat(error_str,path);
    response.returnError(error_str);
  }
}

void AudioController::playToneHandler()
{
  modular_server::Response & response = modular_server_.response();
  if (!audio_apparatus_.codecEnabled())
  {
    response.returnError("No audio codec chip detected.");
    return;
  }
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  playTone(frequency,speaker_ptr);
}

void AudioController::playToneAtHandler()
{
  modular_server::Response & response = modular_server_.response();
  if (!audio_apparatus_.codecEnabled())
  {
    response.returnError("No audio codec chip detected.");
    return;
  }
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long volume;
  modular_server_.parameter(constants::volume_parameter_name).getValue(volume);
  audio_apparatus_.playToneAt(frequency,speakerPtrToSpeaker(speaker_ptr),volume);
}

void AudioController::playNoiseHandler()
{
  modular_server::Response & response = modular_server_.response();
  if (!audio_apparatus_.codecEnabled())
  {
    response.returnError("No audio codec chip detected.");
    return;
  }
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  playNoise(speaker_ptr);
}

void AudioController::playNoiseAtHandler()
{
  modular_server::Response & response = modular_server_.response();
  if (!audio_apparatus_.codecEnabled())
  {
    response.returnError("No audio codec chip detected.");
    return;
  }
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long volume;
  modular_server_.parameter(constants::volume_parameter_name).getValue(volume);
  audio_apparatus_.playNoiseAt(speakerPtrToSpeaker(speaker_ptr),volume);
}

void AudioController::playFilteredNoiseHandler()
{
  modular_server::Response & response = modular_server_.response();
  if (!audio_apparatus_.codecEnabled())
  {
    response.returnError("No audio codec chip detected.");
    return;
  }
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  double bandwidth;
  modular_server_.parameter(constants::bandwidth_parameter_name).getValue(bandwidth);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  playFilteredNoise(frequency,bandwidth,speaker_ptr);
}

void AudioController::playFilteredNoiseAtHandler()
{
  modular_server::Response & response = modular_server_.response();
  if (!audio_apparatus_.codecEnabled())
  {
    response.returnError("No audio codec chip detected.");
    return;
  }
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  double bandwidth;
  modular_server_.parameter(constants::bandwidth_parameter_name).getValue(bandwidth);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long volume;
  modular_server_.parameter(constants::volume_parameter_name).getValue(volume);
  audio_apparatus_.playFilteredNoiseAt(frequency,bandwidth,speakerPtrToSpeaker(speaker_ptr),volume);
}

void AudioController::stopHandler()
{
  audio_apparatus_.stop();
}

void AudioController::isPlayingHandler()
{
  bool playing = audio_apparatus_.isPlaying();
  modular_server_.response().returnResult(playing);
}

void AudioController::getLastAudioPathPlayedHandler()
{
  const char * last_path_played = audio_apparatus_.getLastAudioPathPlayed();
  modular_server_.response().returnResult(last_path_played);
}

void AudioController::getPositionHandler()
{
  long position = audio_apparatus_.getPosition();
  modular_server_.response().returnResult(position);
}

void AudioController::getLengthHandler()
{
  long length = audio_apparatus_.getLength();
  modular_server_.response().returnResult(length);
}

void AudioController::getPercentCompleteHandler()
{
  long percent_complete = audio_apparatus_.getPercentComplete();
  modular_server_.response().returnResult(percent_complete);
}

void AudioController::addTonePwmHandler()
{
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  int index = addTonePwm(frequency,speaker_ptr,delay,period,on_duration,count);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::addTonePwmAtHandler()
{
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long volume;
  modular_server_.parameter(constants::volume_parameter_name).getValue(volume);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  int index = audio_apparatus_.addTonePwmAt(frequency,speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration,count);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::startTonePwmHandler()
{
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  int index = startTonePwm(frequency,speaker_ptr,delay,period,on_duration);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::startTonePwmAtHandler()
{
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long volume;
  modular_server_.parameter(constants::volume_parameter_name).getValue(volume);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  int index = audio_apparatus_.startTonePwmAt(frequency,speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::addNoisePwmHandler()
{
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  int index = addNoisePwm(speaker_ptr,delay,period,on_duration,count);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::addNoisePwmAtHandler()
{
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long volume;
  modular_server_.parameter(constants::volume_parameter_name).getValue(volume);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  int index = audio_apparatus_.addNoisePwmAt(speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration,count);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::startNoisePwmHandler()
{
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  int index = startNoisePwm(speaker_ptr,delay,period,on_duration);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::startNoisePwmAtHandler()
{
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long volume;
  modular_server_.parameter(constants::volume_parameter_name).getValue(volume);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  int index = audio_apparatus_.startNoisePwmAt(speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::addFilteredNoisePwmHandler()
{
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  double bandwidth;
  modular_server_.parameter(constants::bandwidth_parameter_name).getValue(bandwidth);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  int index = addFilteredNoisePwm(frequency,bandwidth,speaker_ptr,delay,period,on_duration,count);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::addFilteredNoisePwmAtHandler()
{
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  double bandwidth;
  modular_server_.parameter(constants::bandwidth_parameter_name).getValue(bandwidth);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long volume;
  modular_server_.parameter(constants::volume_parameter_name).getValue(volume);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  int index = audio_apparatus_.addFilteredNoisePwmAt(frequency,bandwidth,speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration,count);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::startFilteredNoisePwmHandler()
{
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  double bandwidth;
  modular_server_.parameter(constants::bandwidth_parameter_name).getValue(bandwidth);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  int index = startFilteredNoisePwm(frequency,bandwidth,speaker_ptr,delay,period,on_duration);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::startFilteredNoisePwmAtHandler()
{
  long frequency;
  modular_server_.parameter(constants::frequency_parameter_name).getValue(frequency);
  double bandwidth;
  modular_server_.parameter(constants::bandwidth_parameter_name).getValue(bandwidth);
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long volume;
  modular_server_.parameter(constants::volume_parameter_name).getValue(volume);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  int index = audio_apparatus_.startFilteredNoisePwmAt(frequency,bandwidth,speakerPtrToSpeaker(speaker_ptr),volume,delay,period,on_duration);
  if (index >= 0)
  {
    modular_server_.response().returnResult(index);
  }
  else
  {
    modular_server_.response().returnError(constants::pwm_error);
  }
}

void AudioController::stopPwmHandler()
{
  int pwm_index;
  modular_server_.parameter(constants::pwm_index_parameter_name).getValue(pwm_index);
  audio_apparatus_.stopPwm(pwm_index);
}

void AudioController::stopAllPwmHandler()
{
  audio_apparatus_.stopAllPwm();
}

void AudioController::isPulsingHandler()
{
  bool pulsing = audio_apparatus_.isPulsing();
  modular_server_.response().returnResult(pulsing);
}

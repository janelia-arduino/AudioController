// ----------------------------------------------------------------------------
// AudioController.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../AudioController.h"


namespace audio_controller
{
// GUItool: begin automatically generated code
AudioSynthNoiseWhite     g_noise_left;   //xy=85,251
AudioSynthNoiseWhite     g_noise_right;  //xy=89,288
AudioSynthWaveformSine   g_tone_left;    //xy=250,176
AudioSynthWaveformSine   g_tone_right;   //xy=256,215
AudioPlaySdWav           g_play_sd_wav;  //xy=263,102
AudioFilterBiquad        g_biquad_left;  //xy=263,251
AudioPlaySdRaw           g_play_sd_raw;  //xy=264,139
AudioFilterBiquad        g_biquad_right; //xy=265,289
AudioMixer4              g_mixer_left;   //xy=500,111
AudioMixer4              g_mixer_right;  //xy=506,187
AudioMixer4              g_mixer_dac;    //xy=702,188
AudioOutputI2S           g_stereo_speaker; //xy=717,126
AudioOutputAnalog        g_pcb_speaker;  //xy=890,188
AudioConnection          patchCord1(g_noise_left, g_biquad_left);
AudioConnection          patchCord2(g_noise_right, g_biquad_right);
AudioConnection          patchCord3(g_tone_left, 0, g_mixer_left, 2);
AudioConnection          patchCord4(g_tone_right, 0, g_mixer_right, 2);
AudioConnection          patchCord5(g_play_sd_wav, 0, g_mixer_left, 0);
AudioConnection          patchCord6(g_play_sd_wav, 1, g_mixer_right, 0);
AudioConnection          patchCord7(g_biquad_left, 0, g_mixer_left, 3);
AudioConnection          patchCord8(g_play_sd_raw, 0, g_mixer_left, 1);
AudioConnection          patchCord9(g_play_sd_raw, 0, g_mixer_right, 1);
AudioConnection          patchCord10(g_biquad_right, 0, g_mixer_right, 3);
AudioConnection          patchCord11(g_mixer_left, 0, g_stereo_speaker, 0);
AudioConnection          patchCord12(g_mixer_left, 0, g_mixer_dac, 0);
AudioConnection          patchCord13(g_mixer_right, 0, g_stereo_speaker, 1);
AudioConnection          patchCord14(g_mixer_right, 0, g_mixer_dac, 1);
AudioConnection          patchCord15(g_mixer_dac, g_pcb_speaker);
AudioControlSGTL5000     g_sgtl5000;     //xy=498,36
// GUItool: end automatically generated code
}

using namespace audio_controller;

AudioController::AudioController()
{
  codec_enabled_ = false;
  playing_ = false;
  path_played_[0] = 0;
  pulsing_ = false;
}

void AudioController::setup()
{
  // Parent Setup
  ModularDeviceBase::setup();

  // Reset Watchdog
  resetWatchdog();

  // Audio Setup
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(constants::AUDIO_MEMORY_BLOCK_COUNT);
  AudioMemoryUsageMaxReset();

  enableAudioCodec();

  g_pcb_speaker.analogReference(constants::pcb_speaker_reference);

  // Setup SD Card
  sd_interface_.setup();

  // Event Controller Setup
  event_controller_.setup();

  // Pin Setup
  pinMode(constants::pcb_speaker_enable_pin,OUTPUT);
  digitalWrite(constants::pcb_speaker_enable_pin,HIGH);

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
    pins_);

  // Pins
  modular_server::Pin & bnc_a_pin = modular_server_.pin(modular_device_base::constants::bnc_a_pin_name);
  bnc_a_pin.setModeDigitalOutput();
  bnc_a_pin.setValue(LOW);

  modular_server::Pin & bnc_b_pin = modular_server_.pin(modular_device_base::constants::bnc_b_pin_name);
  bnc_b_pin.setModeDigitalOutput();
  bnc_b_pin.setValue(LOW);

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
    properties_,
    parameters_,
    functions_,
    callbacks_);

  // Properties
  modular_server::Property & volume_property = modular_server_.createProperty(constants::volume_property_name,constants::volume_default);
  volume_property.setRange(constants::volume_min,constants::volume_max);
  volume_property.setUnits(constants::percent_units);
  volume_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::setVolumeHandler));

  modular_server::Property & stereo_speaker_gain_property = modular_server_.createProperty(constants::stereo_speaker_gain_property_name,constants::stereo_speaker_gain_default);
  stereo_speaker_gain_property.setRange(constants::gain_min,constants::gain_max);
  stereo_speaker_gain_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::setVolumeHandler));

  modular_server::Property & pcb_speaker_gain_property = modular_server_.createProperty(constants::pcb_speaker_gain_property_name,constants::pcb_speaker_gain_default);
  pcb_speaker_gain_property.setRange(constants::gain_min,constants::gain_max);
  pcb_speaker_gain_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::setVolumeHandler));

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
  pwm_index_parameter.setRange(0,constants::INDEXED_PULSES_COUNT_MAX-1);

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

bool AudioController::playPath(const char * path)
{
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
  bool playing = false;

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
  return playing;
}

void AudioController::playTone(size_t frequency,
  const ConstantString * const speaker_ptr)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  playToneAt(frequency,speaker_ptr,volume);
}

void AudioController::playToneAt(size_t frequency,
  const ConstantString * const speaker_ptr,
  long volume)
{
  stop();
  if ((volume <= 0) || (frequency < constants::frequency_stop_threshold))
  {
    return;
  }
  audio_type_playing_ = constants::TONE_TYPE;
  if ((speaker_ptr == &constants::speaker_all) || (speaker_ptr == &constants::speaker_left))
  {
    g_tone_left.amplitude(0);
    g_tone_left.frequency(frequency);
    g_tone_left.amplitude(1);
  }
  if ((speaker_ptr == &constants::speaker_all) || (speaker_ptr == &constants::speaker_right))
  {
    g_tone_right.amplitude(0);
    g_tone_right.frequency(frequency);
    g_tone_right.amplitude(1);
  }
  setVolume(volume);
  setPlaying(true);

}

void AudioController::playNoise(const ConstantString * const speaker_ptr)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  playNoiseAt(speaker_ptr,volume);
}

void AudioController::playNoiseAt(const ConstantString * const speaker_ptr,
  long volume)
{
  stop();
  if (volume <= 0)
  {
    return;
  }
  audio_type_playing_ = constants::NOISE_TYPE;
  if ((speaker_ptr == &constants::speaker_all) || (speaker_ptr == &constants::speaker_left))
  {
    g_biquad_left.setCoefficients(constants::FILTER_STAGE_0,constants::allpass_filter_coefs);
    g_noise_left.amplitude(1);
  }
  if ((speaker_ptr == &constants::speaker_all) || (speaker_ptr == &constants::speaker_right))
  {
    g_biquad_right.setCoefficients(constants::FILTER_STAGE_0,constants::allpass_filter_coefs);
    g_noise_right.amplitude(1);
  }
  setVolume(volume);
  setPlaying(true);

}

void AudioController::playFilteredNoise(size_t frequency,
  double bandwidth,
  const ConstantString * const speaker_ptr)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  playFilteredNoiseAt(frequency,bandwidth,speaker_ptr,volume);
}

void AudioController::playFilteredNoiseAt(size_t frequency,
  double bandwidth,
  const ConstantString * const speaker_ptr,
  long volume)
{
  stop();
  if ((volume <= 0) || (frequency < constants::frequency_stop_threshold))
  {
    return;
  }
  audio_type_playing_ = constants::NOISE_TYPE;
  if ((speaker_ptr == &constants::speaker_all) || (speaker_ptr == &constants::speaker_left))
  {
    g_biquad_left.setBandpass(constants::FILTER_STAGE_0,frequency,bandwidth);
    g_noise_left.amplitude(1);
  }
  if ((speaker_ptr == &constants::speaker_all) || (speaker_ptr == &constants::speaker_right))
  {
    g_biquad_right.setBandpass(constants::FILTER_STAGE_0,frequency,bandwidth);
    g_noise_right.amplitude(1);
  }
  setVolume(volume);
  setPlaying(true);
}

void AudioController::stop()
{
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case constants::RAW_TYPE:
      {
        g_play_sd_raw.stop();
        break;
      }
      case constants::WAV_TYPE:
      {
        g_play_sd_wav.stop();
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

bool AudioController::isPlaying()
{
  return playing_;
}

const char * AudioController::getLastAudioPathPlayed()
{
  return path_played_;
}

long AudioController::getPosition()
{
  long position = 0;
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case constants::RAW_TYPE:
      {
        position = g_play_sd_raw.positionMillis();
        break;
      }
      case constants::WAV_TYPE:
      {
        position = g_play_sd_wav.positionMillis();
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

long AudioController::getLength()
{
  long length = 0;
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case constants::RAW_TYPE:
      {
        length = g_play_sd_raw.lengthMillis();
        break;
      }
      case constants::WAV_TYPE:
      {
        length = g_play_sd_wav.lengthMillis();
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

int AudioController::getPercentComplete()
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

bool AudioController::codecEnabled()
{
  return codec_enabled_;
}

bool AudioController::pathIsAudio(const char * path)
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

int AudioController::addTonePwm(size_t frequency,
  const ConstantString * const speaker_ptr,
  long delay,
  long period,
  long on_duration,
  long count)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  return addTonePwmAt(frequency,speaker_ptr,volume,delay,period,on_duration,count);
}

int AudioController::addTonePwmAt(size_t frequency,
  const ConstantString * const speaker_ptr,
  long volume,
  long delay,
  long period,
  long on_duration,
  long count)
{
  if (indexed_pulses_.full())
  {
    return constants::bad_index;
  }
  audio_controller::constants::PulseInfo pulse_info;
  pulse_info.frequency = frequency;
  pulse_info.speaker_ptr = speaker_ptr;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioController::playToneHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioController::stopHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int AudioController::startTonePwm(size_t frequency,
  const ConstantString * const speaker_ptr,
  long delay,
  long period,
  long on_duration)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  return startTonePwmAt(frequency,speaker_ptr,volume,delay,period,on_duration);
}

int AudioController::startTonePwmAt(size_t frequency,
  const ConstantString * const speaker_ptr,
  long volume,
  long delay,
  long period,
  long on_duration)
{
  if (indexed_pulses_.full())
  {
    return -1;
  }
  audio_controller::constants::PulseInfo pulse_info;
  pulse_info.frequency = frequency;
  pulse_info.speaker_ptr = speaker_ptr;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioController::playToneHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioController::stopHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int AudioController::addNoisePwm(const ConstantString * const speaker_ptr,
  long delay,
  long period,
  long on_duration,
  long count)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  return addNoisePwmAt(speaker_ptr,volume,delay,period,on_duration,count);
}

int AudioController::addNoisePwmAt(const ConstantString * const speaker_ptr,
  long volume,
  long delay,
  long period,
  long on_duration,
  long count)
{
  if (indexed_pulses_.full())
  {
    return constants::bad_index;
  }
  audio_controller::constants::PulseInfo pulse_info;
  pulse_info.speaker_ptr = speaker_ptr;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioController::playNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioController::stopHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

int AudioController::startNoisePwm(const ConstantString * const speaker_ptr,
  long delay,
  long period,
  long on_duration)
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);
  return startNoisePwmAt(speaker_ptr,volume,delay,period,on_duration);
}

int AudioController::startNoisePwmAt(const ConstantString * const speaker_ptr,
  long volume,
  long delay,
  long period,
  long on_duration)
{
  if (indexed_pulses_.full())
  {
    return -1;
  }
  audio_controller::constants::PulseInfo pulse_info;
  pulse_info.speaker_ptr = speaker_ptr;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioController::playNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioController::stopHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
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
  return addFilteredNoisePwmAt(frequency,bandwidth,speaker_ptr,volume,delay,period,on_duration,count);
}

int AudioController::addFilteredNoisePwmAt(size_t frequency,
  double bandwidth,
  const ConstantString * const speaker_ptr,
  long volume,
  long delay,
  long period,
  long on_duration,
  long count)
{
  if (indexed_pulses_.full())
  {
    return constants::bad_index;
  }
  audio_controller::constants::PulseInfo pulse_info;
  pulse_info.frequency = frequency;
  pulse_info.bandwidth = bandwidth;
  pulse_info.speaker_ptr = speaker_ptr;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioController::playFilteredNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioController::stopHandler),
    delay,
    period,
    on_duration,
    count,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
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
  return startFilteredNoisePwmAt(frequency,bandwidth,speaker_ptr,volume,delay,period,on_duration);
}

int AudioController::startFilteredNoisePwmAt(size_t frequency,
  double bandwidth,
  const ConstantString * const speaker_ptr,
  long volume,
  long delay,
  long period,
  long on_duration)
{
  if (indexed_pulses_.full())
  {
    return -1;
  }
  audio_controller::constants::PulseInfo pulse_info;
  pulse_info.frequency = frequency;
  pulse_info.bandwidth = bandwidth;
  pulse_info.speaker_ptr = speaker_ptr;
  pulse_info.volume = volume;
  int index = indexed_pulses_.add(pulse_info);
  EventIdPair event_id_pair = event_controller_.addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&AudioController::playFilteredNoiseHandler),
    makeFunctor((Functor1<int> *)0,*this,&AudioController::stopHandler),
    delay,
    period,
    on_duration,
    index);
  event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::startPwmHandler));
  event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&AudioController::stopPwmHandler));
  indexed_pulses_[index].event_id_pair = event_id_pair;
  event_controller_.enable(event_id_pair);
  return index;
}

void AudioController::stopPwm(int pwm_index)
{
  if (pwm_index < 0)
  {
    return;
  }
  if (indexed_pulses_.indexHasValue(pwm_index))
  {
    constants::PulseInfo pulse_info = indexed_pulses_[pwm_index];
    event_controller_.remove(pulse_info.event_id_pair);
  }
}

void AudioController::stopAllPwm()
{
  for (size_t i=0; i<constants::INDEXED_PULSES_COUNT_MAX; ++i)
  {
    stopPwm(i);
  }
}

bool AudioController::isPulsing()
{
  return pulsing_;
}

void AudioController::enableAudioCodec()
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

void AudioController::updatePlaying()
{
  if (playing_)
  {
    switch (audio_type_playing_)
    {
      case constants::RAW_TYPE:
      {
        setPlaying(g_play_sd_raw.isPlaying());
        break;
      }
      case constants::WAV_TYPE:
      {
        setPlaying(g_play_sd_wav.isPlaying());
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
    char full_path[constants::STRING_LENGTH_PATH];
    full_path[0] = 0;
    strcat(full_path,pwd);
    strcat(full_path,entry.name());
    if (!entry.isDirectory())
    {
      bool audio_file = false;
      for (unsigned int i=0;i<constants::AUDIO_EXT_COUNT;++i)
      {
        if (strstr(full_path,constants::audio_exts[i]) != NULL)
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

  if (codecEnabled())
  {
    g_sgtl5000.volume(((double)volume*stereo_speaker_gain)/100.0);
  }

  double pcb_speaker_gain;
  modular_server_.property(constants::pcb_speaker_gain_property_name).getValue(pcb_speaker_gain);

  double pcb_speaker_total_gain = (constants::pcb_speaker_channel_gain*constants::pcb_speaker_pre_gain*(double)volume*pcb_speaker_gain)/100.0;
  g_mixer_dac.gain(0,pcb_speaker_total_gain);
  g_mixer_dac.gain(1,pcb_speaker_total_gain);
}

void AudioController::setPlaying(bool playing)
{
  playing_ = playing;

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

void AudioController::startPwmHandler(int index)
{
  pulsing_ = true;
}

void AudioController::stopPwmHandler(int index)
{
  stop();
  indexed_pulses_.remove(index);
  pulsing_ = !indexed_pulses_.empty();
}

void AudioController::setVolumeHandler()
{
  long volume;
  modular_server_.property(constants::volume_property_name).getValue(volume);

  setVolume(volume);
}

void AudioController::getAudioMemoryUsageHandler()
{
  modular_server_.response().returnResult(AudioMemoryUsage());
}

void AudioController::getAudioMemoryUsageMaxHandler()
{
  modular_server_.response().returnResult(AudioMemoryUsage());
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
  response.writeResultKey();
  response.beginObject();
  response.write("detected",sd_interface_.getDetected());
  response.write("type",sd_interface_.getType());
  response.write("formatted",sd_interface_.getFormatted());
  response.write("format",sd_interface_.getFormat());
  response.write("volume_size",sd_interface_.getVolumeSize());
  response.write("initialized",sd_interface_.getInitialized());
  response.endObject();
}

void AudioController::getAudioPathsHandler()
{
  File root = SD.open("/");
  modular_server::Response & response = modular_server_.response();
  response.writeResultKey();
  response.beginArray();
  addDirectoryToResponse(root,constants::sd_prefix);
  response.endArray();
}

void AudioController::playPathHandler()
{
  modular_server::Response & response = modular_server_.response();
  if (!codecEnabled())
  {
    response.returnError("No audio codec chip detected.");
    return;
  }
  const char * path;
  modular_server_.parameter(constants::audio_path_parameter_name).getValue(path);
  if (!pathIsAudio(path))
  {
    char error_str[constants::STRING_LENGTH_ERROR_MESSAGE];
    error_str[0] = 0;
    strcat(error_str,"Invalid audio path: ");
    strcat(error_str,path);
    response.returnError(error_str);
    return;
  }
  bool playing = playPath(path);
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
  if (!codecEnabled())
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
  if (!codecEnabled())
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
  playToneAt(frequency,speaker_ptr,volume);
}

void AudioController::playNoiseHandler()
{
  modular_server::Response & response = modular_server_.response();
  if (!codecEnabled())
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
  if (!codecEnabled())
  {
    response.returnError("No audio codec chip detected.");
    return;
  }
  const ConstantString * speaker_ptr;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_ptr);
  long volume;
  modular_server_.parameter(constants::volume_parameter_name).getValue(volume);
  playNoiseAt(speaker_ptr,volume);
}

void AudioController::playFilteredNoiseHandler()
{
  modular_server::Response & response = modular_server_.response();
  if (!codecEnabled())
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
  if (!codecEnabled())
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
  playFilteredNoiseAt(frequency,bandwidth,speaker_ptr,volume);
}

void AudioController::stopHandler()
{
  stop();
}

void AudioController::isPlayingHandler()
{
  bool playing = isPlaying();
  modular_server_.response().returnResult(playing);
}

void AudioController::getLastAudioPathPlayedHandler()
{
  const char * last_path_played = getLastAudioPathPlayed();
  modular_server_.response().returnResult(last_path_played);
}

void AudioController::getPositionHandler()
{
  long position = getPosition();
  modular_server_.response().returnResult(position);
}

void AudioController::getLengthHandler()
{
  long length = getLength();
  modular_server_.response().returnResult(length);
}

void AudioController::getPercentCompleteHandler()
{
  long percent_complete = getPercentComplete();
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
  int index = addTonePwmAt(frequency,speaker_ptr,volume,delay,period,on_duration,count);
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
  int index = startTonePwmAt(frequency,speaker_ptr,volume,delay,period,on_duration);
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
  int index = addNoisePwmAt(speaker_ptr,volume,delay,period,on_duration,count);
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
  int index = startNoisePwmAt(speaker_ptr,volume,delay,period,on_duration);
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
  int index = addFilteredNoisePwmAt(frequency,bandwidth,speaker_ptr,volume,delay,period,on_duration,count);
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
  int index = startFilteredNoisePwmAt(frequency,bandwidth,speaker_ptr,volume,delay,period,on_duration);
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
  stopPwm(pwm_index);
}

void AudioController::stopAllPwmHandler()
{
  stopAllPwm();
}

void AudioController::isPulsingHandler()
{
  bool pulsing = isPulsing();
  modular_server_.response().returnResult(pulsing);
}

void AudioController::playToneHandler(int index)
{
  size_t frequency = indexed_pulses_[index].frequency;
  const ConstantString * speaker_ptr = indexed_pulses_[index].speaker_ptr;
  long volume = indexed_pulses_[index].volume;
  playToneAt(frequency,speaker_ptr,volume);
}

void AudioController::playNoiseHandler(int index)
{
  const ConstantString * speaker_ptr = indexed_pulses_[index].speaker_ptr;
  long volume = indexed_pulses_[index].volume;
  playNoiseAt(speaker_ptr,volume);
}

void AudioController::playFilteredNoiseHandler(int index)
{
  size_t frequency = indexed_pulses_[index].frequency;
  double bandwidth = indexed_pulses_[index].bandwidth;
  const ConstantString * speaker_ptr = indexed_pulses_[index].speaker_ptr;
  long volume = indexed_pulses_[index].volume;
  playFilteredNoiseAt(frequency,bandwidth,speaker_ptr,volume);
}

void AudioController::stopHandler(int index)
{
  stop();
}

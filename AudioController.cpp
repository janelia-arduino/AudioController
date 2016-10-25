// ----------------------------------------------------------------------------
// AudioController.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "AudioController.h"


namespace audio_controller
{
// GUItool: begin automatically generated code
AudioSynthWaveformSine   g_tone_left;          //xy=287,312
AudioSynthWaveformSine   g_tone_right;          //xy=293,351
AudioSynthNoiseWhite     g_noise_left;         //xy=293,389
AudioSynthNoiseWhite     g_noise_right;         //xy=297,428
AudioPlaySdWav           g_play_sd_wav;     //xy=300,238
AudioPlaySdRaw           g_play_sd_raw;     //xy=301,275
AudioMixer4              g_mixer_left;         //xy=537,247
AudioMixer4              g_mixer_right;         //xy=543,323
AudioOutputI2S           g_i2s;           //xy=707,281
AudioConnection          patchCord1(g_tone_left, 0, g_mixer_left, 2);
AudioConnection          patchCord2(g_tone_right, 0, g_mixer_right, 2);
AudioConnection          patchCord3(g_noise_left, 0, g_mixer_left, 3);
AudioConnection          patchCord4(g_noise_right, 0, g_mixer_right, 3);
AudioConnection          patchCord5(g_play_sd_wav, 0, g_mixer_left, 0);
AudioConnection          patchCord6(g_play_sd_wav, 1, g_mixer_right, 0);
AudioConnection          patchCord7(g_play_sd_raw, 0, g_mixer_left, 1);
AudioConnection          patchCord8(g_play_sd_raw, 0, g_mixer_right, 1);
AudioConnection          patchCord9(g_mixer_left, 0, g_i2s, 0);
AudioConnection          patchCord10(g_mixer_right, 0, g_i2s, 1);
AudioControlSGTL5000     g_sgtl5000;     //xy=535,172
// GUItool: end automatically generated code
}

using namespace audio_controller;

AudioController::AudioController()
{
  codec_enabled_ = false;
  playing_ = false;
  path_played_[0] = 0;
}

void AudioController::setup()
{
  // Parent Setup
  ModularDevice::setup();

  // Audio Setup
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(8);

  enableAudioCodec();

  // Setup SD Card
  sd_interface_.setup();

  // Event Controller Setup
  // event_controller_.setup();

  // Pin Setup

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);

  // Add Hardware Info
  modular_server_.addHardwareInfo(constants::hardware_info);

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
                              fields_,
                              parameters_,
                              methods_,
                              callbacks_);

  // Fields
  modular_server::Field & volume_field = modular_server_.createField(constants::volume_field_name,constants::volume_default);
  volume_field.setRange(constants::volume_min,constants::volume_max);
  volume_field.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::updateVolume));

  // Parameters
  modular_server::Parameter & audio_path_parameter = modular_server_.createParameter(constants::audio_path_parameter_name);
  audio_path_parameter.setTypeString();

  modular_server::Parameter & percent_parameter = modular_server_.createParameter(constants::percent_parameter_name);
  percent_parameter.setRange(constants::percent_min,constants::percent_max);

  modular_server::Parameter & frequency_parameter = modular_server_.createParameter(constants::frequency_parameter_name);
  frequency_parameter.setRange(constants::frequency_min,constants::frequency_max);

  modular_server::Parameter & speaker_parameter = modular_server_.createParameter(constants::speaker_parameter_name);
  speaker_parameter.setTypeString();
  speaker_parameter.setSubset(constants::speaker_str_subset);

  // Methods
  modular_server::Method & get_sd_card_info_method = modular_server_.createMethod(constants::get_sd_card_info_method_name);
  get_sd_card_info_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getSDCardInfoHandler));
  get_sd_card_info_method.setReturnTypeObject();

  modular_server::Method & get_audio_paths_method = modular_server_.createMethod(constants::get_audio_paths_method_name);
  get_audio_paths_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getAudioPathsHandler));
  get_audio_paths_method.setReturnTypeArray();

  modular_server::Method & play_path_method = modular_server_.createMethod(constants::play_path_method_name);
  play_path_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::playPathHandler));
  play_path_method.addParameter(audio_path_parameter);

  modular_server::Method & play_tone_method = modular_server_.createMethod(constants::play_tone_method_name);
  play_tone_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::playToneHandler));
  play_tone_method.addParameter(frequency_parameter);
  play_tone_method.addParameter(speaker_parameter);

  modular_server::Method & play_noise_method = modular_server_.createMethod(constants::play_noise_method_name);
  play_noise_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::playNoiseHandler));
  play_noise_method.addParameter(speaker_parameter);

  modular_server::Method & stop_method = modular_server_.createMethod(constants::stop_method_name);
  stop_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::stopHandler));

  modular_server::Method & is_playing_method = modular_server_.createMethod(constants::is_playing_method_name);
  is_playing_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::isPlayingHandler));
  is_playing_method.setReturnTypeBool();

  modular_server::Method & get_last_audio_path_played_method = modular_server_.createMethod(constants::get_last_audio_path_played_method_name);
  get_last_audio_path_played_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getLastAudioPathPlayedHandler));
  get_last_audio_path_played_method.setReturnTypeString();

  modular_server::Method & get_position_method = modular_server_.createMethod(constants::get_position_method_name);
  get_position_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getPositionHandler));
  get_position_method.setReturnTypeLong();

  modular_server::Method & get_length_method = modular_server_.createMethod(constants::get_length_method_name);
  get_length_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getLengthHandler));
  get_length_method.setReturnTypeLong();

  modular_server::Method & get_percent_complete_method = modular_server_.createMethod(constants::get_percent_complete_method_name);
  get_percent_complete_method.attachFunctor(makeFunctor((Functor0 *)0,*this,&AudioController::getPercentCompleteHandler));
  get_percent_complete_method.setReturnTypeLong();

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

  playing_ = playing;
  if (playing)
  {
    path_played_[0] = 0;
    strcpy(path_played_,path_upper);
  }
  return playing;
}

void AudioController::playTone(size_t frequency, const ConstantString * const speaker_str)
{
  stop();
  audio_type_playing_ = constants::TONE_TYPE;
  if ((speaker_str == &constants::speaker_all) || (speaker_str == &constants::speaker_left))
  {
    g_tone_left.amplitude(0);
    g_tone_left.frequency(frequency);
    g_tone_left.amplitude(1);
  }
  if ((speaker_str == &constants::speaker_all) || (speaker_str == &constants::speaker_right))
  {
    g_tone_right.amplitude(0);
    g_tone_right.frequency(frequency);
    g_tone_right.amplitude(1);
  }
  updateVolume();
  playing_ = true;
}

void AudioController::playNoise(const ConstantString * const speaker_str)
{
  stop();
  audio_type_playing_ = constants::NOISE_TYPE;
  if ((speaker_str == &constants::speaker_all) || (speaker_str == &constants::speaker_left))
  {
    g_noise_left.amplitude(1);
  }
  if ((speaker_str == &constants::speaker_all) || (speaker_str == &constants::speaker_right))
  {
    g_noise_right.amplitude(1);
  }
  updateVolume();
  playing_ = true;
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
    playing_ = false;
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

void AudioController::updateVolume()
{
  if (codecEnabled())
  {
    double volume;
    modular_server_.field(constants::volume_field_name).getValue(volume);
    g_sgtl5000.volume(volume);
  }
}

void AudioController::enableAudioCodec()
{
  pinMode(SDA, INPUT);
  pinMode(SCL, INPUT);
  if (digitalRead(SDA) && digitalRead(SCL))
  {
    // This may wait forever if the SDA & SCL pins lack
    // pullup resistors so check first
    codec_enabled_ = true;
    g_sgtl5000.enable();
    updateVolume();
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
        playing_ = g_play_sd_raw.isPlaying();
        break;
      }
      case constants::WAV_TYPE:
      {
        playing_ = g_play_sd_wav.isPlaying();
        break;
      }
    }
  }
}

void AudioController::addDirectoryToResponse(File dir, const char * pwd)
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

ConstantString * const AudioController::stringToSpeakerPtr(const char * string)
{
  if (string == constants::speaker_left)
  {
    return &constants::speaker_left;
  }
  else if (string == constants::speaker_right)
  {
    return &constants::speaker_right;
  }
  else
  {
    return &constants::speaker_all;
  }
}

// Handlers must be non-blocking (avoid 'delay')
//
// modular_server_.parameter(parameter_name).getValue(value) value type must be either:
// fixed-point number (int, long, etc.)
// floating-point number (float, double)
// bool
// const char *
// ArduinoJson::JsonArray *
// ArduinoJson::JsonObject *
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server_.field(field_name).getValue(value) value type must match the field default type
// modular_server_.field(field_name).setValue(value) value type must match the field default type
// modular_server_.field(field_name).getElementValue(value) value type must match the field array element default type
// modular_server_.field(field_name).setElementValue(value) value type must match the field array element default type

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
  const char * speaker_str;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_str);
  ConstantString * speaker_ptr = stringToSpeakerPtr(speaker_str);
  playTone(frequency,speaker_ptr);
}

void AudioController::playNoiseHandler()
{
  modular_server::Response & response = modular_server_.response();
  if (!codecEnabled())
  {
    response.returnError("No audio codec chip detected.");
    return;
  }
  const char * speaker_str;
  modular_server_.parameter(constants::speaker_parameter_name).getValue(speaker_str);
  ConstantString * speaker_ptr = stringToSpeakerPtr(speaker_str);
  playNoise(speaker_ptr);
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

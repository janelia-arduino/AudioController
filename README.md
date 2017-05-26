# AudioController

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

## Ancestors

[ModularServer](https://github.com/janelia-arduino/ModularServer)

[ModularDeviceBase](https://github.com/janelia-arduino/ModularDeviceBase)

## Clients

## Devices

[modular_device_base](https://github.com/janelia-modular-devices/modular_device_base.git)

[audio_controller](https://github.com/janelia-modular-devices/audio_controller.git)

## API

```json
{
  "id":"getApi",
  "result":{
    "firmware":["AudioController"],
    "functions":[
      "getAudioMemoryUsage",
      "getAudioMemoryUsageMax",
      "resetAudioMemoryUsageMax",
      "getAudioProcessorUsage",
      "getAudioProcessorUsageMax",
      "resetAudioProcessorUsageMax",
      "getSDCardInfo",
      "getAudioPaths",
      "playPath",
      "playTone",
      "playToneAt",
      "playNoise",
      "playNoiseAt",
      "playFilteredNoise",
      "playFilteredNoiseAt",
      "stop",
      "isPlaying",
      "getLastAudioPathPlayed",
      "getPosition",
      "getLength",
      "getPercentComplete",
      "addTonePwm",
      "addTonePwmAt",
      "startTonePwm",
      "startTonePwmAt",
      "addNoisePwm",
      "addNoisePwmAt",
      "startNoisePwm",
      "startNoisePwmAt",
      "addFilteredNoisePwm",
      "addFilteredNoisePwmAt",
      "startFilteredNoisePwm",
      "startFilteredNoisePwmAt",
      "stopPwm",
      "stopAllPwm",
      "isPulsing"
    ],
    "parameters":[
      "audio_path",
      "percent",
      "frequency",
      "speaker",
      "delay",
      "period",
      "on_duration",
      "count",
      "pwm_index",
      "volume",
      "bandwidth"
    ],
    "properties":[
      "volume"
    ],
    "callbacks":[]
  }
}
```

## API Verbose

[API Verbose](./api.json)

## More Detailed Modular Device Information

[modular-devices](https://github.com/janelia-modular-devices/modular-devices)

## Install Library Dependencies

[arduino-libraries](https://github.com/janelia-arduino/arduino-libraries)

# AudioController

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

## Inherits

[ModularServer](https://github.com/janelia-arduino/ModularServer)

[ModularDeviceBase](https://github.com/janelia-arduino/ModularDeviceBase)

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

## More Detailed Modular Device Information

[modular-devices](https://github.com/janelia-modular-devices/modular-devices)

## Install Library Dependencies

[arduino-libraries](https://github.com/janelia-arduino/arduino-libraries)

#AudioController

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

[Examples](./examples)

##API

```json
{
  "id":"getApi",
  "result":{
    "firmware":["AudioController"],
    "functions":[
      "getSDCardInfo",
      "getAudioPaths",
      "playPath",
      "playTone",
      "playNoise",
      "stop",
      "isPlaying",
      "getLastAudioPathPlayed",
      "getPosition",
      "getLength",
      "getPercentComplete"
    ],
    "parameters":[
      "audio_path",
      "percent",
      "frequency",
      "speaker"
    ],
    "properties":[
      "volume"
    ],
    "callbacks":[]
  }
}

```

##More Detailed Modular Device Information

[modular-devices](https://github.com/janelia-modular-devices/modular-devices)

##Install Library Dependencies

[arduino-libraries](https://github.com/janelia-arduino/arduino-libraries)

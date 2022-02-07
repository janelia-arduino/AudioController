#include <AudioApparatus.h>


const long BAUD = 115200;
const int DELAY = 2000;
const uint16_t FREQUENCY_MIN = 1000;
const uint16_t FREQUENCY_MAX = 22000;
const uint16_t FREQUENCY_INC = 1000;

const uint16_t frequency = FREQUENCY_MIN;

AudioApparatus audio_apparatus;

void setup()
{
  audio_apparatus.setup();
}

void loop()
{
  Serial.print("audio_apparatus.playToneAt(");
  Serial.print(frequency);
  Serial.println(")");
  audio_apparatus.playToneAt(frequency,AudioApparatus::SPEAKER_ALL,25);
  delay(DELAY);
  Serial.println("");

  if (frequency == FREQUENCY_MAX)
  {
    frequency = FREQUENCY_MIN;
  }
  frequency += FREQUENCY_INC;
}

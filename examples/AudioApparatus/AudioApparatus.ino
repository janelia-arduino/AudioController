#include <AudioApparatus.h>


const long BAUD = 115200;
const int DELAY = 2000;
const uint16_t FREQUENCY_MIN = 1000;
const uint16_t FREQUENCY_MAX = 10000;
const uint16_t FREQUENCY_INC = 1000;

uint16_t frequency = FREQUENCY_MIN;

AudioApparatus aa;

void setup()
{
  aa.setup();
}

void loop()
{
  Serial.print("aa.playToneAt(");
  Serial.print(frequency);
  Serial.println(")");
  aa.playToneAt(frequency,audio_apparatus::constants::SPEAKER_ALL,25);
  delay(DELAY);
  Serial.println("");

  if (frequency == FREQUENCY_MAX)
  {
    frequency = FREQUENCY_MIN;
  }
  frequency += FREQUENCY_INC;
}

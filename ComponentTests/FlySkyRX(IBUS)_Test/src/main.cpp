#include <Arduino.h>
#include <IBusBM.h>

IBusBM ibusRc;

HardwareSerial& ibusRcSerial = Serial2;
//Connect IBUS Sense data pin to pin 13

void setup() {
  Serial.begin(115200);
  ibusRc.begin(ibusRcSerial);
}

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(uint8_t channelInput, int minLimit, int maxLimit, int defaultValue){
  uint16_t ch = ibusRc.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit); 
}

void loop() {
  for (int i = 0; i < 10; i++){
    int value = readChannel(i, -100, 100, 0);
    Serial.print("Ch");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(value);
    Serial.print(" ");
  }
  Serial.println();
  delay(10);
}
#include "./protocoll/index.hpp"
#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
  }

  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
}

void loop() {}
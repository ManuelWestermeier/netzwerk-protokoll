#include "./protocoll/index.hpp"
#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
  }

  PhysikalNode node;

  node.pins.push_back(PhysikalConnection{
      .inpPin = 0,
      .outPin = 27,
  });

  node.init();

  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
}

void loop() {}
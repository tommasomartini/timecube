#include <TimeCube.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  TimeCube timeCube;
  timeCube.run();
}

void loop() {}

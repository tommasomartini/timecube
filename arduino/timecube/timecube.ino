#include <Arduino_LSM6DS3.h>

// Accelerations in g along the axes.
float x, y, z;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  #ifdef ARDUINO_AVR_UNO_WIFI_REV2
  Serial.println("Yes");
  #else
  Serial.println("No");
  #endif

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (true);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");
}

void loop() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

//    Serial.print(x);
//    Serial.print('\t');
//    Serial.print(y);
//    Serial.print('\t');
//    Serial.println(z);
  }
}

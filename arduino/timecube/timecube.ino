#include <Arduino_LSM9DS1.h>
#include <TimeCube.h>

// Accelerations in g along the axes.
float x, y, z;

unsigned long now;
unsigned long diff;

TimeCube timeCube;

void setup() {
  Serial.begin(9600);
  while (!Serial);

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

  now = millis();
}

void loop() {
  if (!IMU.accelerationAvailable()) {
    return;
  }

  IMU.readAcceleration(x, y, z);

  /**
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.println("\t");
  /**/
  
  timeCube.update(x, y, z);

  /**
  Serial.print(timeCube.getX());
  Serial.print("\t");
  Serial.print(timeCube.getY());
  Serial.print("\t");
  Serial.print(timeCube.getZ());
  Serial.println("\t");
  /**/
  
  Serial.println(timeCube.getSideUpName().c_str());
  // Serial.println(timeCube.getLargestComponent());
}

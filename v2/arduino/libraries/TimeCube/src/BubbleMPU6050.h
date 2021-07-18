#ifndef BUBBLE_MPU6050_H
#define BUBBLE_MPU6050_H

#include "Bubble.h"

#include <Arduino.h>
#include <MPU6050.h>


class BubbleMPU6050: public Bubble {
private:
  MPU6050 _imu;

  int16_t _rawX, _rawY, _rawZ;

  float decodeAccelScaleRange(const uint8_t);
  virtual bool readGravity();

public:
  virtual bool initialize();
};

#endif

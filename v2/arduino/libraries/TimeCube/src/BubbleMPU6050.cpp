#include "BubbleMPU6050.h"


bool BubbleMPU6050::initialize() {
  _imu.initialize();
  return true;
}

float BubbleMPU6050::decodeAccelScaleRange(const uint8_t code) {
	switch (code) {
	case 0:
		return 2.0f;
	case 1:
		return 4.0f;
	case 2:
		return 8.0f;
	case 3:
		return 16.0f;
	default:
		return -1.0;
	}
}

bool BubbleMPU6050::readGravity() {
  _imu.getAcceleration(&_rawX, &_rawY, &_rawZ);

  uint8_t fullScaleRangeCode = _imu.getFullScaleAccelRange();
  float scaleRange = decodeAccelScaleRange(fullScaleRangeCode);

  _normX = _rawX / scaleRange;
  _normY = _rawY / scaleRange;
  _normZ = _rawZ / scaleRange;

  normalizeGravity();
  return true;
}

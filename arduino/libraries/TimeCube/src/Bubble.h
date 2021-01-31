#ifndef BUBBLE_H
#define BUBBLE_H

#include "Constants.h"

class Bubble {
private:
  // The gravity vector must be at this angle from one of the axes for a timer to start.
  const float TILT_THRESHOLD_deg = 30.0;

  float _normX, _normY, _normZ;
  Side* _upSide;

  float computeNorm(float x, float y, float z);
  float computeGravityAngleToAxisDeg(Axis axis);
  bool gravityCloseToAxis(Axis axis);
  Axis getLargestComponent();
  void updateGravity(float x, float y, float z);

public:
  Bubble(Side* upSide);

  void updateUpSide(float x, float y, float z);
};

#endif

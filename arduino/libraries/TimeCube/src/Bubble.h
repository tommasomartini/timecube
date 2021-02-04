#ifndef BUBBLE_H
#define BUBBLE_H

#include "Constants.h"

class Bubble {
private:
  // The gravity vector must be at this angle from one of the axes for a timer to start.
  const float TILT_THRESHOLD_deg = 30.0;

  float _normX, _normY, _normZ;
  Side _upSide;

  bool readGravity();
  float computeNorm() const;
  float computeGravityAngleToAxisDeg(Axis axis) const;
  bool gravityCloseToAxis(Axis axis) const;
  Axis getLargestComponent() const;
  void normalizeGravity();

public:
  Bubble();

  /**
   * Returns false if initialization fails; true otherwise.
   */
  bool initialize();
  Side getUpSide();
};

#endif

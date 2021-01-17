/*
 * Axes definitions:
 * X: from rear to front
 * Y: from left to right
 * Z: from top to bottom
 *
 * Gravity is along Z when the cube is in its initial position.
 */
#ifndef TIMECUBE_H
#define TIMECUBE_H

#include <string>


class TimeCube {

  enum Axis {
    AXIS_X,
    AXIS_Y,
    AXIS_Z,
  };

  enum Side {
    SIDE_NONE,
    SIDE_FRONT,
    SIDE_REAR,
    SIDE_TOP,
    SIDE_BOTTOM,
    SIDE_LEFT,
    SIDE_RIGHT,
  };

private:
  // The gravity vector must fall this angle from one of the axes for a timer to start.
  const float TILT_THRESHOLD_deg = 30.0;

  float _x, _y, _z;
  Side _sideUp;

  float computeNorm(float x, float y, float z);
  float computeGravityAngleToAxisDeg(Axis axis);
  bool gravityCloseToAxis(Axis axis);
  Axis getLargestComponent();
  void setSideUp();

public:
  TimeCube();

  void update(float x, float y, float z);

  float getX();
  float getY();
  float getZ();
  std::string getSideUpName();
};

#endif

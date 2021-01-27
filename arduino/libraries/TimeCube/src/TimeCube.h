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
    SIDE_NONE,  // used when booting or transitioning
    SIDE_FRONT,
    SIDE_REAR,
    SIDE_TOP,
    SIDE_BOTTOM,
    SIDE_LEFT,
    SIDE_RIGHT,
  };

  enum State {
    // Whenever the up side points towards the sky.
    // No timer is running and no blinking going on.
    STANDBY,

    // A timer is running, but it has not expired yet.
    COUNTING,

    // A timer expired and the blinking started.
    BLINKING,

    // A timer expired and the relative blinking expired too.
    EXPIRED,
  };

private:
  // The gravity vector must be at this angle from one of the axes for a timer to start.
  const float TILT_THRESHOLD_deg = 30.0;

  float _x, _y, _z;
  Side _sideUp;
  State _state;

  float computeNorm(float x, float y, float z);
  float computeGravityAngleToAxisDeg(Axis axis);
  bool gravityCloseToAxis(Axis axis);
  Axis getLargestComponent();

  /**
   * Starts a new timer.
   */
  void startTimer();

  /**
   * Goes to standby mode.
   */
  void standby();

  /**
   * Called when a new up-side is detected.
   */
  void newUpSideCallback();

  /**
   * Called when the same up-side is detected in consecutive cycles.
   */
  void sameUpSideCallback();

  /**
   * Returns true if the up side was updated with, false if the new
   * up side is the same as the one before calling the function.
   */
  bool updateSideUp();

public:
  TimeCube();

  /**
   * Updates TimeCube with a new gravity vector.
   * This function should be called continuously.
   */
  void update(float x, float y, float z);

  float getX();
  float getY();
  float getZ();
  std::string getSideUpName();
};

#endif

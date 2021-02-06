#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

/*
 * Axes definitions:
 * X: from rear to front
 * Y: from left to right
 * Z: from top to bottom
 *
 * Gravity is along Z when the cube is in its initial position.
 */
enum Axis {
  AXIS_X,
  AXIS_Y,
  AXIS_Z,
};

enum Side {
  SIDE_FRONT,
  SIDE_REAR,
  SIDE_TOP,
  SIDE_BOTTOM,
  SIDE_LEFT,
  SIDE_RIGHT,
};

enum State {
  // Either the up side points towards the sky or blinking expired.
  // No timer is running and no blinking going on.
  STATE_STANDBY,

  // A timer is running, but it has not expired yet.
  STATE_COUNTING,

  // A timer expired and the blinking started.
  STATE_BLINKING,
};

std::string nameOfAxis(Axis axis);
std::string nameOfSide(Side side);
std::string nameOfState(State state);

#endif

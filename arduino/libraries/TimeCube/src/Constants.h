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
  STATE_STANDBY,

  // A timer is running, but it has not expired yet.
  STATE_COUNTING,

  // A timer expired and the blinking started.
  STATE_BLINKING,

  // A timer expired and the relative blinking expired too.
  STATE_EXPIRED,
};

std::string nameOfAxis(Axis axis) {
  switch (axis) {
  case AXIS_X:
    return "X";
  case AXIS_Y:
    return "Y";
  case AXIS_Z:
    return "Z";
  default:
    return "unknown";
  }
}

std::string nameOfSide(Side side) {
  switch (side) {
  case SIDE_NONE:
    return "None";
  case SIDE_TOP:
    return "Top";
  case SIDE_BOTTOM:
    return "Bottom";
  case SIDE_LEFT:
    return "Left";
  case SIDE_RIGHT:
    return "Right";
  case SIDE_REAR:
    return "Rear";
  case SIDE_FRONT:
    return "Front";
  default:
    return "unknown";
  }
}

std::string nameOfState(State state) {
  switch (state) {
  case STATE_STANDBY:
    return "Standby";
  case STATE_COUNTING:
    return "Counting";
  case STATE_BLINKING:
    return "Blinking";
  case STATE_EXPIRED:
    return "Expired";
  default:
    return "unknown";
  }
}

#endif

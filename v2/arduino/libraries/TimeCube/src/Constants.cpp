#include "Constants.h"


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
  default:
    return "unknown";
  }
}

#include "TimeCube.h"

#include <cmath>

namespace {
float EPS = 1e-6;
}


TimeCube::TimeCube()
: _x(0), _y(0), _z(1), _sideUp(Side::SIDE_NONE), _state(EXPIRED)
{}

float TimeCube::computeNorm(const float x, const float y, const float z) {
  return std::sqrt(x * x + y * y + z * z);
}

float TimeCube::computeGravityAngleToAxisDeg(const Axis axis) {
  float component;
  switch (axis) {
  case AXIS_X:
    component = _x;
    break;
  case AXIS_Y:
    component = _y;
    break;
  case AXIS_Z:
    component = _z;
    break;
  }

  return std::acos(component) * 180.0 / M_PI;
}

bool TimeCube::gravityCloseToAxis(const Axis axis) {
  float angleWithAxisDeg = computeGravityAngleToAxisDeg(axis);
  if (angleWithAxisDeg <= TILT_THRESHOLD_deg) {
    return true;
  }

  if (angleWithAxisDeg >= (180.0 - TILT_THRESHOLD_deg)) {
    return true;
  }

  return false;
}

TimeCube::Axis TimeCube::getLargestComponent() {
  // The initial guess is that the X component is the largest.
  Axis axis = AXIS_X;
  float magnitude = _x;

  if (std::abs(_y) > std::abs(magnitude)) {
    axis = AXIS_Y;
    magnitude = _y;
  }
  if (std::abs(_z) > std::abs(magnitude)) {
    axis = AXIS_Z;
    magnitude = _z;
  }

  return axis;
}

bool TimeCube::updateSideUp() {
  Side oldSide = _sideUp;

  Axis dominantComponent = getLargestComponent();
  if (!gravityCloseToAxis(dominantComponent)) {
    // Transitioning between two up-sides.
    _sideUp = SIDE_NONE;
  } else {
    switch (dominantComponent) {
      case AXIS_X:
        _sideUp = _x >= 0 ? Side::SIDE_REAR : Side::SIDE_FRONT;
        break;

      case AXIS_Y:
        _sideUp = _y >= 0 ? Side::SIDE_LEFT : Side::SIDE_RIGHT;
        break;

      // case AXIS_Z:
      default:
        _sideUp = _z >= 0 ? Side::SIDE_TOP : Side::SIDE_BOTTOM;
        break;
      }
  }

  return _sideUp != oldSide;
}

void TimeCube::standby() {
  switch (_state) {
    case COUNTING:
      // Stop the running timer.
      break;
    case BLINKING:
      // Stop the running blinker.
      break;
    default:
      // Nothing to do.
      break;
  }

  _state = STANDBY;
}

void TimeCube::startTimer() {
  _state = COUNTING;
}

void TimeCube::newUpSideCallback() {
  if (_sideUp == SIDE_TOP) {
    // Go back to standby.
    standby();
  } else {
    // Start the relative timer.
  }
}

void TimeCube::sameUpSideCallback() {
  if (_sideUp == SIDE_TOP) {
    // Nothing to do: remain in standby mode.
    return;
  }

  if (_state == EXPIRED) {
    // Nothing to do: wait to go back in standby or to start another timer.
    return;
  }

  switch (_state) {
    case COUNTING:
      // Check if the timer expired.
      break;
    case BLINKING:
      // Check if the blinking expired.
      break;
    default:
      break;
  }
}

void TimeCube::update(const float x, const float y, const float z) {
  float norm = computeNorm(x, y, z);
  if (norm < EPS) {
    // Do not update.
    return;
  }

  _x = x / norm;
  _y = y / norm;
  _z = z / norm;

  if (updateSideUp()) {
    newUpSideCallback();
  } else {
    sameUpSideCallback();
  }
}

float TimeCube::getX() {
  return _x;
}

float TimeCube::getY() {
  return _y;
}

float TimeCube::getZ() {
  return _z;
}

std::string TimeCube::getSideUpName() {
  switch (_sideUp) {
  case SIDE_BOTTOM:
    return "bottom";

  case SIDE_TOP:
    return "top";

  case SIDE_REAR:
    return "rear";

  case SIDE_FRONT:
    return "front";

  case SIDE_LEFT:
    return "left";

  case SIDE_RIGHT:
    return "right";

  default:
    return "none";
  }
}

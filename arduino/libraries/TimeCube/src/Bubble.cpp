#include "Bubble.h"

#include <cmath>

namespace {
float EPS = 1e-6;
}


Bubble::Bubble()
: _x(0), _y(0), _z(1), _sideUp(Side::SIDE_NONE)
{}

float Bubble::computeNorm(const float x, const float y, const float z) {
  return std::sqrt(x * x + y * y + z * z);
}

float Bubble::computeGravityAngleToAxisDeg(const Axis axis) {
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

bool Bubble::gravityCloseToAxis(const Axis axis) {
  float angleWithAxisDeg = computeGravityAngleToAxisDeg(axis);
  if (angleWithAxisDeg <= TILT_THRESHOLD_deg) {
    return true;
  }

  if (angleWithAxisDeg >= (180.0 - TILT_THRESHOLD_deg)) {
    return true;
  }

  return false;
}

Axis Bubble::getLargestComponent() {
  // The initial guess is that the X component is the largest.
  Axis axis = AXIS_X;
  float magnitude = _x;

  if (abs(_y) > abs(magnitude)) {
    axis = AXIS_Y;
    magnitude = _y;
  }
  if (abs(_z) > abs(magnitude)) {
    axis = AXIS_Z;
    magnitude = _z;
  }

  return axis;
}

void Bubble::updateGravity(float x, float y, float z) {
  float norm = computeNorm(x, y, z);
  if (norm < EPS) {
    // Do not update.
    return;
  }

  _x = x / norm;
  _y = y / norm;
  _z = z / norm;
}

void Bubble::updateUpSide() {
  Axis dominantComponent = getLargestComponent();
  if (!gravityCloseToAxis(dominantComponent)) {
    // Transitioning does not count as a new side up: do not update.
    return;
  }

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

Side Bubble::getUpSide(const float x, const float y, const float z) {
  updateGravity(x, y, z);
  updateUpSide();
  return _sideUp;
}

#include "Bubble.h"

#include <cmath>

namespace {
float EPS = 1e-6;
}


Bubble::Bubble(Side* upSide)
: _normX(1.0f), _normY(1.0f), _normZ(1.0f), _upSide(upSide)
{}

float Bubble::computeNorm(float x, float y, float z) const {
  return std::sqrt(x * x + y * y + z * z);
}

float Bubble::computeGravityAngleToAxisDeg(const Axis axis) const {
  float component;
  switch (axis) {
  case AXIS_X:
    component = _normX;
    break;
  case AXIS_Y:
    component = _normY;
    break;
  case AXIS_Z:
    component = _normZ;
    break;
  }

  return std::acos(component) * 180.0 / M_PI;
}

bool Bubble::gravityCloseToAxis(const Axis axis) const {
  float angleWithAxisDeg = computeGravityAngleToAxisDeg(axis);
  if (angleWithAxisDeg <= TILT_THRESHOLD_deg) {
    return true;
  }

  if (angleWithAxisDeg >= (180.0 - TILT_THRESHOLD_deg)) {
    return true;
  }

  return false;
}

Axis Bubble::getLargestComponent() const {
  // The initial guess is that the X component is the largest.
  Axis axis = AXIS_X;
  float magnitude = _normX;

  if (abs(_normY) > abs(magnitude)) {
    axis = AXIS_Y;
    magnitude = _normY;
  }
  if (abs(_normZ) > abs(magnitude)) {
    axis = AXIS_Z;
  }

  return axis;
}

void Bubble::updateGravity(float x, float y, float z) {
  float norm = computeNorm(x, y, z);
  if (norm < EPS) {
    // Do not update.
    return;
  }

  _normX = x / norm;
  _normY = y / norm;
  _normZ = z / norm;
}

void Bubble::updateUpSide(float x, float y, float z) {
  updateGravity(x, y, z);

  Axis dominantComponent = getLargestComponent();
  if (!gravityCloseToAxis(dominantComponent)) {
    // Transitioning does not count as a new side up: do not update.
    return;
  }

  switch (dominantComponent) {
  case AXIS_X:
    *_upSide = _normX >= 0 ? Side::SIDE_REAR : Side::SIDE_FRONT;
    break;

  case AXIS_Y:
    *_upSide = _normY >= 0 ? Side::SIDE_LEFT : Side::SIDE_RIGHT;
    break;

  default:
    // case AXIS_Z:
    *_upSide = _normZ >= 0 ? Side::SIDE_TOP : Side::SIDE_BOTTOM;
    break;
  }
}

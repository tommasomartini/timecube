#include "Bubble.h"

#include <Arduino.h>

namespace {
float EPS = 1e-6;

// How long to try to initialize before giving up.
unsigned long MAX_INIT_TIME_s = 3;
}


Bubble::Bubble()
: _normX(0.0f), _normY(0.0f), _normZ(1.0f), _upSide()
{}

float Bubble::computeNorm() const {
  return sqrt(_normX * _normX + _normY * _normY + _normZ * _normZ);
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

  return acos(component) * 180.0 / M_PI;
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

void Bubble::normalizeGravity() {
  float norm = computeNorm();
  if (norm < EPS) {
    // Do not update.
    return;
  }

  _normX = _normX / norm;
  _normY = _normY / norm;
  _normZ = _normZ / norm;
}

Side Bubble::getUpSide() {
  if (!readGravity()) {
    // If the gravity reading fails return the previous up-side.
    return _upSide;
  }

  Axis dominantComponent = getLargestComponent();
  if (!gravityCloseToAxis(dominantComponent)) {
    // Transitioning phase, return the previous up-side.
    return _upSide;
  }

  switch (dominantComponent) {
  case AXIS_X:
    _upSide = _normX >= 0 ? Side::SIDE_REAR : Side::SIDE_FRONT;
    break;

  case AXIS_Y:
    _upSide = _normY >= 0 ? Side::SIDE_LEFT : Side::SIDE_RIGHT;
    break;

  default:
    // case AXIS_Z:
    _upSide = _normZ >= 0 ? Side::SIDE_TOP : Side::SIDE_BOTTOM;
    break;
  }

  return _upSide;
}

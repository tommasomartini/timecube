#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
 * The longest timer that can be set is half the largest number that the time
 * keeper can hold. We use "unsigned long" types for time counters, which
 * are represented with 4 bytes on an nRF52840 chip (32-bit).
 * A time counter can assume 2^32 different values, from 0 to 2^32 - 1.
 * The largest timer that we allow is then:
 *  floor((2^32 - 1) / 2) = 2^31 - 1 = 2147483647
 * This value in milliseconds corresponds to more than 24 days.
 */
const unsigned long MAX_TIMER = 2147483647L;

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
  SIDE_TOP,
  SIDE_BOTTOM,
  SIDE_FRONT,
  SIDE_REAR,
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

#endif

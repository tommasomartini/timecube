#include "Timers.h"

unsigned long getTimerDuration(const Side side) {
  unsigned long durationInMinutes = 0;
  switch (side) {
  case SIDE_FRONT:
    durationInMinutes = 1;
    break;
  case SIDE_REAR:
    durationInMinutes = 23;
    break;
  case SIDE_BOTTOM:
    durationInMinutes = 37;
    break;
  case SIDE_LEFT:
    durationInMinutes = 5;
    break;
  case SIDE_RIGHT:
    durationInMinutes = 11;
    break;
  }

  // Convert to milliseconds.
  return 1000 * 60 * durationInMinutes;
}

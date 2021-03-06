#ifndef TIMECUBE_H
#define TIMECUBE_H

#include "Bubble.h"
#include "Pitches.h"

#include <Arduino.h>

// #define DEBUG 0


class TimeCube {
private:
  const unsigned long BLINK_INTERVAL_ms = 200;
  const unsigned long BLINK_PITCH = NOTE_A3;
  const int BLINK_PIN = A5;
  const unsigned long BLINKING_DURATION_ms = 1000 * 60;

  unsigned long _now;
  unsigned long _stateTransitionAt;
  unsigned long _toggleBlinkAt;
  bool _blinkOn;
  Side _upSide;
  State _state;
  Bubble _bubble;

  bool genericTimeExpired(unsigned long expirationTime) const;
  bool stateExpired() const;
  bool timeToToggleBlink() const;

  void setTimer();
  void setBlink();

  void toggleBlink();

  bool updateUpSide();

  void executeStandby();
  void executeCounting();
  void executeBlinking();

public:
  TimeCube();

  void run();
};

#endif

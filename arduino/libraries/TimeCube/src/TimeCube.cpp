#include "TimeCube.h"

#include <Arduino.h>


TimeCube::TimeCube()
: _now(0L)
, _stateTransitionAt(0L)
, _toggleBlinkAt(0L)
, _blinkOn(false)
, _upSide()
, _state(STATE_STANDBY)
, _bubble()
{
  if (!_bubble.initialize()) {
    // If the IMU fails, do not continue.

#ifdef DEBUG
    Serial.println("Failed to initialize");
#endif

    while (true);
  }

  _upSide = _bubble.getUpSide();

#ifdef DEBUG
  Serial.print("TimeCube initialized with up-side: ");
  Serial.println(nameOfSide(_upSide).c_str());
#endif

  pinMode(BLINK_PIN, OUTPUT);
  noTone(BLINK_PIN);
}

bool TimeCube::updateUpSide() {
  Side prevUpSide = _upSide;
  _upSide = _bubble.getUpSide();
  return prevUpSide != _upSide;
}

bool TimeCube::stateExpired() const {
  // TODO Check for overflow.
  return _now >= _stateTransitionAt;
}

bool TimeCube::timeToToggleBlink() const {
  // TODO Check for overflow.
  return _now >= _toggleBlinkAt;
}

void TimeCube::setTimer() {
  unsigned long timerDuration = 1000 * 3;
  _stateTransitionAt = _now + timerDuration;
}

void TimeCube::setBlink() {
  _stateTransitionAt = _now + BLINKING_DURATION_ms;
  _toggleBlinkAt = _now + BLINK_INTERVAL_ms;
}

void TimeCube::toggleBlink() {
  _blinkOn = !_blinkOn;
  if (_blinkOn) {
    tone(BLINK_PIN, BLINK_PITCH);
  } else {
    noTone(BLINK_PIN);
  }
  _toggleBlinkAt = _now + BLINK_INTERVAL_ms;
}

void TimeCube::executeStandby() {
  if (updateUpSide()) {
    if (_upSide != SIDE_TOP) {
      _state = STATE_COUNTING;

#ifdef DEBUG
      Serial.print(nameOfState(STATE_STANDBY).c_str());
      Serial.print(" -> ");
      Serial.println(nameOfState(STATE_COUNTING).c_str());

      Serial.print(" Set timer for side: ");
      Serial.println(nameOfSide(_upSide).c_str());
#endif

      setTimer();
    }
  }
}

void TimeCube::executeCounting() {
  if (updateUpSide()) {
    if (_upSide == SIDE_TOP) {
      // Go back to standby.
      _state = STATE_STANDBY;
#ifdef DEBUG
      Serial.print(nameOfState(STATE_COUNTING).c_str());
      Serial.print(" -> ");
      Serial.println(nameOfState(STATE_STANDBY).c_str());
#endif
      return;
    }

    // Activate a different timer.
    setTimer();
#ifdef DEBUG
    Serial.print(" Set timer for side: ");
    Serial.println(nameOfSide(_upSide).c_str());
#endif
    return;
  }

  // No side update.

  if (stateExpired()) {
    _state = STATE_BLINKING;
#ifdef DEBUG
    Serial.print(nameOfState(STATE_COUNTING).c_str());
    Serial.print(" -> ");
    Serial.println(nameOfState(STATE_BLINKING).c_str());
#endif
    setBlink();
    toggleBlink();
  }
}

void TimeCube::executeBlinking() {
  if (updateUpSide()) {
    // Quit blinking.
    _blinkOn = false;
    noTone(BLINK_PIN);

    if (_upSide == SIDE_TOP) {
      _state = STATE_STANDBY;
#ifdef DEBUG
      Serial.print(nameOfState(STATE_BLINKING).c_str());
      Serial.print(" -> ");
      Serial.println(nameOfState(STATE_STANDBY).c_str());
#endif
      return;
    }

    _state = STATE_COUNTING;
#ifdef DEBUG
    Serial.print(nameOfState(STATE_BLINKING).c_str());
    Serial.print(" -> ");
    Serial.println(nameOfState(STATE_COUNTING).c_str());

    Serial.print(" Set timer for side: ");
    Serial.println(nameOfSide(_upSide).c_str());
#endif
    setTimer();
    return;
  }

  // No side update.

  if (timeToToggleBlink()) {
    toggleBlink();

#ifdef DEBUG
    Serial.print(" Blink ");
    if (_blinkOn) {
      Serial.println("ON");
    } else {
      Serial.println("OFF");
    }
#endif
  }

  if (stateExpired()) {
    // Quit blinking.
    _blinkOn = false;
    noTone(BLINK_PIN);

    _state = STATE_STANDBY;
#ifdef DEBUG
    Serial.print(nameOfState(STATE_BLINKING).c_str());
    Serial.print(" -> ");
    Serial.println(nameOfState(STATE_STANDBY).c_str());
#endif
  }
}

void TimeCube::run() {
  while (true) {
    _now = millis();

    switch (_state) {
    case STATE_STANDBY:
      executeStandby();
      break;

    case STATE_COUNTING:
      executeCounting();
      break;

    case STATE_BLINKING:
      executeBlinking();
      break;
    }
  }
}

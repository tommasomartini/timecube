#include "TimeCube.h"

#include "Timers.h"
#include "Tunes.h"

//#include <MPU6050.h>


//void doit() {
//	MPU6050 mpu;
//	mpu.initialize();
//
////	uint8_t mode = MPU6050_DLPF_BW_5;
//
//	uint8_t rate = 255;
//
////	mpu.setDLPFMode(MPU6050_DLPF_BW_5);
//	mpu.setRate(rate);
//	mpu.setSlaveDelayEnabled(0, true);
//	mpu.setSlave4MasterDelay(rate);
//
//	int16_t* x;
//	int16_t* y;
//	int16_t* z;
//
//	while (true) {
//		int16_t x = mpu.getAccelerationX();
//		Serial.println(x);
//	}
//}


TimeCube::TimeCube()
: _now(0L)
, _stateTransitionAt(0L)
, _toggleBlinkAt(0L)
, _blinkOn(false)
, _upSide(SIDE_TOP)
, _state(STATE_STANDBY)
, _bubble()
{
  if (!checkMaxTimer()) {
    // Invalid timer value.

#ifdef DEBUG
    Serial.println("Invalid timer value");
#endif

    while (true);
  }

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
  digitalWrite(BLINK_PIN, LOW);

#ifdef DEBUG
  // In debug the built-in LED is synchronized with the buzzer.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif

  playTurnOnTune(BLINK_PIN);
}

bool TimeCube::updateUpSide() {
  Side prevUpSide = _upSide;
  _upSide = _bubble.getUpSide();
  return prevUpSide != _upSide;
}

bool TimeCube::genericTimeExpired(unsigned long expirationTime) const {
  // The timer is always "now + something": it will always wrap before "now".
  if (_now < expirationTime) {
    return false;
  }

  // "now" is "to the right" of timer. Is it overflow?
  if (_now - expirationTime > MAX_TIMER) {
    // Overflow.
    return false;
  }

  return true;
}

bool TimeCube::stateExpired() const {
  return genericTimeExpired(_stateTransitionAt);
}

bool TimeCube::timeToToggleBlink() const {
  return genericTimeExpired(_toggleBlinkAt);
}

void TimeCube::setTimer() {
  unsigned long timerDuration = getTimerDuration(_upSide);
  _stateTransitionAt = _now + timerDuration;

#ifdef DEBUG
  Serial.print(" Start timer: ");
  Serial.print(timerDuration / 1000);
  Serial.println(" seconds");
#endif
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

#ifdef DEBUG
  digitalWrite(LED_BUILTIN, _blinkOn);
#endif

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

      playStartTimerTune(BLINK_PIN);
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

      playInterruptTimerTune(BLINK_PIN);
      return;
    }

    // Activate a different timer.
    setTimer();

#ifdef DEBUG
    Serial.print(" Set timer for side: ");
    Serial.println(nameOfSide(_upSide).c_str());
#endif

    playStartTimerTune(BLINK_PIN);
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
    digitalWrite(BLINK_PIN, LOW);

#ifdef DEBUG
    digitalWrite(LED_BUILTIN, LOW);
#endif

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

    playStartTimerTune(BLINK_PIN);
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
    digitalWrite(BLINK_PIN, LOW);

#ifdef DEBUG
    digitalWrite(LED_BUILTIN, LOW);
#endif

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


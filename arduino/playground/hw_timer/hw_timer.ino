volatile bool ledState = LOW;

extern "C" {
  void TIMER4_IRQHandler_v() {
    if (NRF_TIMER4->EVENTS_COMPARE[0] & TIMER_EVENTS_COMPARE_EVENTS_COMPARE_Msk) {
      ledState = !ledState;
      digitalWrite(LED_BUILTIN, ledState);
      NRF_TIMER4->EVENTS_COMPARE[0] = 0;
    }
  }
}

void startTimer() {
  NRF_TIMER_Type* TIMER = NRF_TIMER4;

  // Stop the timer task before setting it.
  TIMER->TASKS_STOP = (1 << TIMER_TASKS_START_TASKS_START_Pos) & TIMER_TASKS_START_TASKS_START_Msk;

  // Reset the time.
  TIMER->TASKS_CLEAR = (1 << TIMER_TASKS_CLEAR_TASKS_CLEAR_Pos) & TIMER_TASKS_CLEAR_TASKS_CLEAR_Msk;

  // When the compare triggers clear the timer and generate an interrupt.
  TIMER->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos) & TIMER_SHORTS_COMPARE0_CLEAR_Msk;
  TIMER->INTENSET = (TIMER_INTENSET_COMPARE0_Set << TIMER_INTENSET_COMPARE0_Pos) & TIMER_INTENSET_COMPARE0_Msk;

  TIMER->MODE = (TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos) & TIMER_MODE_MODE_Msk;
  TIMER->BITMODE = (TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos) & TIMER_BITMODE_BITMODE_Msk;

  // f_timer = 16 MHz / (2^PRESCALER).
  // The highest possible prescaler is 15 (1111 in binary), which gives ~488 Hz.
  unsigned int prescaler = 8;
  TIMER->PRESCALER = (prescaler << TIMER_PRESCALER_PRESCALER_Pos) & TIMER_PRESCALER_PRESCALER_Msk;

  unsigned int cc = 62500;
  TIMER->CC[0] = (cc << TIMER_CC_CC_Pos) & TIMER_CC_CC_Msk;

  NVIC_EnableIRQ(TIMER4_IRQn);
  TIMER->TASKS_START = (1 << TIMER_TASKS_START_TASKS_START_Pos) & TIMER_TASKS_START_TASKS_START_Msk;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  startTimer();

  Serial.println("Starting");
}

void loop() {}

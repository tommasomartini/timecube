volatile bool ledState = LOW;
volatile NRF_RTC_Type* TIMER = NRF_RTC2;

//extern "C" {
//  void TIMER4_IRQHandler_v() {
//    if (TIMER->EVENTS_COMPARE[0] & TIMER_EVENTS_COMPARE_EVENTS_COMPARE_Msk) {
//      ledState = !ledState;
//      digitalWrite(LED_BUILTIN, ledState);
//      TIMER->EVENTS_COMPARE[0] = 0;
//    }
//  }
//}

void printHighFreqClockStatus() {
  unsigned int HFCLK_run = NRF_CLOCK->HFCLKRUN;

  unsigned int triggered = (HFCLK_run & CLOCK_HFCLKRUN_STATUS_Msk) >> CLOCK_HFCLKRUN_STATUS_Pos;
  switch (triggered) {
    case CLOCK_HFCLKRUN_STATUS_NotTriggered:
      Serial.println("HFCLK not triggered");
      break;
    case CLOCK_HFCLKRUN_STATUS_Triggered:
      Serial.println("HFCLK triggered");
      break;
    default:
      Serial.println("HFCLK run status unknown");
  }

  unsigned int HFCLK_stat = NRF_CLOCK->HFCLKSTAT;

  unsigned int state = (HFCLK_stat & CLOCK_HFCLKSTAT_STATE_Msk) >> CLOCK_HFCLKSTAT_STATE_Pos;
  switch (state) {
    case CLOCK_HFCLKSTAT_STATE_NotRunning:
      Serial.println("HFCLK state: Not Running");
      break;
    case CLOCK_HFCLKSTAT_STATE_Running:
      Serial.println("HFCLK state: Running");
      break;
    default:
      Serial.println("HFCLK state: unknown");
  }
  
  unsigned int src = (HFCLK_stat & CLOCK_HFCLKSTAT_SRC_Msk) >> CLOCK_HFCLKSTAT_SRC_Pos;
  switch (src) {
    case CLOCK_HFCLKSTAT_SRC_RC:
      Serial.println("HFCLK surce: 64 MHz internal oscillator (HFINT)");
      break;
    case CLOCK_HFCLKSTAT_SRC_Xtal:
      Serial.println("HFCLK source: 64 MHz crystal oscillator (HFXO)");
      break;
    default:
      Serial.println("HFCLK source: unknown");
  }
}

void printLowFreqClockStatus() {
  unsigned int LFCLK_run = NRF_CLOCK->LFCLKRUN;

  unsigned int triggered = (LFCLK_run & CLOCK_LFCLKRUN_STATUS_Msk) >> CLOCK_LFCLKRUN_STATUS_Pos;
  switch (triggered) {
    case CLOCK_LFCLKRUN_STATUS_NotTriggered:
      Serial.println("LFCLK not triggered");
      break;
    case CLOCK_LFCLKRUN_STATUS_Triggered:
      Serial.println("LFCLK triggered");
      break;
    default:
      Serial.println("LFCLK run status unknown");
  }

  unsigned int LFCLK_stat = NRF_CLOCK->LFCLKSTAT;

  unsigned int state = (LFCLK_stat & CLOCK_LFCLKSTAT_STATE_Msk) >> CLOCK_LFCLKSTAT_STATE_Pos;
  switch (state) {
    case CLOCK_LFCLKSTAT_STATE_NotRunning:
      Serial.println("LFCLK state: Not Running");
      break;
    case CLOCK_LFCLKSTAT_STATE_Running:
      Serial.println("LFCLK state: Running");
      break;
    default:
      Serial.println("LFCLK state: unknown");
  }
  
  unsigned int src = (LFCLK_stat & CLOCK_LFCLKSTAT_SRC_Msk) >> CLOCK_LFCLKSTAT_SRC_Pos;
  switch (src) {
    case CLOCK_LFCLKSTAT_SRC_RC:
      Serial.println("LFCLK surce: 32.768 kHz RC oscillator (LFRC)");
      break;
    case CLOCK_LFCLKSTAT_SRC_Xtal:
      Serial.println("LFCLK source: 32.768 kHz crystal oscillator (LFXO)");
      break;
    case CLOCK_LFCLKSTAT_SRC_Synth:
      Serial.println("LFCLK source: 32.768 kHz synthesized from HFCLK (LFSYNT)");
      break;
    default:
      Serial.println("LFCLK source: unknown");
  }

  unsigned int mode = (NRF_CLOCK->LFRCMODE & CLOCK_LFRCMODE_STATUS_Msk) >> CLOCK_LFRCMODE_STATUS_Pos;
  switch (mode) {
    case CLOCK_LFRCMODE_STATUS_Normal:
      Serial.println("LFRC mode configuration: Normal mode");
      break;
    case CLOCK_LFRCMODE_STATUS_ULP:
      Serial.println("LFRC mode configuration: Ultra-low power mode (ULP)");
      break;
    default:
      Serial.println("LFRC mode configuration: unknown");
  }
}

void startClock() {
  // TODO: start the HFXO and use it to calibrate the LFXO. Then disable the HFXO to save power.

  NRF_CLOCK->TASKS_HFCLKSTOP = (1 << CLOCK_TASKS_HFCLKSTOP_TASKS_HFCLKSTOP_Pos) & CLOCK_TASKS_HFCLKSTOP_TASKS_HFCLKSTOP_Msk;
  NRF_CLOCK->TASKS_LFCLKSTOP = (1 << CLOCK_TASKS_LFCLKSTOP_TASKS_LFCLKSTOP_Pos) & CLOCK_TASKS_LFCLKSTOP_TASKS_LFCLKSTOP_Msk;

  NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos) & CLOCK_LFCLKSRC_SRC_Msk;
  NRF_CLOCK->LFRCMODE = (CLOCK_LFRCMODE_MODE_ULP << CLOCK_LFRCMODE_MODE_Pos) & CLOCK_LFRCMODE_MODE_Msk;

  NRF_CLOCK->TASKS_LFCLKSTART = (1 << CLOCK_TASKS_LFCLKSTART_TASKS_LFCLKSTART_Pos) & CLOCK_TASKS_LFCLKSTART_TASKS_LFCLKSTART_Msk;
  
  printLowFreqClockStatus();
  printHighFreqClockStatus();
}

void startTimer() {
//  // Stop the timer task before setting it.
//  TIMER->TASKS_STOP = (1 << TIMER_TASKS_START_TASKS_START_Pos) & TIMER_TASKS_START_TASKS_START_Msk;
//
//  // Reset the time.
//  TIMER->TASKS_CLEAR = (1 << TIMER_TASKS_CLEAR_TASKS_CLEAR_Pos) & TIMER_TASKS_CLEAR_TASKS_CLEAR_Msk;
//
//  // When the compare triggers clear the timer and generate an interrupt.
//  TIMER->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos) & TIMER_SHORTS_COMPARE0_CLEAR_Msk;
//  TIMER->INTENSET = (TIMER_INTENSET_COMPARE0_Set << TIMER_INTENSET_COMPARE0_Pos) & TIMER_INTENSET_COMPARE0_Msk;
//
//  TIMER->MODE = (TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos) & TIMER_MODE_MODE_Msk;
//  TIMER->BITMODE = (TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos) & TIMER_BITMODE_BITMODE_Msk;
//
//  // f_timer = 16 MHz / (2^PRESCALER).
//  // The highest possible prescaler is 15 (1111 in binary), which gives ~488 Hz.
//  unsigned int prescaler = 8;
//  TIMER->PRESCALER = (prescaler << TIMER_PRESCALER_PRESCALER_Pos) & TIMER_PRESCALER_PRESCALER_Msk;
//
//  unsigned int cc = 62500;
//  TIMER->CC[0] = (cc << TIMER_CC_CC_Pos) & TIMER_CC_CC_Msk;
//
//  NVIC_EnableIRQ(TIMER4_IRQn);
//  TIMER->TASKS_START = (1 << TIMER_TASKS_START_TASKS_START_Pos) & TIMER_TASKS_START_TASKS_START_Msk;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  startClock();
//  startTimer();

  Serial.println("Starting");
}

void loop() {}

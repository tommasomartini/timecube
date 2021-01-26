volatile bool ledState = LOW;
volatile NRF_RTC_Type* TIMER = NRF_RTC2;

extern "C" {
  void RTC2_IRQHandler_v() {
    if (TIMER->EVENTS_COMPARE[0] & RTC_EVENTS_COMPARE_EVENTS_COMPARE_Msk) {
      ledState = !ledState;
      digitalWrite(LED_BUILTIN, ledState);
      TIMER->EVENTS_COMPARE[0] = 0;
      TIMER->TASKS_CLEAR = (1 << RTC_TASKS_CLEAR_TASKS_CLEAR_Pos) & RTC_TASKS_CLEAR_TASKS_CLEAR_Msk;
    }
  }
}

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

  NRF_CLOCK->LFCLKSRC = ((CLOCK_LFCLKSRC_EXTERNAL_Disabled << CLOCK_LFCLKSRC_EXTERNAL_Pos) & CLOCK_LFCLKSRC_EXTERNAL_Msk)
                      | ((CLOCK_LFCLKSRC_BYPASS_Disabled << CLOCK_LFCLKSRC_BYPASS_Pos) & CLOCK_LFCLKSRC_BYPASS_Msk)
                      | ((CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos) & CLOCK_LFCLKSRC_SRC_Msk);

  NRF_CLOCK->LFRCMODE = (CLOCK_LFRCMODE_MODE_ULP << CLOCK_LFRCMODE_MODE_Pos) & CLOCK_LFRCMODE_MODE_Msk;

  NRF_CLOCK->TASKS_LFCLKSTART = (1 << CLOCK_TASKS_LFCLKSTART_TASKS_LFCLKSTART_Pos) & CLOCK_TASKS_LFCLKSTART_TASKS_LFCLKSTART_Msk;
}

void startTimer() {
  TIMER->TASKS_STOP = (1 << RTC_TASKS_START_TASKS_START_Pos) & RTC_TASKS_START_TASKS_START_Msk;
  TIMER->TASKS_CLEAR = (1 << RTC_TASKS_CLEAR_TASKS_CLEAR_Pos) & RTC_TASKS_CLEAR_TASKS_CLEAR_Msk;

  TIMER->INTENSET = (RTC_INTENSET_COMPARE0_Set << RTC_INTENSET_COMPARE0_Pos) & RTC_INTENSET_COMPARE0_Msk;
  TIMER->EVTENSET = (RTC_EVTENSET_COMPARE0_Enabled << RTC_EVTENSET_COMPARE0_Pos) & RTC_EVTENSET_COMPARE0_Msk;

  // f_timer = 32.768 kHz / (PRESCALER + 1).
  // The highest possible prescaler is 4095 = 2^11-1 (12 bits set to 1), which gives ~488 Hz.
  unsigned int prescaler = 1023;
  TIMER->PRESCALER = (prescaler << RTC_PRESCALER_PRESCALER_Pos) & RTC_PRESCALER_PRESCALER_Msk;

  unsigned int cc = 16;
  TIMER->CC[0] = (cc << RTC_CC_COMPARE_Pos) & RTC_CC_COMPARE_Msk;

  NVIC_EnableIRQ(RTC2_IRQn);
  TIMER->TASKS_START = (1 << RTC_TASKS_START_TASKS_START_Pos) & RTC_TASKS_START_TASKS_START_Msk;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  startClock();
  printLowFreqClockStatus();
  printHighFreqClockStatus();
  startTimer();

  Serial.println("Starting");
}

void loop() {}

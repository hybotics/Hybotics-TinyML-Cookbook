#include "mbed.h"

#if defined(ARDUINO_ARDUINO_NANO33BLE)
static const PinName gpio_pin_out = p23;
static const PinName gpio_pin_in  = p21;
#endif

static mbed::InterruptIn button(gpio_pin_in);
static mbed::DigitalOut led(gpio_pin_out);

void rise_ISR() {
  led = 0;
}

void fall_ISR() {
  led = 1;
}

void setup() {
  button.mode(PullUp);
  button.rise(&rise_ISR);
  button.fall(&fall_ISR);
}

void loop() {
  delay(4000);
}

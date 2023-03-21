#include "mbed.h"

#if defined(ARDUINO_ARDUINO_NANO33BLE)
static const PinName gpio_pin_out = p23;
static const PinName gpio_pin_in  = p21;
#endif

static mbed::DigitalIn button(gpio_pin_in);
static mbed::DigitalOut led(gpio_pin_out);

void setup() {
  button.mode(PullUp);
}

void loop() {
  led = !button;
}

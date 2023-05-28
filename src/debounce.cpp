#include <Arduino.h>
#include <stdbool.h>
#include <assert.h>

#include "debounce.h"

#define DEBOUNCE_DELAY 50 // the debounce time; increase if the output flickers

uint8_t debounce_read(uint8_t pin)
{
    static unsigned long last_debounce = 0;
    static uint8_t last_pin = pin;
    static uint8_t last_read = LOW;

    uint8_t read = digitalRead(pin);

    // don't debounce read on different pin
    if (pin != last_pin)
    {
        last_debounce = millis();
        last_pin = pin;
        return last_read = read;
    }

    // prevent pressing, flickering or noise
    if (read != last_read)
    {
        last_debounce = millis();
    }

    // debounce delay elapsed, update current state
    if (millis() - last_debounce > DEBOUNCE_DELAY)
    {
        return last_read = read;
    }

    // debouncing is in progress, consider reading low
    return LOW;
}
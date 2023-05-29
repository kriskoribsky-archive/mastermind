#include <Arduino.h>
#include <stdbool.h>
#include <assert.h>

#include "debounce.h"

#define DEBOUNCE_DELAY 100

uint8_t debounce_read(uint8_t pin)
{
    static unsigned long last_debounce = 0;
    static uint8_t last_pin = pin;
    static uint8_t last_read = LOW;

    uint8_t read = digitalRead(pin);

    // reset debounce on different pin
    if (pin != last_pin)
    {
        if (read == HIGH)
        {
            last_debounce = millis();
            last_pin = pin;
            last_read = read;
            return read;
        }
        return LOW;
    }

    // prevent pressing, flickering or noise
    if (millis() - last_debounce > DEBOUNCE_DELAY && read == HIGH && last_read != HIGH)
    {
        last_debounce = millis();
        last_read = read;
        return HIGH;
    }

    last_read = read;
    return LOW;
}
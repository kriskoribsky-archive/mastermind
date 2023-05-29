#include <Arduino.h>

#include "init.h"

#include "mastermind.h"
#include "lcd.h"
#include "debounce.h"

void wait_for_high(byte pin);
void leds_red(byte state);
void leds_blue(byte state);

void arduino_init(void)
{
    init();

    // initVariant();

#if defined(USBCON)
    USBDevice.attach();
#endif
}

void circuit_init(void)
{
    // if analog input pin 1 is unconnected, random analog
    // noise will cause the call to randomSeed() to generate
    // different seed numbers each time the sketch runs.
    // randomSeed() will then shuffle the random function.
    randomSeed(analogRead(1));

    Serial.begin(9600);

    pinMode(LED_BLUE_1, OUTPUT);
    pinMode(LED_RED_1, OUTPUT);
    pinMode(LED_BLUE_2, OUTPUT);
    pinMode(LED_RED_2, OUTPUT);
    pinMode(LED_BLUE_3, OUTPUT);
    pinMode(LED_RED_3, OUTPUT);
    pinMode(LED_RED_4, OUTPUT);
    pinMode(LED_BLUE_4, OUTPUT);

    pinMode(BTN_1_PIN, INPUT);
    pinMode(BTN_2_PIN, INPUT);
    pinMode(BTN_3_PIN, INPUT);
    pinMode(BTN_4_PIN, INPUT);

    pinMode(BTN_ENTER_PIN, INPUT);
}

void run_diagnostics(void)
{
    lcd_print_at(0, 0, "Running");
    lcd_print_at(1, 0, "diagnostics...");
    delay(TEXT_DELAY);

    lcd_clear();
    lcd_print_at(0, 0, "Diagnostics:");

    // Buttons
    lcd_print_at(1, 0, "Press enter");
    wait_for_high(BTN_ENTER_PIN);

    lcd_print_at(1, 0, "Press btn 1");
    wait_for_high(BTN_1_PIN);

    lcd_print_at(1, 0, "Press btn 2");
    wait_for_high(BTN_2_PIN);

    lcd_print_at(1, 0, "Press btn 3");
    wait_for_high(BTN_3_PIN);

    lcd_print_at(1, 0, "Press btn 4");
    wait_for_high(BTN_4_PIN);

    // LEDs
    lcd_print_at(1, 0, "Are LEDs blue?");
    leds_blue(HIGH);
    wait_for_high(BTN_ENTER_PIN);
    leds_blue(LOW);

    lcd_print_at(1, 0, "Are LEDs red? ");
    leds_red(HIGH);
    wait_for_high(BTN_ENTER_PIN);
    leds_red(LOW);

    lcd_clear();
    lcd_print_at(0, 0, "Diagnostics ran");
    lcd_print_at(1, 0, "successfully...");

    delay(TEXT_DELAY);
    lcd_clear();
}

void wait_for_high(byte pin)
{
    while (debounce_read(pin) != HIGH)
        ;
}

void leds_red(byte state)
{
    digitalWrite(LED_RED_1, state);
    digitalWrite(LED_RED_2, state);
    digitalWrite(LED_RED_3, state);
    digitalWrite(LED_RED_4, state);
}

void leds_blue(byte state)
{
    digitalWrite(LED_BLUE_1, state);
    digitalWrite(LED_BLUE_2, state);
    digitalWrite(LED_BLUE_3, state);
    digitalWrite(LED_BLUE_4, state);
}
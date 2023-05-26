#include <Arduino.h>

#include "init.h"
#include "mastermind.h"
#include "lcd.h"

void wait_for_press(int btn)
{
    while (digitalRead(btn) != HIGH)
        ;
}

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
    lcd_print_at(0, 0, "Diagnostics...");

    // Buttons
    lcd_print_at(1, 0, "Press btn 1");
    wait_for_press(BTN_1_PIN);

    lcd_print_at(1, 0, "Press btn 2");
    wait_for_press(BTN_2_PIN);

    lcd_print_at(1, 0, "Press btn 3");
    wait_for_press(BTN_3_PIN);

    lcd_print_at(1, 0, "Press btn 4");
    wait_for_press(BTN_4_PIN);

    // LEDs
    digitalWrite(LED_BLUE_1, LOW);
    digitalWrite(LED_RED_1, HIGH);

    digitalWrite(LED_BLUE_2, LOW);
    digitalWrite(LED_RED_2, HIGH);

    digitalWrite(LED_BLUE_3, LOW);
    digitalWrite(LED_RED_3, HIGH);

    digitalWrite(LED_BLUE_4, LOW);
    digitalWrite(LED_RED_4, HIGH);

    lcd_print_at(1, 0, "Are LEDs red?");
    wait_for_press(BTN_ENTER_PIN);

    digitalWrite(LED_BLUE_1, HIGH);
    digitalWrite(LED_RED_1, LOW);

    digitalWrite(LED_BLUE_2, HIGH);
    digitalWrite(LED_RED_2, LOW);

    digitalWrite(LED_BLUE_3, HIGH);
    digitalWrite(LED_RED_3, LOW);

    digitalWrite(LED_BLUE_4, HIGH);
    digitalWrite(LED_RED_4, LOW);

    lcd_print_at(1, 0, "Are LEDs blue?");
    wait_for_press(BTN_ENTER_PIN);

    lcd_clear();
    lcd_print_at(0, 0, "Diagnostics ran");
    lcd_print_at(1, 0, "successfully...");
}
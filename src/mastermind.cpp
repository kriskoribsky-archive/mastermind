#include <Arduino.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "mastermind.h"

#include "lcd.h"

char *generate_code(bool repeat, int length)
{
    assert(length <= SECRET_LENGTH);

    if (length < 1 || (repeat == false && length > 10))
    {
        return NULL;
    }

    char *code = (char *)calloc(length + 1, sizeof(char));
    assert(code != NULL);

    for (int i = 0; i < length; i++)
    {
        char value;
        do
        {
            value = '0' + random(0, 9);

        } while (repeat == false && strchr(code, value) != NULL);

        code[i] = value;
    }

    Serial.println("Generated secret is:");
    Serial.println(code);

    return code;
}

void get_score(const char *secret, const char *current_guess, int *peg_a, int *peg_b)
{
    size_t length = strlen(secret);
    assert(length == strlen(current_guess));

    *peg_a = *peg_b = 0;

    for (size_t i = 0; i < length; i++)
    {
        if (secret[i] == current_guess[i])
        {
            (*peg_a)++;
        }
        else if (strchr(secret, current_guess[i]) != NULL)
        {
            (*peg_b)++;
        }
    }
}

void turn_off_leds()
{
    digitalWrite(LED_BLUE_1, LOW);
    digitalWrite(LED_RED_1, LOW);

    digitalWrite(LED_BLUE_2, LOW);
    digitalWrite(LED_RED_2, LOW);

    digitalWrite(LED_BLUE_3, LOW);
    digitalWrite(LED_RED_3, LOW);

    digitalWrite(LED_BLUE_4, LOW);
    digitalWrite(LED_RED_4, LOW);
}

void render_leds(const int peg_a, const int peg_b)
{
    assert((peg_a + peg_b) <= SECRET_LENGTH);
    assert((peg_a + peg_b) <= NUM_DIGITAL_PINS);

    turn_off_leds();

    byte pin = LED_RED_1;
    for (int i = 0; i < peg_a; i++)
    {
        digitalWrite(pin, HIGH);
        pin += 2;
    }

    pin -= 1;
    for (int i = 0; i < peg_b; i++)
    {
        digitalWrite(pin, HIGH);
        pin += 2;
    }
}

void render_history(char *secret, char **history, const int entry_nr)
{
    assert(secret != NULL);
    assert(history != NULL);

    // no entries in history
    if (entry_nr == -1)
    {
        lcd_print_at(0, 0, "Guess my code!");
        return;
    }

    assert(history[entry_nr] != NULL);
    assert(entry_nr >= 0 && entry_nr < MAX_GUESSES);

    // evaluate
    int peg_a, peg_b;
    get_score(secret, history[entry_nr], &peg_a, &peg_b);

    // render
    render_leds((const int)peg_a, (const int)peg_b);
    lcd_printf_at(0, 0, "%02d: %s %dA%dB", entry_nr + 1, peg_a, peg_b);
}

void play_game(char *secret)
{
    assert(secret != NULL);

    // intro
    lcd_print_at(0, 0, "Welcome to the");
    lcd_print_at(1, 0, "game Mastermind!");
    delay(TEXT_DELAY);
    lcd_clear();
    lcd_print_at(0, 0, "Guess my code!");

    // allocate history
    char **history = (char **)malloc(MAX_GUESSES * sizeof(char *));
    assert(history != NULL);

    for (int i = 0; i < MAX_GUESSES; i++)
    {
        history[i] = (char *)calloc(SECRET_LENGTH + 1, sizeof(char));
        assert(history[i] != NULL);
        memset(history[i], '0', SECRET_LENGTH);
    }

    int peg_a = 0, peg_b = 0;
    int current_guess = 0;
    int current_history = -1;

    // render
    lcd_clear();
    render_history(secret, history, current_history);
    lcd_printf_at(1, 0, "Your guess: %s", history[current_guess]);

    while (peg_a < SECRET_LENGTH && current_guess < MAX_GUESSES)
    {
        // process
        if (digitalRead(BTN_1_PIN) == HIGH)
        {
            bool history_changed = false;

            while (digitalRead(BTN_1_PIN) == HIGH)
            {
                if (digitalRead(BTN_2_PIN) == HIGH && current_guess > 0)
                {
                    current_history = max(0, current_history - 1); // iterate in history backwards
                    history_changed = true;
                    render_history(secret, history, current_history);
                    delay(READ_DELAY);
                }
                else if (digitalRead(BTN_3_PIN) == HIGH && current_guess > 0)
                {
                    current_history = min(current_guess - 1, current_history + 1);
                    history_changed = true;
                    render_history(secret, history, current_history);
                    delay(READ_DELAY);
                }
            }
            if (!history_changed)
            {
                history[current_guess][0] = '0' + (history[current_guess][0] - '0' + 1) % 10;
            }
        }
        else if (digitalRead(BTN_2_PIN) == HIGH)
        {
            history[current_guess][1] = '0' + (history[current_guess][1] - '0' + 1) % 10;
        }
        else if (digitalRead(BTN_3_PIN) == HIGH)
        {
            history[current_guess][2] = '0' + (history[current_guess][2] - '0' + 1) % 10;
        }
        else if (digitalRead(BTN_4_PIN) == HIGH)
        {
            history[current_guess][3] = '0' + (history[current_guess][3] - '0' + 1) % 10;
        }
        else if (digitalRead(BTN_ENTER_PIN) == HIGH)
        {
            current_history = current_guess;

            get_score(secret, history[current_guess], &peg_a, &peg_b);
            current_guess++;
        }
        else
        {
            delay(TICK_RATE);
            continue;
        }

        // render
        lcd_clear();
        render_history(secret, history, current_history);
        lcd_printf_at(1, 0, "Your guess: %s", history[current_guess]);

        // delay
        delay(READ_DELAY);
    }

    // result
    lcd_clear();
    lcd_print_at(0, 0, (char *)(current_guess < MAX_GUESSES ? "You won!" : "You lost!"));
    delay(5000);
    lcd_clear();
    turn_off_leds();

    for (int i = 0; i < MAX_GUESSES; i++)
    {
        free(history[i]);
    }
    free(history);
}

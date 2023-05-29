#include <Arduino.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "mastermind.h"

#include "lcd.h"
#include "debounce.h"

void render_intro(void);
void render_game(char *secret, char **history, const int current_guess, const int current_history);
void render_end(char *secret, const int current_guess);

#define INCREMENT(c) (c) = '0' + ((c) - '0' + 1) % 10

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
    assert(SECRET_LENGTH <= UINT8_MAX);

    for (byte i = 0, pin = LED_BLUE_1; i < SECRET_LENGTH; i++, pin += 2)
    {
        digitalWrite(pin, LOW);
        digitalWrite(pin + 1, LOW);
    }
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
    lcd_printf_at(0, 0, "%02d: %s %dA%dB", entry_nr + 1, history[entry_nr], peg_a, peg_b);
}

void play_game(char *secret)
{
    assert(secret != NULL);

    // init history
    char **history = (char **)malloc(MAX_GUESSES * sizeof(char *));
    assert(history != NULL);

    for (int i = 0; i < MAX_GUESSES; i++)
    {
        history[i] = (char *)calloc(SECRET_LENGTH + 1, sizeof(char));
        assert(history[i] != NULL);
        memset(history[i], '0', SECRET_LENGTH);
    }

    // init game state
    int peg_a = 0, peg_b = 0;
    int current_guess = 0;
    int current_history = -1;

    render_intro();
    render_game(secret, history, current_guess, current_history);

    while (peg_a < SECRET_LENGTH && current_guess < MAX_GUESSES)
    {
        // process
        if (debounce_read(BTN_1_PIN))
        {
            bool history_changed = false;

            while (digitalRead(BTN_1_PIN))
            {

                if (debounce_read(BTN_2_PIN) && current_guess > 0)
                {
                    current_history = max(0, current_history - 1); // iterate in history backwards
                    history_changed = true;
                    render_history(secret, history, current_history);
                }
                else if (debounce_read(BTN_3_PIN) && current_guess > 0)
                {
                    current_history = min(current_guess - 1, current_history + 1); // iterate in history forwards
                    history_changed = true;
                    render_history(secret, history, current_history);
                }
            }

            if (!history_changed)
            {
                INCREMENT(history[current_guess][0]);
            }
        }
        else if (debounce_read(BTN_2_PIN))
        {
            INCREMENT(history[current_guess][1]);
        }
        else if (debounce_read(BTN_3_PIN))
        {
            INCREMENT(history[current_guess][2]);
        }
        else if (debounce_read(BTN_4_PIN))
        {
            INCREMENT(history[current_guess][3]);
        }
        else if (debounce_read(BTN_ENTER_PIN))
        {
            get_score(secret, history[current_guess], &peg_a, &peg_b);
            current_history = current_guess++;
        }
        else
        {
            continue;
            delay(TICK_RATE);
        }

        // update
        render_game(secret, history, current_guess, current_history);

        // tick
        delay(TICK_RATE);
    }

    render_end(secret, current_guess);

    for (int i = 0; i < MAX_GUESSES; i++)
    {
        free(history[i]);
    }
    free(history);
}

void render_intro(void)
{
    lcd_clear();
    lcd_print_at(0, 0, "Welcome to the");
    lcd_print_at(1, 0, "game Mastermind!");
    delay(TEXT_DELAY);
}

void render_game(char *secret, char **history, const int current_guess, const int current_history)
{
    lcd_clear();
    if (current_history == -1)
    {
        lcd_print_at(0, 0, "Guess my code!");
    }
    else
    {
        render_history(secret, history, current_history);
    }

    lcd_printf_at(1, 0, "Your guess: %s", history[current_guess]);
}

void render_end(char *secret, const int current_guess)
{
    assert(SECRET_LENGTH <= UINT8_MAX);

    lcd_clear();
    turn_off_leds();

    if (current_guess < MAX_GUESSES)
    {
        lcd_print("You won!");
        lcd_printf_at(1, 0, "secret was: %s", secret);

        // won led effect
        for (byte i = 0; i < 8; i++)
        {
            for (byte j = 0, pin = LED_BLUE_1; j < SECRET_LENGTH; j++, pin += 2)
            {
                digitalWrite(pin, HIGH);
                delay(100);
            }
            turn_off_leds();
            delay(100);
        }
    }
    else
    {
        lcd_print("You lost!");
        lcd_printf_at(1, 0, "secret was: %s", secret);

        // lost led effect
        for (byte i = 0; i < 4; i++)
        {
            for (byte j = 0, pin = LED_RED_1; j < SECRET_LENGTH; j++, pin += 2)
            {
                digitalWrite(pin, HIGH);
            }
            delay(340);
            turn_off_leds();
            delay(340);
        }
    }

    turn_off_leds();
    delay(TEXT_DELAY);
    lcd_clear();
}

#include <Arduino.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "mastermind.h"

#include "helpers.h"
#include "lcd.h"

#define LCD_BUFFER_SIZE LCD_COLS + 1

char *generate_code(bool repeat, int length)
{
    assert(length <= SECRET_LENGTH);

    if (length < 1 || (repeat == false && length > 10))
    {
        return NULL;
    }

    char *code;
    CALLOC(length + 1, code);

    for (int i = 0; i < length; i++)
    {
        char value;
        do
        {
            value = random(0, 9) + '0';

        } while (repeat == false && strchr(code, value) != NULL);

        code[i] = value;
    }

    code[length] = '\0';
    return code;
}

void get_score(const char *secret, const char *guess, int *peg_a, int *peg_b)
{
    size_t length = strlen(secret);
    assert(length == strlen(guess));

    *peg_a = *peg_b = 0;

    for (size_t i = 0; i < length; i++)
    {
        if (secret[i] == guess[i])
        {
            (*peg_a)++;
        }
        else if (strchr(secret, guess[i]) != NULL)
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
    CHECK_NULL_VOID(secret);
    CHECK_NULL_VOID(history);

    assert(entry_nr >= 0 && entry_nr <= SECRET_LENGTH);

    // evaluate
    int peg_a, peg_b;
    get_score(secret, history[entry_nr], &peg_a, &peg_b);

    // render
    render_leds((const int)peg_a, (const int)peg_b);

    char buffer[LCD_BUFFER_SIZE];
    snprintf(buffer, LCD_BUFFER_SIZE, "%02d: %s %dA%dB", entry_nr + 1, history[entry_nr], peg_a, peg_b);
    lcd_print_at(0, 0, buffer);
}

void play_game(char *secret)
{
    ASSERT(secret != NULL);
    CHECK_NULL_VOID(secret);

    // intro
    lcd_print_at(0, 0, "Welcome to the Game Mastermind");
    lcd_print_at(1, 0, "game Mastermind!");
    delay(TEXT_DELAY);
    lcd_print_at(0, 0, "Guess my code:");

    int peg_a = 0;
    int peg_b = 0;
    int guess = 0;
    int entry_nr = 0;

    char *history[MAX_GUESSES];

    while (peg_a + peg_b < SECRET_LENGTH && guess < MAX_GUESSES)
    {
        // inputs
        if (digitalRead(BTN_1_PIN) == HIGH && digitalRead(BTN_2_PIN) == HIGH)
        {
            entry_nr = max(0, entry_nr - 1);
        }
        else if (digitalRead(BTN_1_PIN) == HIGH && digitalRead(BTN_3_PIN) == HIGH)
        {
            entry_nr = min(SECRET_LENGTH, entry_nr + 1);
        }
        else if (digitalRead(BTN_1_PIN) == HIGH)
        {
            history[guess][0] = '0' + (history[guess][0] - '0' + 1) % 10;
        }
        else if (digitalRead(BTN_2_PIN) == HIGH)
        {
            history[guess][1] = '0' + (history[guess][1] - '0' + 1) % 10;
        }
        else if (digitalRead(BTN_3_PIN) == HIGH)
        {
            history[guess][2] = '0' + (history[guess][2] - '0' + 1) % 10;
        }
        else if (digitalRead(BTN_4_PIN) == HIGH)
        {
            history[guess][3] = '0' + (history[guess][3] - '0' + 1) % 10;
        }
        else if (digitalRead(BTN_ENTER_PIN) == HIGH)
        {
            get_score(secret, history[guess], &peg_a, &peg_b);
            guess++;
            entry_nr = guess;
        }

        // render
        render_history(secret, history, entry_nr);
    }

    // result
    lcd_clear();
    char *win_msg = "You won !";
    char *lose_msg = "You lost!";
    lcd_print_at(0, 0, guess < MAX_GUESSES ? win_msg : lose_msg);

    for (int i = 0; i < MAX_GUESSES; i++)
    {
        FREE(history[i]);
    }
    FREE(history);
}

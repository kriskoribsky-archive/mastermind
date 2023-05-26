#include <Arduino.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "mastermind.h"

#include "helpers.h"
#include "lcd.h"

#define NUMBER_OF_GUESSES 10
#define LENGTH 4

#define LCD_BUFFER_SIZE 32

char *generate_code(bool repeat, int length)
{
    if (length < 1)
    {
        return NULL;
    }

    char *code = (char *)malloc(length * sizeof(char));
    CHECK_NULL(code);

    for (int i = 0; i < length; i++)
    {
        char value;
        do
        {
            value = random(0, 9) + '0';

        } while (repeat == false && strchr(code, value) != NULL);

        code[i] = value;
    }

    return code;
}

void get_score(const char *secret, const char *guess, int *peg_a, int *peg_b)
{
    int len = (int)strlen(secret);
    for (int i = 0; i < len; i++)
    {
        assert(guess[i] != '\0');

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
    assert((peg_a + peg_b) <= LENGTH);

    for (int pin = LED_RED_1, i = 0; i < peg_a; i++, pin += 2)
    {
        digitalWrite(pin, HIGH);
    }

    for (int pin = LED_BLUE_1, i = 0; i < peg_b; i++, pin += 2)
    {
        digitalWrite(pin, HIGH);
    }
}

void render_history(char *secret, char **history, const int entry_nr)
{
    ASSERT(secret != NULL);
    ASSERT(history != NULL);
    CHECK_NULL_VOID(secret);
    CHECK_NULL_VOID(history);

    assert(entry_nr >= 0 && entry_nr <= LENGTH);

    // evaluate
    int peg_a, peg_b;
    get_score(secret, history[entry_nr], &peg_a, &peg_b);

    // render
    turn_off_leds();
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
    lcd_clear();
    lcd_print_at(0, 0, "Game Mastermind");
    lcd_print_at(1, 0, "Guess my code!");
    delay(2000);
    lcd_clear();

    int peg_a = 0;
    int peg_b = 0;
    int guess = 0;
    int entry_nr = 0;

    char **history = (char **)malloc(NUMBER_OF_GUESSES * sizeof(char *));
    for (int i = 0; i < NUMBER_OF_GUESSES; i++)
    {
        history[i] = (char *)malloc((LENGTH + 1) * sizeof(char));
        history[i][0] = '0';
        history[i][1] = '0';
        history[i][2] = '0';
        history[i][3] = '0';
        history[i][4] = '\0';
    }

    while (peg_a + peg_b < LENGTH && guess < NUMBER_OF_GUESSES)
    {
        // inputs
        if (digitalRead(BTN_1_PIN) == HIGH && digitalRead(BTN_2_PIN) == HIGH)
        {
            entry_nr = max(0, entry_nr - 1);
        }
        else if (digitalRead(BTN_1_PIN) == HIGH && digitalRead(BTN_3_PIN) == HIGH)
        {
            entry_nr = min(LENGTH, entry_nr + 1);
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
    lcd_print_at(0, 0, guess < NUMBER_OF_GUESSES ? win_msg : lose_msg);

    for (int i = 0; i < NUMBER_OF_GUESSES; i++)
    {
        FREE(history[i]);
    }
    FREE(history);
}

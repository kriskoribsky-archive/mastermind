#include <Arduino.h>
#include <stdlib.h>

#include "init.h"
#include "lcd.h"
#include "mastermind.h"

// TODO implement unit tests

int main(void)
{
    arduino_init();
    circuit_init();
    lcd_init();

    run_diagnostics();

    for (;;)
    {
        char *code = generate_code(false, SECRET_LENGTH);
        play_game(code);
        free(code);
    }

    return EXIT_SUCCESS;
}
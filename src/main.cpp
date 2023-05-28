#include <Arduino.h>
#include <stdlib.h>

#include "init.h"
#include "mastermind.h"
#include "lcd.h"

int main(void)
{
    arduino_init();
    circuit_init();
    lcd_init();

    run_diagnostics();

    // for (;;)
    // {
    //     char *code = generate_code(false, 4);
    //     play_game(code);
    //     free(code);
    // }

    return EXIT_SUCCESS;
}
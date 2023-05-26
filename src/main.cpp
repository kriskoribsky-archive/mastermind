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


    return EXIT_SUCCESS;
}
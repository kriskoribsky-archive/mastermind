#include <Arduino.h>
#include <LCDi2c.h> // https://registry.platformio.org/libraries/sstaub/LCD%20I2C
#include <stdarg.h>

#include "lcd.h"

LCDi2c lcd(LCD_ADDR);

void lcd_init()
{
    lcd.begin(LCD_COLS, LCD_ROWS);
}

void lcd_clear()
{
    lcd.cls();
}

void lcd_set_cursor(int y, int x)
{
    lcd.locate(x, y);
}

void lcd_print(char *text)
{
    lcd.printf("%s", text);
}

void lcd_print_at(int y, int x, char *text)
{
    lcd_set_cursor(y, x);
    lcd_print(text);
}

void lcd_printf_at(int y, int x, const char *format, ...)
{
    va_list args;

    va_start(args, format);

    lcd_set_cursor(y, x);
    lcd.printf(format, args);

    va_end(args);
}
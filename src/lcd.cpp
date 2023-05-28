#include <Arduino.h>
#include <LCDi2c.h> // https://registry.platformio.org/libraries/sstaub/LCD%20I2C
#include <stdarg.h>

#include "lcd.h"

static LCDi2c lcd(LCD_ADDR); // restrict scope just for this file

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
    char buffer[LCD_COLS + 1];

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    lcd_set_cursor(y, x);
    lcd_print(buffer);
}
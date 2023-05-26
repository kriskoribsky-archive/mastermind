#include <Arduino.h>
#include <LCDi2c.h>

#include "lcd.h"

#define I2C_ADDR    0x27

#define LCD_COLS    16
#define LCD_ROWS    2

LCDi2c lcd(I2C_ADDR);

void lcd_init(){
    lcd.begin(LCD_COLS, LCD_ROWS);
}

void lcd_clear(){
    lcd.cls();
}

void lcd_set_cursor(int y, int x){
    lcd.locate(x, y);
}

void lcd_print(char* text){
    lcd.printf("%s", text);
}

void lcd_print_at(int y, int x, char* text){
    lcd_set_cursor(y, x);
    lcd_print(text);
}


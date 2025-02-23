#include "gas_monitor_config.h"
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,2);
char lcd_row1[17] = {0};
char lcd_row2[17] = {0};
unsigned long disp_millis;

bool lcd_Init( void )
{
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("INNITIALIZING...");
    return true;
}

bool lcd_setRowMsg( uint8_t row_num, char *row_msg)
{
    if(strlen(row_msg) > 16)
    {
        return false;
    }
    if(row_num == 1)
    {
        memset(lcd_row1, '\0', 17);
        sprintf(lcd_row1, "%s", row_msg);
        return true;
    }
    else if(row_num == 2)
    {
        memset(lcd_row2, '\0', 17);
        sprintf(lcd_row2, "%s", row_msg);
        return true;
    }
    
    return false;
}

void lcd_Update( void )
{
    if(millis() - disp_millis > 800)
    {
        disp_millis = millis();
        lcd.clear();
        lcd.print(lcd_row1);
        lcd.setCursor(0,1);
        lcd.print(lcd_row2);
    }
}

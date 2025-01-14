
#include "gas_monitor_config.h"
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#define DISPLAY_INTERVAL    500

LiquidCrystal_PCF8574 lcd(0x27);

unsigned long display_time;

LCD_STATE display_setup()
{
    #ifdef DEVICE_DEBUG
        Serial.println("display setup");
    #endif

    Wire.begin();
    Wire.beginTransmission(0x27);
    int error = Wire.endTransmission();

    if(error != 0)
    {
        return LCD_ERROR;
    }

    lcd.begin(16, 2);
    lcd.setBacklight(255);
    lcd.display();

    return LCD_READY;
}

void display_clear(){
    lcd.clear();
    lcd.setBacklight(0);
    lcd.noDisplay();
}


/* ----------------------------------- EOF ---------------------------------- */

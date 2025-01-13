

#include "gas_monitor_config.h"
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#define DISPLAY_INTERVAL    500

LiquidCrystal_PCF8574 lcd(0x27);

// DEVICE_PARAMS display_params;

unsigned long display_time;

DEVICE_STATE display_setup()
{
    #ifdef DEVICE_DEBUG
        Serial.println("display setup");
    #endif

    Wire.begin();
    Wire.beginTransmission(0x27);
    int error = Wire.endTransmission();

    if(error != 0)
    {
        return DEVICE_STATE_ERROR;
    }

    lcd.begin(16, 2);
    lcd.setBacklight(255);
    lcd.display();

    return DEVICE_STATE_READY;
}

void display_clear(){
    lcd.clear();
    lcd.setBacklight(0);
    lcd.noDisplay();
}

void display_update( ){
  
}


/* ----------------------------------- EOF ---------------------------------- */

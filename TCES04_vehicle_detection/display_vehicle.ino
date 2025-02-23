#include "vehicle_detection_config.h"
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#define DISP_MSG_LEN 16

LiquidCrystal_PCF8574 lcd(0x27);

char disp_line1_msg[DISP_MSG_LEN+1];
char disp_line2_msg[DISP_MSG_LEN+1];
bool disp_line1;
bool disp_line2;

unsigned long disp_millis;

bool Disp_Init( void )
{
    Wire.begin();
    Wire.beginTransmission(0x27);
    uint8_t error = Wire.endTransmission();
    
    if( error != 0)
    {
        #ifdef DEBUG_SYSTEM
            Serial.println("LCD not found");
        #endif
        return false;
    }
    
    #ifdef DEBUG_SYSTEM
        Serial.println("LCD found");
    #endif
    lcd.begin(16, 2);
    lcd.setBacklight(255);
    lcd.print("Initialising...");

    disp_millis = millis();
    disp_line1 = true;
    disp_line2 = true;

    return true;
}

bool Disp_Update( void )
{
    if(millis() - disp_millis > DISPLAY_UPDATE_INTERVAL)
    {
        disp_millis = millis();

        if(disp_line1)
        {
            lcd.setCursor(0,0);
            lcd.print( disp_line1_msg );
            disp_line1 = false;
        }

        if( disp_line2 )
        {
            lcd.setCursor(0,1);
            lcd.print( disp_line2_msg );
            disp_line2 = false;
        }
    }

    return true;
}

bool Disp_SetMessage( const char *msg, uint8_t line )
{
    if(
        ( line > 2 ) ||
        ( line < 1 ) ||
        ( msg == NULL ) ||
        (strlen( msg ) > DISP_MSG_LEN)
    )
    {
        return false;
    }

    if( line == 1 )
    {
        memset( disp_line1_msg, 0, strlen( disp_line1_msg ) );
        sprintf( disp_line1_msg, "%s", msg );
        disp_line1 = true;
    }
    else if( line == 2 )
    {
        memset( disp_line2_msg, 0, strlen( disp_line2_msg ) );
        sprintf( disp_line2_msg, "%s", msg );
        disp_line2 = true;
    }
    else
    {
        return false;
    }

    return true;
}

/* ------------------------------- END_OF_FILE ------------------------------ */
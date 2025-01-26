
#include "dripset_monitor_config.h"
#include <HX711.h>

#define FULL_BOTTLE_VAL      482
#define EMPTY_BOOTLE_VAL     315
#define MAXIMUM_VOLUME       500

HX711 scale;
unsigned long lc_millis;

SENSOR_STATES load_cell_Init(uint8_t sda_pin, uint8_t sck_pin) {
    scale.begin(sda_pin, sck_pin);
    delay(1000);
    if(scale.is_ready())
    {
        return SENSOR_STATE_READY;
    }

    return SENSOR_STATE_ERROR;
}

SENSOR_STATES load_cell_SetVolume( void )
{
    if(millis() - lc_millis > 1000)
    {
        lc_millis = millis();
        
        if (scale.is_ready()) 
        {
            long reading = scale.read()/1000;

            if(reading < EMPTY_BOOTLE_VAL)
            {
                Serial.println("Bottle is empty");
                dripset_params.drip_volume_left = 0;
            }
            else if(reading > FULL_BOTTLE_VAL)
            {
                Serial.println("Bottle is too full");
                dripset_params.drip_volume_left = MAXIMUM_VOLUME;
            }
            else
            {
                dripset_params.drip_volume_left = (reading - EMPTY_BOOTLE_VAL) * MAXIMUM_VOLUME / (FULL_BOTTLE_VAL - EMPTY_BOOTLE_VAL);
            }

            return SENSOR_STATE_READY;
        } 
        else 
        {
            return SENSOR_STATE_ERROR;
        }
    }
    
    return SENSOR_STATE_BUSY;
}



#include "dripset_monitor_config.h"
#include <HX711.h>

#define TOP_FULL_VAL      482
#define GETTING_FULL_VAL  440
#define HALF_FULL_VAL     400
#define RUNNING_LOW_VAL   340
#define EMPTY_CAN_VAL     315

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

          if(reading >= TOP_FULL_VAL)
          {
              device_params.current_weight = TOP_FULL;
          }
          else if(reading >= GETTING_FULL_VAL)
          {
              device_params.current_weight = GETTING_FULL;
          }
          else if(reading >= HALF_FULL_VAL)
          {
              device_params.current_weight = HALF_FULL;
          }
          else if(reading >= RUNNING_LOW_VAL)
          {
              device_params.current_weight = RUNNING_OUT;
          }
          else if(reading >= EMPTY_CAN_VAL)
          {
              device_params.current_weight = EMPTY_CAN;
          }
          else
          {
              device_params.current_weight = NO_CAN;
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


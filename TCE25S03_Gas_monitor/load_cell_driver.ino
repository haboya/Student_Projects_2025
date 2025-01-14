
#include "gas_monitor_config.h"
#include "HX711.h"

HX711 scale;

SENSOR_STATE load_cell_Init( uint8_t sda_pin, uint8_t sck_pin )
{
    if(scale.begin(sda_pin, sck_pin)){
      Serial.println("Before setting up the scale:");
      Serial.print("read: \t\t");
      Serial.println(scale.read());
    }
    else{
      return SENSOR_STATE_ERROR;
    }
}

SENSOR_STATE load_cell_SetVolume( void )
{
   
    unsigned long previousMillis = 0;
    unsigned long currentMillis;
    unsigned const long period = 5000;

    Serial.print("one reading:\t");
    device_params.weight = scale.get_units();
    Serial.print(device_params.weight, 1);
    // Serial.print("\t| average:\t");
    // Serial.println(scale.get_units(10), 1);

    scale.power_down();
    currentMillis = millis();
    if(currentMillis - previousMillis == period){
      scale.power_up();
      return SENSOR_STATE_READY;
    }
    else{
      return SENSOR_STATE_BUSY;
    }
}
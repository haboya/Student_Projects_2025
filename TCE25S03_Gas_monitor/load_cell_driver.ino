
#include "gas_monitor_config.h"
#include "HX711.h"

HX711 scale;

bool load_cell_Init(uint8_t sda_pin, uint8_t sck_pin) {
    scale.begin(sda_pin, sck_pin);
    Serial.println("Before setting up the scale:");
    Serial.print("Read: ");
    scale.read();
    Serial.print("read average: \t\t");
    Serial.println(scale.read_average(20));

    return true;
}

SENSOR_STATE load_cell_SetVolume( void )
{
   
    unsigned long previousMillis = 0;
    unsigned long currentMillis;
    unsigned const long period = 5000;

    device_params.weight = scale.get_units();
    // Serial.print(device_params.weight, 1);
    // Serial.print("\t| average:\t");
    // Serial.println(scale.get_units(10), 1);

    scale.power_down();
    currentMillis = millis();
    if (currentMillis - previousMillis >= period) {
        scale.power_down();
        previousMillis = currentMillis;
        scale.power_up();
        return SENSOR_STATE_READY;
    } 
    else {
        return SENSOR_STATE_BUSY; // Ensure a value is always returned
    }
}
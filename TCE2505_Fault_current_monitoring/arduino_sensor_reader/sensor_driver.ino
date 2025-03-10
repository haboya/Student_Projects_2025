
#include "arduino_sensor_config.h"


CURRENT_SESNOR sensor_bus87, sensor_bus14, sensor_bus16, sensor_bus22, sensor_bus24;

int current_sensor_getMaxValue( CURRENT_SESNOR *sensor )
{
    int max_value = sensor->max_val[0];
    for(int i = 1; i < SENSOR_SAMPLES; i++) {
        if(sensor->max_val[i] > max_value) {
            max_value = sensor->max_val[i];
        }
    }
    return max_value;
}

bool current_sensor_init( CURRENT_SESNOR *sensor, uint8_t sensor_pin )
{
    //confirm that sensor pin is valid anoalog pin
    if(sensor_pin < A0 || sensor_pin > A5) {
        return false;
    }
    sensor->ana_pin = sensor_pin;
    pinMode( sensor->ana_pin, INPUT );

    for(int i=0; i<SENSOR_SAMPLES; i++)
    {
        sensor->max_val[i] = 0;
    }
    sensor->old_val = 0;
    sensor->val_index = 0;
    sensor->IRMS = 0;

    return true;
}

bool current_sensor_read( CURRENT_SESNOR *sensor )
{
    int new_val = analogRead(sensor->ana_pin);
    if(new_val > sensor->old_val)
    {
        sensor->old_val = new_val;
    }
    else
    {
        delayMicroseconds(50);
        new_val = analogRead(sensor->ana_pin);
        if(new_val < sensor->old_val) 
        {
            sensor->max_val[sensor->val_index] = sensor->old_val;
            sensor->val_index = (sensor->val_index + 1) % SENSOR_SAMPLES;
            sensor->old_val = 0;
        }
        
        float rms = current_sensor_getMaxValue(sensor) * 5.00 * 0.707 / 1024.0;
        sensor->IRMS = rms - CALIBRATION_THRESHOLD;
        if(sensor->IRMS < 0.08) sensor->IRMS = 0.0;
        sensor->IRMS = sensor->IRMS * 10.0;

        return true;
    }
    return false;
}

bool Sensors_Init( void )
{
    for(int i = 0; i < SENSOR_COUNT; i++)
    {
        switch(i)
        {
            case 0:
            {
                if(!current_sensor_init(&sensor_bus87, SENSOR_BUS87_PIN))
                {
                    #ifdef DEBUG_SENSOR_SYSTEM
                        Serial.println("Sensor bus87 failed to initialize");
                    #endif
                    return false;
                }
                sensor_bus87.threshold = 18.1;
                break;
            }

            case 1:
            {
                if(!current_sensor_init(&sensor_bus14, SENSOR_BUS14_PIN))
                {
                    #ifdef DEBUG_SENSOR_SYSTEM
                        Serial.println("Sensor bus14 failed to initialize");
                    #endif
                    return false;
                }
                sensor_bus14.threshold = 14.6;
                break;
            }

            case 2:
            {
                if(!current_sensor_init(&sensor_bus16, SENSOR_BUS16_PIN))
                {
                    #ifdef DEBUG_SENSOR_SYSTEM
                        Serial.println("Sensor bus16 failed to initialize");
                    #endif
                    return false;
                }
                sensor_bus16.threshold = 17.7;
                break;
            }

            case 3:
            {
                if(!current_sensor_init(&sensor_bus22, SENSOR_BUS22_PIN))
                {
                    #ifdef DEBUG_SENSOR_SYSTEM
                        Serial.println("Sensor bus22 failed to initialize");
                    #endif
                    return false;
                }
                sensor_bus22.threshold = 14.1;
                break;
            }

            case 4:
            {
                if(!current_sensor_init(&sensor_bus24, SENSOR_BUS24_PIN))
                {
                    #ifdef DEBUG_SENSOR_SYSTEM
                        Serial.println("Sensor bus24 failed to initialize");
                    #endif
                    return false;
                }
                sensor_bus24.threshold = 18.7;
                break;
            }

            default:
                return false;
        }
    }
    return true;
}

void Sensors_Read( void )
{
    static uint8_t sensor_index = 0;
    switch(sensor_index)
    {
        case 0:
        {
            if(current_sensor_read(&sensor_bus87))
            {
                sensor_currents.bus87 = sensor_bus87.IRMS;
            }
        }
        break;

        case 1:
        {
            if(current_sensor_read(&sensor_bus14))
            {
                sensor_currents.bus14 = sensor_bus14.IRMS;
            }
        }
        break;

        case 2:
        {
            if(current_sensor_read(&sensor_bus16))
            {
                sensor_currents.bus16 = sensor_bus16.IRMS;
            }
        }
        break;

        case 3:
        {
            if(current_sensor_read(&sensor_bus22))
            {
                sensor_currents.bus22 = sensor_bus22.IRMS;
            }
        }
        break;

        case 4:
        {
            if(current_sensor_read(&sensor_bus24))
            {
                sensor_currents.bus24 = sensor_bus24.IRMS;
            }
        }
        break;

        default:
            break;
    }

    sensor_index = (sensor_index + 1) % SENSOR_COUNT;
    
    // #ifdef DEBUG_SENSOR_SYSTEM
    //     if(sensor_index == 0)
    //     {
    //         Serial.println("Current Values: ");
    //         Serial.print(sensor_currents.bus87); Serial.print(",");
    //         Serial.print(sensor_currents.bus14); Serial.print(",");
    //         Serial.print(sensor_currents.bus16); Serial.print(",");
    //         Serial.print(sensor_currents.bus22); Serial.print(",");
    //         Serial.print(sensor_currents.bus24); Serial.print("\n");
    //     }
    // #endif
}

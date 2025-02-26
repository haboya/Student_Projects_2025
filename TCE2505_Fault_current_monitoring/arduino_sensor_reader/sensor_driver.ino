
#include "arduino_sensor_config.h"

#define CALIBRATION_THRESHOLD   1.947

typedef struct CURRENT_SESNOR {
    uint8_t ana_pin;
    int max_val;
    int new_val;
    int old_val;
    float rms;
    float IRMS;
} CURRENT_SESNOR;

CURRENT_SESNOR sensor_bus87, sensor_bus14, sensor_bus16, sensor_bus22, sensor_bus24;

bool current_sensor_init( CURRENT_SESNOR *sensor, uint8_t sensor_pin )
{
    sensor->max_val = 0;
    sensor->new_val = 0;
    sensor->old_val = 0;
    sensor->rms = 0;
    sensor->IRMS = 0;

    //confirm that sensor pin is valid anoalog pin
    if(sensor_pin < A0 || sensor_pin > A5) {
        return false;
    }
    sensor->ana_pin = sensor_pin;
    pinMode( sensor->ana_pin, INPUT );

    return true;
}

bool current_sensor_read( CURRENT_SESNOR *sensor )
{
    sensor->new_val = analogRead(sensor->ana_pin);
    if(sensor->new_val > sensor->old_val)
    {
        sensor->old_val = sensor->new_val;
    }
    else
    {
        delayMicroseconds(50);
        sensor->new_val = analogRead(sensor->ana_pin);
        if(sensor->new_val < sensor->old_val) 
        {
            sensor->max_val = sensor->old_val;
            sensor->old_val = 0;
        }
        
        sensor->rms = sensor->max_val * 5.00 * 0.707 / 1024.0;
        sensor->IRMS = sensor->rms - CALIBRATION_THRESHOLD;

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
            break;
        }

        case 1:
        {
            if(current_sensor_read(&sensor_bus14))
            {
                sensor_currents.bus14 = sensor_bus14.IRMS;
            }
            break;
        }

        case 2:
        {
            if(current_sensor_read(&sensor_bus16))
            {
                sensor_currents.bus16 = sensor_bus16.IRMS;
            }
            break;
        }

        case 3:
        {
            if(current_sensor_read(&sensor_bus22))
            {
                sensor_currents.bus22 = sensor_bus22.IRMS;
            }
            break;
        }

        case 4:
        {
            if(current_sensor_read(&sensor_bus24))
            {
                sensor_currents.bus24 = sensor_bus24.IRMS;
            }
            break;
        }

        default:
            break;
    }

    sensor_index = (sensor_index + 1) % SENSOR_COUNT;
}

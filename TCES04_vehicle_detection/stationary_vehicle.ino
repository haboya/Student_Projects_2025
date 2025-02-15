#include "vehicle_detection_config.h"

uint8_t IR_sensor_pins[VEHICLE_TOTAL_IR_SENSORS];

bool Stationary_Init( uint8_t sensor_pins[] )
{
    for(int pin=0; pin<VEHICLE_TOTAL_IR_SENSORS; pin++)
    {
        if(sensor_pins[pin] < PIN_A6) return false;

        IR_sensor_pins[pin] = sensor_pins[pin];
        pinMode(IR_sensor_pins[pin], INPUT);
    }

    return true;
}

uint8_t Stationary_GetVehicleDistance( void )
{
    uint8_t ir_distance = 0;
    for(int pin=0; pin<VEHICLE_TOTAL_IR_SENSORS; pin++)
    {
        if( digitalRead(IR_sensor_pins[pin]) == false )
        {
            ir_distance = pin + 1;
            break;
        }
    }
    
    if(ir_distance > 0)
    {
        ir_distance -= 1;
        ir_distance = (ir_distance * VEHICLE_DISTANCE_IN_SENSORS) + VEHICLE_DISTANCE_TO_SENSORS;
    }

    return ir_distance;
}

/* ----------------------------------- eof ---------------------------------- */
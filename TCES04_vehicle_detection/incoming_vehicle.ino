#include "vehicle_detection_config.h"
#include <NewPing.h>

NewPing Inc_vehicle( VEHICLE_SENSOR_TRIG, VEHICLE_SENSOR_ECHO, VEHICLE_MAXIMUM_DISTANCE);
uint8_t Inc_distance[VEHICLE_SENSOR_SAMPLES];
uint8_t Inc_ping_index;

bool Incoming_Init( void )
{
    for(int i=0; i<VEHICLE_SENSOR_SAMPLES; i++)
    {
        Inc_distance[i] = Inc_vehicle.ping_cm();
    }

    Inc_ping_index = 0;

    return true;
}

uint8_t Incoming_GetVehicleDistance( void )
{
    Inc_distance[Inc_ping_index] = Inc_vehicle.ping_cm();
    Inc_ping_index = (Inc_ping_index + 1) % VEHICLE_SENSOR_SAMPLES;

    uint16_t inc_average_cm = 0;
    for(int i=0; i<VEHICLE_SENSOR_SAMPLES; i++)
    {
        inc_average_cm += Inc_distance[i];
    }

    return (uint8_t)(inc_average_cm/VEHICLE_SENSOR_SAMPLES);
}

/* ------------------------------- END_OF_FILE ------------------------------ */
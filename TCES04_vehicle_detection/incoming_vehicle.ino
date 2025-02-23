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
    #ifdef DEBUG_SYSTEM
        Serial.print("Ultrasonic Ready: ");
        Serial.println(Inc_distance[VEHICLE_SENSOR_SAMPLES-1]);
    #endif

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

    inc_average_cm = inc_average_cm/VEHICLE_SENSOR_SAMPLES;
    if(inc_average_cm > 5)
    {
        return (inc_average_cm - 5);
    }
    else if(inc_average_cm > 0)
    {
        return 1;
    }
    return 0;
}

/* ------------------------------- END_OF_FILE ------------------------------ */

#include "gas_monitor_config.h"

#define NORMAL_LOW          90
#define NORMAL_HIGH         150
#define TRIGGER_THRESHOLD   200
#define SENSOR_SAMPLES      10

uint8_t gs_pin = A0;
uint8_t gs_index = 0;
int gas_values[SENSOR_SAMPLES];
SENSOR_STATES gas_sensor_Init(uint8_t sensor_pin)
{
    gs_pin = sensor_pin;
    for(int i=0; i<SENSOR_SAMPLES; i++)
    {
        gas_values[i] = analogRead(gs_pin);
    }
    return gas_sensor_GetState();
}

SENSOR_STATES gas_sensor_GetState(void)
{
    gas_values[gs_index] = analogRead(gs_pin);
    gs_index = (gs_index+1) % SENSOR_SAMPLES;
    
    int average = 0;
    for(int i=0; i<SENSOR_SAMPLES; i++)
    {
        average += gas_values[i];
    }

    average /= SENSOR_SAMPLES;

    if(average >= TRIGGER_THRESHOLD)
    {
        device_params.gas_status = true;
        return SENSOR_STATE_ERROR;
    }
    else if((average > NORMAL_HIGH) && (device_params.gas_status == true))
    {
        return SENSOR_STATE_BUSY;
    }
    else if(average >= NORMAL_LOW)
    {
        device_params.gas_status = false;
        return SENSOR_STATE_READY;
    }

    return SENSOR_STATE_BUSY;
}

#include "dripset_monitor_config.h"



SENSOR_STATE load_cell_Init( uint8_t sda_pin, uint8_t sck_pin )
{
    return SENSOR_STATE_ERROR;
}

SENSOR_STATE load_cell_SetVolume( void )
{
    // This function should be non blocking, it should return BUSY in case conversion is still ongoing
    return SENSOR_STATE_READY;
}
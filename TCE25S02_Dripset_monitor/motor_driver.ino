
#include "dripset_monitor_config.h"
#include <Servo.h>

#define CLOSED_POSITION 90
#define OPENED_POSITION 180

Servo flow_motor;

void motor_Init( uint8_t motor_pin )
{
    flow_motor.attach(motor_pin);
}

bool motor_OpenFlow( void )
{
    int curr_position = flow_motor.read();
    if(curr_position >= OPENED_POSITION) return true;

    if(curr_position < OPENED_POSITION)
    {
        flow_motor.write(curr_position + 1);
    }

    return false;
}

bool motor_CloseFlow( void )
{
    int curr_position = flow_motor.read();
    if(curr_position <= CLOSED_POSITION) return true;

    if(curr_position > CLOSED_POSITION)
    {
        flow_motor.write(curr_position - 1);
    }

    return false;
}
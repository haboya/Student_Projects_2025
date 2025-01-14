
#include "gas_monitor_config.h"
#include <Servo.h>

#define CLOSED_POSITION 90
#define OPENED_POSITION 180

Servo gas_motor;

void motor_Init( uint8_t motor_pin )
{
    gas_motor.attach(motor_pin);
}

bool motor_OpenFlow( void )
{
    int curr_position = gas_motor.read();
    if(curr_position >= OPENED_POSITION) return true;

    if(curr_position < OPENED_POSITION)
    {
        gas_motor.write(curr_position + 1);
    }

    return false;
}

bool motor_CloseFlow( void )
{
    int curr_position = gas_motor.read();
    if(curr_position <= CLOSED_POSITION) return true;

    if(curr_position > CLOSED_POSITION)
    {
        lcd.setCursor(0,1);
        lcd.print("TURNING GAS KNOB");
        gas_motor.write(curr_position - 1);
    }

    return false;
}
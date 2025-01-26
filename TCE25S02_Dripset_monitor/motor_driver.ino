
#include "dripset_monitor_config.h"
#include <Servo.h>

#define CLOSED_POSITION 90
#define OPENED_POSITION 180

Servo drip_motor;

void motor_Init(uint8_t motor_pin)
{
    drip_motor.attach(motor_pin);
    motor_CloseFlow();
}

void motor_CloseFlow( void )
{
    int curr_position = drip_motor.read();
    for(curr_position; curr_position>=CLOSED_POSITION; curr_position--){
      drip_motor.write(curr_position);
      delay(15);
    }
    dripset_params.drip_running = false;
}

void motor_OpenFlow( void )
{
    int curr_position = drip_motor.read();
    for(curr_position; curr_position<=OPENED_POSITION; curr_position++){
      drip_motor.write(curr_position);
      delay(15);
    }

    dripset_params.drip_running = true;
}

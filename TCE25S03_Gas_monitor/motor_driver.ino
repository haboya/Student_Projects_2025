
#include "gas_monitor_config.h"
#include <Servo.h>

#define CLOSED_POSITION 90
#define OPENED_POSITION 180

Servo gas_motor;

void motor_Init(uint8_t motor_pin){
    gas_motor.attach(motor_pin);
    motor_CloseFlow();
}

bool read_gas_hose_status(){
    int curr_position = gas_motor.read();
    if(curr_position >= OPENED_POSITION){
      return false;
    }
    else{
      return true;
    }
}

void motor_CloseFlow( void ){
    int curr_position = gas_motor.read();
    for(curr_position; curr_position>=CLOSED_POSITION; curr_position--){
      gas_motor.write(curr_position);
      delay(15);
    }
    device_params.motor_status = true;
}

void motor_OpenFlow( void ){
    int curr_position = gas_motor.read();
    for(curr_position; curr_position<=OPENED_POSITION; curr_position++){
      gas_motor.write(curr_position);
      delay(15);
    }

    device_params.motor_status = false;
}

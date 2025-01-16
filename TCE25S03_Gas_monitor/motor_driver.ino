
#include "gas_monitor_config.h"
#include <Servo.h>

#define CLOSED_POSITION 90
#define OPENED_POSITION 180

Servo gas_motor;

void motor_Init(uint8_t motor_pin){
    gas_motor.attach(motor_pin);
    Serial.println("motor: ");
    gas_motor.write(OPENED_POSITION);
}

bool read_gas_hose_status(){
    int curr_position = gas_motor.read();
    Serial.println("current motor position: ");
    Serial.print(curr_position);
    if(curr_position >= OPENED_POSITION){
      return false;
    }
    else{
      return true;
    }
}

void motor_CloseFlow( void ){
      int curr_position = gas_motor.read();
      for(int i=0; i<=CLOSED_POSITION; I++){
        gas_motor.write(curr_position - 1);
      }
}

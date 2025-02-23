
#include "dripset_monitor_config.h"

volatile uint8_t dropCount;
unsigned long lastMillis = 0;
// bool watch;

bool infrared_Init( uint8_t sensor_pin )
{
    pinMode(sensor_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(sensor_pin), drops_count, RISING);
    Serial.println("infrared");
    return true;
}

SENSOR_STATE infrared_SetRate( void )
{
      unsigned long currentMillis = millis();
      
      if (currentMillis - lastMillis >= 1000){
        noInterrupts();
        dripset_params.drip_flow_rate = dropCount;
        dropCount = 0;
        interrupts();

        Serial.print("Drops per second: ");
        Serial.println(dripset_params.drip_flow_rate);

        lastMillis = currentMillis;
       
      }
      // else{
      //   return SENSOR_STATE_BUSY;
      // }
       return SENSOR_STATE_READY;
}

ICACHE_RAM_ATTR void drops_count(){
  dropCount++;
  Serial.println(dropCount);
}
/* ----------------------------------- EOF ---------------------------------- */
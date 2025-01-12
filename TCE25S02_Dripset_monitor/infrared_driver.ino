
#include "dripset_monitor_config.h"

volatile uint8_t dropCount;
bool watch;

SENSOR_STATE infrared_Init( uint8_t sensor_pin )
{
    pinMode(sensor_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(sensor_pin), drops_count, FALLING);
    watch = true;
}

SENSOR_STATE infrared_SetRate( void )
{
    if(watch){
      unsigned long currentMillis = millis();
      
      // Calculate drops per minute every interval, 60000 is the equivqlent of a min in millis
      if (currentMillis - lastMillis >= 10000) {
        noInterrupts(); //turn interrupts off for the moment
        int drip_flow_rate = dropCount * 6;
        dropCount = 0;
        interrupts();

        Serial.print("Drops per minute: ");
        Serial.println(drip_flow_rate);

        lastMillis = currentMillis;
      }
      
      return SENSOR_STATE_READY;
    }

    else{
      watch = false;
      return SENSOR_STATE_ERROR;
    }
}

ICACHE_RAM_ATTR drops_count(){
  dropCount++;
}
/* ----------------------------------- EOF ---------------------------------- */
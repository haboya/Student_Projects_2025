
#include "dripset_monitor_config.h"

#define DEBUG_DROP_SENSOR
#define DROP_WAIT
#define DROP_MINUTE_mS      60000 // Milliseconds in a single minute
#define DROP_DEBOUNCE       50    // milliseconds to wait before checking for more drops
#define DROP_SAMPLES        3     // number of drops to average
#define DROP_MIN_DESIGNED   6     // smallest number of drops the system can measure

volatile uint8_t dropCount;
bool watch;

unsigned long drop_millis[DROP_SAMPLES+1];

void ICACHE_RAM_ATTR drops_count()
{
    if (millis() - drop_millis[DROP_SAMPLES] > DROP_DEBOUNCE)
    {
        dropCount++;
        #ifdef DEBUG_DROP_SENSOR
        Serial.print("Drop: ");
        Serial.println(dropCount);
        #endif
    
        for(int i = 0; i < DROP_SAMPLES; i++)
        {
            drop_millis[i] = drop_millis[i+1];
        }
        drop_millis[DROP_SAMPLES] = millis();
    }
}

SENSOR_STATES infrared_Init( uint8_t sensor_pin )
{
    pinMode(sensor_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(sensor_pin), drops_count, FALLING);
    
    return SENSOR_STATE_READY;
}

SENSOR_STATES infrared_SetRate( void )
{
    if(millis() - drop_millis[DROP_SAMPLES] > (DROP_MINUTE_mS/DROP_MIN_DESIGNED))
    {
        dripset_params.drip_flow_rate = 0;
    }
    else
    {
        //get drop times
        unsigned long drop_time = 0;;
        for(int i=0; i<DROP_SAMPLES; i++)
        {
          drop_time += (drop_millis[i+1] - drop_millis[i]);
        }
        dripset_params.drip_flow_rate = drop_time/DROP_SAMPLES;
    }

    return SENSOR_STATE_READY;
}

/* ----------------------------------- EOF ---------------------------------- */
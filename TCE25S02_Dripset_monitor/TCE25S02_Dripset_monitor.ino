
#include "dripset_monitor_config.h"

#define BUZZER D7
#define NOTIFICATION_INTERVAL_SECS  1

DRIPSET_STATE dripset_state;
DRIPSET_PARAMS dripset_params;

void beepBuzzer( uint8_t buzzer_rate_500ms)
{
    static unsigned long buzzer_millis = 0;
    if(millis() - buzzer_millis > buzzer_rate_500ms*500)
    {
        tone(BUZZER, 4000, 150);
        buzzer_millis = millis();
    }
}

void setup()
{
    pinMode(BUZZER, OUTPUT);
    
    dripset_params.drip_flow_rate = 0;
    dripset_params.drip_running = false;
    dripset_params.drip_volume_left = 0;
    memset(dripset_params.drip_msg, '\0', DRIPSET_MONITOR_STRING_LEN);

    Serial.begin(115200);
    dripset_state = DRIP_STATE_OFF;
}

void loop()
{
    switch( dripset_state)
    {
        case DRIP_STATE_OFF:
        {
            beepBuzzer(6);
            break;
        }

        case DRIP_STATE_DRIPPING:
        {
            beepBuzzer(6);
            break;
        }

        case DRIP_STATE_STOPPED:
        {
            beepBuzzer(3);
            break;
        }

        case DRIP_STATE_ERROR:
        {
            beepBuzzer(1);
            break;
        }
    }
}

#include "dripset_monitor_config.h"

#define BUZZER_PIN                  D7
#define INFRARED_PIN                D6
#define LOAD_CELL_SCK_PIN           D2
#define LOAD_CELL_SDA_PIN           D1
#define NOTIFICATION_INTERVAL_SECS  1

#define CUTOFF_VOLUME               100
#define MINIMUM_FLOW_RATE           10
#define MAXIMUM_FLOW_RATE           30

DRIPSET_STATE dripset_state;
DRIPSET_PARAMS dripset_params;
SENSOR_STATE sensor_state;

void beepBuzzer( uint8_t buzzer_rate_500ms)
{
    static unsigned long buzzer_millis = 0;
    if(millis() - buzzer_millis > buzzer_rate_500ms*500)
    {
        tone(BUZZER_PIN, 4000, 150);
        buzzer_millis = millis();
    }
}

void setup()
{
    Serial.begin(115200);

    //reset all variable parameters
    dripset_params.drip_flow_rate = 0;
    dripset_params.drip_running = false;
    dripset_params.drip_volume_left = 0;
    memset(dripset_params.drip_msg, '\0', DRIPSET_MONITOR_STRING_LEN);
    
    //init buzzer
    pinMode(BUZZER_PIN, OUTPUT);
    SENSOR_STATE infrared_state = SENSOR_STATE_BUSY;

    do{
        infrared_state = infrared_Init(INFRARED_PIN);
    }
    while(infrared_state == SENSOR_STATE_BUSY);

    SENSOR_STATE loadcell_state = SENSOR_STATE_BUSY;
    do{
        loadcell_state = load_cell_Init(LOAD_CELL_SDA_PIN,LOAD_CELL_SCK_PIN);
    }
    while(loadcell_state == SENSOR_STATE_BUSY);

    if(infrared_state == SENSOR_STATE_ERROR || loadcell_state == SENSOR_STATE_ERROR)
    {
        dripset_state = DRIP_STATE_ERROR;
    }
    else
    {
        dripset_state  = DRIP_STATE_OFF;
    }

}

void loop()
{
    if((load_cell_SetVolume() == SENSOR_STATE_ERROR) || (infrared_SetRate() == SENSOR_STATE_ERROR))
    {
        dripset_state = DRIP_STATE_ERROR;
    }

    switch( dripset_state)
    {
        case DRIP_STATE_OFF:
        {
            motor_OpenFlow();
            beepBuzzer(6);
            if(dripset_params.drip_flow_rate > 0)
            {
                dripset_state = DRIP_STATE_DRIPPING;
            }
            break;
        }

        case DRIP_STATE_DRIPPING:
        {
            if(dripset_params.drip_flow_rate == 0)
            {
                dripset_state = DRIP_STATE_OFF;
            }

            motor_OpenFlow();
            if( (dripset_params.drip_flow_rate < MINIMUM_FLOW_RATE) || 
                (dripset_params.drip_flow_rate > MAXIMUM_FLOW_RATE) ||
                (dripset_params.drip_volume_left <= CUTOFF_VOLUME)
            )
            {
                dripset_state = DRIP_STATE_STOPPED;
            }

            beepBuzzer(20);
            break;
        }

        case DRIP_STATE_STOPPED:
        {
            motor_CloseFlow();
            beepBuzzer(2);
            break;
        }

        case DRIP_STATE_ERROR:
        {
            motor_CloseFlow();
            if((load_cell_SetVolume() != SENSOR_STATE_ERROR) && (infrared_SetRate() != SENSOR_STATE_ERROR))
            {
                dripset_state = DRIP_STATE_OFF;
            }
            beepBuzzer(1);
            break;
        }
    }
}
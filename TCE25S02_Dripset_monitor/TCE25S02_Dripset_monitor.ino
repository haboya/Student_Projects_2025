
#include "dripset_monitor_config.h"

DRIPSET_STATE dripset_state;
DRIPSET_PARAMS dripset_params;

void setup()
{
    dripset_params.drip_flow_rate = 0;
    dripset_params.drip_running = false;
    dripset_params.drip_volume_left = 0;
    memset(dripset_params.drip_msg, '\0', DRIPSET_MONITOR_STRING_LEN);
    
    Serial.begin(115200);
    dripset_state = DRIPSET_STATE_OFF;
}

void loop()
{
    //
}
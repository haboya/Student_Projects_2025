
#include "dripset_monitor_config.h"

DRIPSET_STATES dripset_state;

void setup()
{
    Serial.begin(115200);
    dripset_state = DRIPSET_STATE_OFF;
}

void loop()
{
    //
}
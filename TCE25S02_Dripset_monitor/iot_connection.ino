
#include "dripset_monitor_config.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

const char* ssid = "PowerUPLab";
const char* password = "#powerup2"; 

BlynkTimer timer;

bool iot_data_sent = false;

BLYNK_WRITE(V3)
{
    dripset_params.cutoff_volume = param.asInt();
}

BLYNK_WRITE(V4)
{
    dripset_params.minimum_flow_rate = param.asInt();
}

BLYNK_WRITE(V5)
{
    dripset_params.maximum_flow_rate = param.asInt();
}

void iot_sendData( void )
{
    Blynk.virtualWrite(V0, dripset_params.drip_running);
    Blynk.virtualWrite(V1, dripset_params.drip_flow_rate);
    Blynk.virtualWrite(V2, dripset_params.drip_volume_left);

    iot_data_sent = true;
}

bool iot_Init( void )
{
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
    timer.setInterval(1000L, iot_sendData);

    return true;
}

bool iot_UpdateStatus( void )
{
    Blynk.run();
    timer.run();

    if(iot_data_sent != false)
    {
        iot_data_sent = false;
        return true;
    }

    return false;
}


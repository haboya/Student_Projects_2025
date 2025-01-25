
#include "dripset_monitor_config.h"
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

const char* ssid = "PowerUPLab";
const char* password = "#powerup2"; 

WiFiClient  client;

unsigned long myChannelNumber = 1675288;
const char * myWriteAPIKey = "69WWTWVWTZE1YEG0";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

bool iot_connected = false;

bool iot_Init( void )
{
    WiFi.mode(WIFI_STA);   
    ThingSpeak.begin(client);  // Initialize ThingSpeak

    return true;
}

bool iot_UpdateStatus( void )
{
    static unsigned long prev_send_millis = 0;
    if(millis() - prev_send_millis < DRIPSET_IOT_UPDATE_INTERVAL*1000) return false;

    // reconnect to wifi if disconnected
    if(WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid, password);
        prev_send_millis = millis();
        return false;
    }

    // set the fields with the values
    ThingSpeak.setField(1, dripset_params.drip_flow_rate);
    ThingSpeak.setField(2, dripset_params.drip_volume_left);
    ThingSpeak.setField(3, dripset_params.drip_running);
    
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }

    prev_send_millis = millis();
    return true;
}
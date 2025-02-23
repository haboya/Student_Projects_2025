
#include "gas_monitor_config.h"
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define IOT_UPDATE_INTERVAL 6
const char* ssid = "boffins_mobile";
const char* password = "Boffins@13"; 

WiFiClient  client;

unsigned long myChannelNumber = 2820353;
const char * myWriteAPIKey = "559GFXZAZ2IRLU9C";

DEVICE_PARAMS iot_params;

bool iot_Init( void )
{
    WiFi.mode(WIFI_STA);   
    ThingSpeak.begin(client);  // Initialize ThingSpeak

    delay(3000);
    if(WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid, password);
        return false;
    }

    return true;
}

bool iot_UpdateStatus( void )
{
    static unsigned long prev_send_millis = 0;
    bool new_data = false;

    if(millis() - prev_send_millis < IOT_UPDATE_INTERVAL*1000) return false;

    // reconnect to wifi if disconnected
    if(WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi disconnected. Reconnecting...");
        WiFi.begin(ssid, password);
        prev_send_millis = millis();
        return false;
    }
    
    // if(iot_params.current_weight != device_params.current_weight)
    // {
    //     iot_params.current_weight = device_params.current_weight;
    //     new_data = true;
    // }
    // else if(iot_params.gas_status != device_params.gas_status)
    // {
    //     iot_params.gas_status = device_params.gas_status;
    //     new_data = true;
    // }
    // else if(iot_params.motor_status != device_params.motor_status)
    // {
    //     iot_params.motor_status = device_params.motor_status;
    //     new_data = true;
    // }
    
    // if(new_data)
    // {
        Serial.println("Updating ThingSpeak channel...");
        // set the fields with the values
        ThingSpeak.setField(1, device_params.current_weight); //volume
        ThingSpeak.setField(2, device_params.gas_status); //leakage
        ThingSpeak.setField(3, device_params.motor_status);    //running
        
        int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

        if(x == 200){
        Serial.println("Channel update successful.");
        }
        else{
        Serial.println("Problem updating channel. HTTP error code " + String(x));
        }
    // }

    prev_send_millis = millis();
    return true;
}


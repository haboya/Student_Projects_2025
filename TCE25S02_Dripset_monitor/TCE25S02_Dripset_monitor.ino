
#define BLYNK_TEMPLATE_ID "TMPL2C2GdsF4U"
#define BLYNK_TEMPLATE_NAME "DRIP MONITOR"
#define BLYNK_AUTH_TOKEN "yokyjZh4Ol7eLta3Uonxbu2VpMRV91Wf"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "dripset_monitor_config.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DEBUG_DEVICE_SATES 
#define MOTOR_PIN                   D5
#define BUZZER_PIN                  D7
#define INFRARED_PIN                D6
#define LOAD_CELL_SCK_PIN           D3
#define LOAD_CELL_SDA_PIN           D4
#define NOTIFICATION_INTERVAL_SECS  1

#define CUTOFF_VOLUME               100
#define MINIMUM_FLOW_RATE           10
#define MAXIMUM_FLOW_RATE           30

DRIPSET_STATES dripset_state;
DRIPSET_PARAMS dripset_params;
SENSOR_STATES sensor_state;

/// @brief Iot Connection stuff moved here due to compiler conflicts failing to compile
const char* ssid = "PowerUPLab";
const char* password = "#powerup2"; 

LiquidCrystal_I2C lcd(0x27,16,2);

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
/* -------------------------- iot connection stuff -------------------------- */

void beepBuzzer( uint8_t buzzer_rate_500ms)
{
    static unsigned long buzzer_millis = 0;
    if(millis() - buzzer_millis > buzzer_rate_500ms*500)
    {
        tone(BUZZER_PIN, 4000, 150);
        buzzer_millis = millis();
    }
}

bool setDripState( DRIPSET_STATES state)
{
    if(dripset_state != state)
    {
        #ifdef DEBUG_DEVICE_SATES
        Serial.print("Device state changed from ");
        Serial.print(dripset_state);
        Serial.print(" to ");
        Serial.println(state);
        #endif
        dripset_state = state;
        return true;
    }
    return false;
}

void setup()
{
    Serial.begin(115200);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("INNITIALIZING....");

    //reset all variable parameters
    dripset_params.cutoff_volume = CUTOFF_VOLUME;
    dripset_params.minimum_flow_rate = MINIMUM_FLOW_RATE;
    dripset_params.maximum_flow_rate = MAXIMUM_FLOW_RATE;
    dripset_params.drip_flow_rate = 0;
    dripset_params.drip_running = false;
    dripset_params.drip_volume_left = 0;
    memset(dripset_params.drip_msg, '\0', DRIPSET_MONITOR_STRING_LEN);
    
    //init buzzer
    pinMode(BUZZER_PIN, OUTPUT);
    SENSOR_STATES infrared_state = SENSOR_STATE_BUSY;

    do{
        infrared_state = infrared_Init(INFRARED_PIN);
    }
    while(infrared_state == SENSOR_STATE_BUSY);

    SENSOR_STATES loadcell_state = SENSOR_STATE_BUSY;
    do{
        loadcell_state = load_cell_Init(LOAD_CELL_SDA_PIN,LOAD_CELL_SCK_PIN);
    }
    while(loadcell_state == SENSOR_STATE_BUSY);

    if(infrared_state == SENSOR_STATE_ERROR || loadcell_state == SENSOR_STATE_ERROR)
    {
        setDripState(DRIP_STATE_ERROR);
    }
    else
    {
        setDripState(DRIP_STATE_OFF);
    }

    motor_Init(MOTOR_PIN);

}

void loop()
{
    if((load_cell_SetVolume() == SENSOR_STATE_ERROR) || (infrared_SetRate() == SENSOR_STATE_ERROR))
    {
        #ifdef DEBUG_DEVICE_SATES
        Serial.println("Error in setting sensor values");
        #endif
        setDripState(DRIP_STATE_ERROR);
    }

    switch( dripset_state)
    {
        case DRIP_STATE_OFF:
        {
            motor_OpenFlow();
            dripset_params.drip_running = false;
            beepBuzzer(5);
            if(dripset_params.drip_flow_rate > 0)
            {
                setDripState(DRIP_STATE_DRIPPING);
            }
            break;
        }

        case DRIP_STATE_DRIPPING:
        {
            motor_OpenFlow();
            dripset_params.drip_running = true;
            if(dripset_params.drip_flow_rate == 0)
            {
                setDripState(DRIP_STATE_OFF);
            }
            else if((dripset_params.drip_flow_rate < dripset_params.minimum_flow_rate) || 
                    (dripset_params.drip_flow_rate > dripset_params.maximum_flow_rate) ||
                    (dripset_params.drip_volume_left <= dripset_params.cutoff_volume)
            )
            {
                setDripState(DRIP_STATE_STOPPED);
            }

            beepBuzzer(3);
            break;
        }

        case DRIP_STATE_STOPPED:
        {
            motor_CloseFlow();
            dripset_params.drip_running = false;
            beepBuzzer(4);
            break;
        }

        case DRIP_STATE_ERROR:
        {
            motor_CloseFlow();
            dripset_params.drip_running = false;
            if((load_cell_SetVolume() != SENSOR_STATE_ERROR) && (infrared_SetRate() != SENSOR_STATE_ERROR))
            {
                setDripState(DRIP_STATE_OFF);
            }
            beepBuzzer(2);
            break;
        }
    }
}

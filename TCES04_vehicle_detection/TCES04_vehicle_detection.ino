#include "vehicle_detection_config.h"

uint8_t Broken_vehicle_sensor[VEHICLE_TOTAL_IR_SENSORS] = 
{
    VEHICLE_SENSOR_IR1,
    VEHICLE_SENSOR_IR2
};

void beepBuzzer(uint8_t buzzer_rate_500ms)
{
    static unsigned long buzzer_millis = 0;
    if(millis() - buzzer_millis > buzzer_rate_500ms*500)
    {
        // tone(BUZZER, 4000, 150);
        analogWrite(VEHICLE_DETECTED_BUZZER, 180);
        delay(60);
        analogWrite(VEHICLE_DETECTED_BUZZER, 0);
        buzzer_millis = millis();
    }
}

void handleError( void )
{
    while(1)
    {
        beepBuzzer(1);
        delay(500);
    }
}

void setup()
{
    pinMode( VEHICLE_DETECTED_BUZZER, OUTPUT);
    if( !Disp_Init())
    {
        handleError();
    }
    delay(1000);

    Incoming_Init();
    delay(1000);
    
    if(Stationary_Init(Broken_vehicle_sensor) == false)
    {
        Disp_SetMessage("Sensor Error!!", 2);
        Disp_Update();
        handleError();
    }
}


void loop()
{
    uint8_t moving_vehicle = Incoming_GetVehicleDistance();
    uint16_t broken_vehicle = Stationary_GetVehicleDistance();

    if(broken_vehicle > 0)
    {
        //sensed broken vehicle
        uint16_t distance_apart = moving_vehicle + broken_vehicle;
        char alert_msg[17] ={0};
        memset(alert_msg, '\0', 17);

        Disp_SetMessage("Broken vehicle!", 1);
        if(distance_apart == broken_vehicle)
        {
            sprintf(alert_msg, "At: %uM point", distance_apart);
            beepBuzzer(4);
        }
        else
        {
            //incoming vehicle
            sprintf(alert_msg, "%uM ahead", distance_apart);
            beepBuzzer(2);
        }

        Disp_SetMessage(alert_msg, 1);
    }
    else
    {
        //no broken vehicle
        Disp_SetMessage("No vehicles", 1);
        Disp_SetMessage("Drive Safely", 2);
    }

    Disp_Update();
}
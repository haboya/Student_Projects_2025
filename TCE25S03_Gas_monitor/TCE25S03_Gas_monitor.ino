
#include "gas_monitor_config.h"
#include <Wire.h> 

#define DEBUG_DEVICE_STATES
DEVICE_PARAMS device_params;
DEVICE_STATES curr_device_state;

void beepBuzzer( uint8_t buzzer_rate_500ms)
{
    static unsigned long buzzer_millis = 0;
    if(millis() - buzzer_millis > buzzer_rate_500ms*500)
    {
        tone(BUZZER_PIN, 4000, 150);
        buzzer_millis = millis();
    }
}

bool device_setup()
{
    lcd_Init();
    button_Init(BUTTON_PIN);
    gas_sensor_Init(GAS_SENSOR_PIN);

    motor_Init(MOTOR_PIN);
    delay(1000);
    if(load_cell_Init(LOAD_CELL_SDA_PIN, LOAD_CELL_SCK_PIN) == SENSOR_STATE_ERROR)
    {
            lcd_setRowMsg(2, "load cell error");
            return false;
    }
  
    lcd_setRowMsg(2, "load cell ready");
    return true;
}

bool set_DeviceState(DEVICE_STATES new_state)
{
    if(curr_device_state == new_state) return false;

    #ifdef DEBUG_DEVICE_STATES
        Serial.print("Device State Changed from: ");
        Serial.print(curr_device_state);
        Serial.print(" to: ");
        Serial.println(new_state);
    #endif
    curr_device_state = new_state;
    return true;
}

void setup()
{
    set_DeviceState(DEVICE_INIT);
    Serial.begin(115200);
    
    pinMode(BUZZER_PIN, OUTPUT);

    beepBuzzer(1);
    bool device_ready = device_setup();
    beepBuzzer(1);
    lcd_Update();
    
    if(!device_ready)
    {
        #ifdef DEBUG_DEVICE_STATES
            Serial.println("Device Error");
        #endif
        set_DeviceState(DEVICE_ERROR);
    }

    iot_Init();
}

void loop(){

    switch (load_cell_SetVolume()) 
    {
        case SENSOR_STATE_READY:
        {
            switch(device_params.current_weight)
            {
                case TOP_FULL:
                {
                    lcd_setRowMsg(1, "GAS CAN FULL");
                    set_DeviceState(NO_GAS_DETECTED);
                    break;
                }

                case GETTING_FULL:
                {
                    lcd_setRowMsg(1, "GAS ALMOST FULL");
                    set_DeviceState(NO_GAS_DETECTED);
                    break;
                }

                case HALF_FULL:
                {
                    lcd_setRowMsg(1, "GAS HALF FULL");
                    set_DeviceState(NO_GAS_DETECTED);
                    break;
                }

                case RUNNING_OUT:
                {
                    lcd_setRowMsg(1, "GAS RUNNING LOW");
                    set_DeviceState(GAS_WEIGHT_LOW);
                    break;
                }

                case EMPTY_CAN:
                {
                    lcd_setRowMsg(1, "GAS CAN EMPTY");
                    set_DeviceState(GAS_WEIGHT_LOW);
                    break;
                }

                case NO_CAN:
                {
                    lcd_setRowMsg(1, "PLACE GAS CAN");
                    set_DeviceState(GAS_WEIGHT_LOW);
                    break;
                }
            }

            break;
        }

        case SENSOR_STATE_BUSY:
        {
            break;
        }

        case SENSOR_STATE_ERROR:
        {
            #ifdef DEBUG_DEVICE_STATES
                Serial.println("Sensor Error");
            #endif
            lcd_setRowMsg(1, "SENSOR ERROR!!!");
            set_DeviceState(DEVICE_ERROR);
            break;
        }
    }

    switch(gas_sensor_GetState())
    {
        case SENSOR_STATE_READY:
        {
            lcd_setRowMsg(2, "NO GAS DETECTED");
            break;
        }

        case SENSOR_STATE_BUSY:
        {
            if(curr_device_state == NO_GAS_DETECTED)
            {
                set_DeviceState(DEVICE_INIT);
            }
            lcd_setRowMsg(2, "SENSOR INIT...");
            break;
        }
        
        case SENSOR_STATE_ERROR:
        {
            set_DeviceState(GAS_DETECTED);
            lcd_setRowMsg(1, " GAS DETECTED!! ");
            break;
        }
    }
    
    lcd_Update();

    switch(curr_device_state)
    {
        //
        case DEVICE_INIT:
        case NO_GAS_DETECTED:
            //check BUTTON_PIN
            if(button_GetState())
            {
                #ifdef DEBUG_DEVICE_STATES
                    Serial.println("Button Pressed");
                #endif
                if(device_params.motor_status)
                {
                    #ifdef DEBUG_DEVICE_STATES
                        Serial.println("Opening Flow");
                    #endif
                    motor_OpenFlow();
                }
                else
                {
                    #ifdef DEBUG_DEVICE_STATES
                        Serial.println("Closing Flow");
                    #endif
                    motor_CloseFlow();
                }
            }
        break;

        case GAS_DETECTED:
            beepBuzzer(1);
            motor_CloseFlow();
        break;

        case GAS_WEIGHT_LOW:
            beepBuzzer(3);
            motor_CloseFlow();
        break;

        case DEVICE_ERROR:
            beepBuzzer(2);
            motor_CloseFlow();
        break;
    }
    
    iot_UpdateStatus();

    delay(10);
}
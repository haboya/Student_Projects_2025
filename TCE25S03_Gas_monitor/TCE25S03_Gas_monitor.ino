
#include "gas_monitor_config.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>


#define MOTOR_PIN             D6
#define gas_sensor_pin              A0
#define button                      D5
#define buzzer_pin                  D7

DEVICE_PARAMS device_params;
DEVICE_STATES curr_device_state;
LiquidCrystal_I2C lcd(0x27,16,2);
char lcd_row1[17] = {0};
char lcd_row2[17] = {0};
unsigned long disp_millis;
bool lcd_setRowMsg( uint8_t row_num, char *row_msg)
{
    if(strlen(row_msg) > 16)
    {
        return false;
    }
    if(row_num == 1)
    {
        memset(lcd_row1, '\0', 17);
        sprintf(lcd_row1, "%s", row_msg);
        return true;
    }
    else if(row_num == 2)
    {
        memset(lcd_row2, '\0', 17);
        sprintf(lcd_row2, "%s", row_msg);
        return true;
    }
    
    return false;
}

void beepBuzzer( uint8_t buzzer_rate_500ms)
{
    static unsigned long buzzer_millis = 0;
    if(millis() - buzzer_millis > buzzer_rate_500ms*500)
    {
        tone(buzzer_pin, 4000, 150);
        buzzer_millis = millis();
    }
}

bool device_setup()
{
  pinMode(gas_sensor_pin, INPUT);
  pinMode(button, INPUT);
  pinMode(buzzer_pin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("INNITIALIZING...");

  motor_Init(MOTOR_PIN);
  delay(1000);
  lcd.setCursor(0, 1);
  if(load_cell_Init(LOAD_CELL_SDA_PIN, LOAD_CELL_SCK_PIN) == SENSOR_STATE_ERROR){
    lcd.print("load cell error");
    return false;
  }
  
  lcd.print("load cell ready");
  return true;
}

void setup()
{
    curr_device_state = DEVICE_INIT;
    Serial.begin(115200);
    bool device_ready = device_setup();
    beepBuzzer(1);
    
    if(!device_ready)
    {
        curr_device_state = DEVICE_ERROR;
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
                    curr_device_state = GAS_WEIGHT_LOW;
                    break;
                }

                case GETTING_FULL:
                {
                    lcd_setRowMsg(1, "GAS ALMOST FULL");
                    curr_device_state = GAS_WEIGHT_LOW;
                    break;
                }

                case HALF_FULL:
                {
                    lcd_setRowMsg(1, "GAS HALF FULL");
                    curr_device_state = NO_GAS_DETECTED;
                    break;
                }

                case RUNNING_OUT:
                {
                    lcd_setRowMsg(1, "GAS RUNNING LOW");
                    curr_device_state = GAS_WEIGHT_LOW;
                    break;
                }

                case EMPTY_CAN:
                {
                    lcd_setRowMsg(1, "GAS CAN EMPTY");
                    curr_device_state = GAS_WEIGHT_LOW;
                    break;
                }

                case NO_CAN:
                {
                    lcd_setRowMsg(1, "PLACE GAS CAN");
                    curr_device_state = GAS_WEIGHT_LOW;
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
            curr_device_state = DEVICE_ERROR;
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
                curr_device_state = DEVICE_INIT;
            }
            lcd_setRowMsg(2, "SENSOR INIT...");
            break;
        }
        case SENSOR_STATE_ERROR:
        {
            curr_device_state = GAS_DETECTED;
            lcd_setRowMsg(1, " GAS DETECTED!! ");
            break;
        }
    }
    
    if(millis() - disp_millis > 800)
    {
        disp_millis = millis();
        lcd.clear();
        lcd.print(lcd_row1);
        lcd.setCursor(0,1);
        lcd.print(lcd_row2);
    }

    switch(curr_device_state)
    {
        //
        case DEVICE_INIT:
        case NO_GAS_DETECTED:
            //check button
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
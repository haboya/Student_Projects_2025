
#include "gas_monitor_config.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define LOAD_CELL_SCK_PIN           D3
#define LOAD_CELL_SDA_PIN           D4
#define servo_motor_pin             D6
#define gas_sensor_pin              A0
#define button                      D5
#define buzzer_pin                  D7

extern DEVICE_STATES curr_device_state; 
LiquidCrystal_I2C lcd(0x27,16,2);

bool device_setup(){
  pinMode(gas_sensor_pin, INPUT);
  pinMode(button, INPUT);
  pinMode(buzzer_pin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("INNITIALIZING....");

  delay(200);

  if(load_cell_Init(LOAD_CELL_SDA_PIN, LOAD_CELL_SCK_PIN)){
    Serial.println("load cell innitialised");
  }
  else{
    Serial.println("load cell error");
  }

  motor_Init(servo_motor_pin);
  delay(200);

  return true;
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

void setup(){
  Serial.begin(115200);
  if(device_setup()){
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("GAS MONITOR");
     lcd.setCursor(0,1);
     lcd.print("READY!!!");
  }

  curr_device_state = INIT;
}

void loop(){

    lcd.clear();
    lcd.backlight();
    device_params.sensor_value = analogRead(gas_sensor_pin);

    switch(curr_device_state){
      case INIT:
        // lcd.setCursor(0,0);
        // lcd.print("CHECKING GAS");
        // lcd.setCursor(0,1);
        // lcd.print("KNOB.....");
        // watch = read_gas_hose_status();
        // delay(1000);

        /*if(!watch){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("KNOB OPEN! IF IN");
          lcd.setCursor(0,1);
          lcd.print("USE PRESS BUTTON");

          uint8_t button_state = digitalRead(button);
          if(button_state == 1){
            curr_device_state = WAITING;
          }
          else{
            curr_device_state = GAS_DETECTED;
          }
        }*/
        // else{
          // lcd.clear();
          // lcd.setCursor(0,0);
          // lcd.print("KNOB CLOSED");
          lcd.setCursor(0,1);
          lcd.print("CHECKING SENSOR");
          delay(500);
          
          if(device_params.sensor_value < threshold - 1){
            curr_device_state = NO_GAS_DETECTED;
          }
          else if(device_params.sensor_value >= threshold){
            curr_device_state = GAS_DETECTED;
          }
          else{
            curr_device_state = ERROR;
          }
        // }
      break;

      case NO_GAS_DETECTED:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("NO GAS DETECTED");
        curr_device_state = INIT;
      break;

      case GAS_DETECTED:
        beepBuzzer(1);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("GAS DETECTED");
        watch = read_gas_hose_status();
        delay(1000);

        if(!watch){
          lcd.setCursor(0,1);
          lcd.print("CLOSING GAS KNOB");
          motor_CloseFlow();
          delay(1000);
          
          if(device_params.sensor_value < threshold - 1){
            curr_device_state = NO_GAS_DETECTED;
          }
          else if(device_params.sensor_value >= threshold){
            curr_device_state = GAS_DETECTED;
          }
          else{
            curr_device_state = ERROR;
          }
        }
        else{
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("GAS KNOB CLOSED!");
          lcd.setCursor(0,1);
          lcd.print("THERE'S A LEAK!");

          if(device_params.sensor_value < threshold - 1){
            curr_device_state = NO_GAS_DETECTED;
          }
          else if(device_params.sensor_value >= threshold){
            curr_device_state = GAS_DETECTED;
          }
          else{
            curr_device_state = ERROR;
          }
        }
      break;

      case WAITING:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("DONE COOKING???");
        lcd.setCursor(0,1);
        lcd.print("PRESS BUTTON...");
        if(digitalRead(button) == 1){
          curr_device_state = INIT;
        }
      break;

      case ERROR:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("SYSTEM ERROR!!!");
        lcd.setCursor(0,1);
        lcd.print("CHECK SYSTEM!!!");
        while(1);
    }

    delay(1000);
}
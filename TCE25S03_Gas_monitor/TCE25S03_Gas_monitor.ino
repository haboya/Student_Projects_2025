
#include "gas_monitor_config.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define LOAD_CELL_SCK_PIN           D3
#define LOAD_CELL_SDA_PIN           D4
#define servo_motor_pin             D6
#define gas_sensor_pin              A0

LiquidCrystal_I2C lcd(0x27,16,2); 

void setup(){
  Serial.begin(115200);
  if(device_setup()){
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("INNITIALIZATION");
     lcd.setCursor(0,1);
     lcd.print("COMPLETE!!!");
  }
}

void loop(){
    SENSOR_STATE load_cell_SetVolume();
    device_params.sensor_value = analogRead(gas_sensor_pin);
    if (device_params.sensor_value > threshold) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("LEAK DETECTED");
      lcd.setCursor(0,1);
      lcd.print("CLOSING VALVE");
      motor_CloseFlow();
    }
    delay(100);
}

bool device_setup(){
  pinMode(gas_sensor_pin, INPUT);

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
  return true;
}


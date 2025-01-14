
#include "gas_monitor_config.h"

#define LOAD_CELL_SCK_PIN           D3
#define LOAD_CELL_SDA_PIN           D4
#define motor_pin                   D6
#define gas_sensor_pin              A0


void setup(){
  Serial.begin(115200);
  if(device_setup()){
     display_clear();
     lcd.setCursor(0,0);
     lcd.println(device_params.message);
  }
}

void loop(){
    SENSOR_STATE load_cell_SetVolume();
    device_params.sensor_value = analogRead(analogPin);
    if (device_params.sensor_value > threshold) {
      display_clear();
      lcd.setCursor(0,0);
      lcd.print("LEAK DETECTED");
      motor_CloseFlow();
    }
    delay(100);
}

bool device_setup(){
  pinMode(gas_sensor_pin, INPUT);
  display_setup();
  return true;
}


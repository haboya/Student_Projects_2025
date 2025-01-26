#include "gas_monitor_config.h"

uint8_t button_pin;
unsigned long button_millis = 0;

void button_Init(uint8_t btn_pin)
{
    button_pin = btn_pin;
    pinMode(button_pin, INPUT);
}

bool button_GetState()
{
    if(millis() - button_millis < 500)
    {
        return false;
    }

    if(digitalRead(button_pin) == HIGH)
    {
        button_millis = millis();
        return true;
    }

    return false;
}
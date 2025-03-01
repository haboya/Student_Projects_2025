#include "arduino_sensor_config.h"

typedef enum READER_PROCESS
{
    READER_PROCESS_READ,
    READER_PROCESS_WAIT,
    READER_PROCESS_CASE1,
    READER_PROCESS_CASE2,
    READER_PROCESS_CASE3,
    READER_PROCESS_DONE
}
READER_PROCESS;

typedef enum FAULT_PROCESS
{
    FAULT_PROCESS_INIT,
    FAULT_PROCESS_SPIKE,
    FAULT_PROCESS_DROP
}
FAULT_PROCESS;

SENSOR_CURRENTS sensor_currents;
SENSOR_CURRENTS output_currents;
SENSOR_CURRENTS temp_currents;
FAULT_PROCESS fault_process = FAULT_PROCESS_INIT;
READER_PROCESS fault_sensor = READER_PROCESS_READ;
READER_PROCESS reader_process = READER_PROCESS_READ;

unsigned long last_time = 0;
unsigned long current_time = 0;
uint32_t wait_time = 0;

void Process_SendCurrents( void )
{
    Serial.print(output_currents.bus87); Serial.print(",");
    Serial.print(output_currents.bus14); Serial.print(",");
    Serial.print(output_currents.bus16); Serial.print(",");
    Serial.print(output_currents.bus22); Serial.print(",");
    Serial.print(output_currents.bus24); Serial.print("\n");
}

bool Process_HandleCase1( void )
{
    output_currents.bus87 = sensor_currents.bus87;
    return true;
}

bool Process_HandleCase2( void )
{
    output_currents.bus14 = sensor_currents.bus14;
    return true;
}

bool Process_HandleCase3( void )
{
    output_currents.bus16 = sensor_currents.bus16;
    return true;
}

float getRandomVal( void )
{
    return (float)(random(0, 10)*1.0) / 10.0;
}

void setup()
{
    Serial.begin(9600);
    if(!Sensors_Init())
    {
        Serial.println("Sensor initialization failed");
        while(1);
    }

    last_time = millis();
    current_time = millis();
    wait_time = 1000;
}

void loop()
{
    Sensors_Read();
    current_time = millis();
    switch(reader_process)
    {
        case READER_PROCESS_READ:
        {
            if(sensor_currents.bus14 > SENSOR_OVERLOAD)
            {
                fault_sensor = READER_PROCESS_CASE1;
                reader_process = READER_PROCESS_WAIT;
                temp_currents = sensor_currents;
            }
            else if(sensor_currents.bus22 > SENSOR_OVERLOAD)
            {
                fault_sensor = READER_PROCESS_CASE2;
                reader_process = READER_PROCESS_WAIT;
                temp_currents = sensor_currents;
            }
            else if(sensor_currents.bus24 > SENSOR_OVERLOAD)
            {
                fault_sensor = READER_PROCESS_CASE3;
                reader_process = READER_PROCESS_WAIT;
                temp_currents = sensor_currents;
            }
            
            wait_time = 1000;
            if(current_time - last_time > wait_time)
            {
                output_currents = sensor_currents;
                Process_SendCurrents();
                last_time = current_time;
            }
        }
        break;

        case READER_PROCESS_WAIT:
        {
            if(sensor_currents.bus87 <= SENSOR_UNDERLOAD)
            {
                reader_process = fault_sensor;
                fault_process = FAULT_PROCESS_INIT;
            }

            wait_time = 1000;
            if(current_time - last_time < wait_time)
            {
                output_currents.bus87  = temp_currents.bus87 + getRandomVal();
                output_currents.bus14  = temp_currents.bus14 + getRandomVal();
                output_currents.bus16  = temp_currents.bus16 + getRandomVal();
                output_currents.bus22  = temp_currents.bus22 + getRandomVal();
                output_currents.bus24  = temp_currents.bus24 + getRandomVal();
                Process_SendCurrents();
                last_time = current_time;
            }
            reader_process = fault_sensor; //wait for lines to fall and move to exact case
        }
        break;

        case READER_PROCESS_CASE1:
        {
            output_currents.bus87 = sensor_currents.bus87;
            switch(fault_process)
            {
                case FAULT_PROCESS_INIT:
                {
                    if(sensor_currents.bus14 > SENSOR_OVERLOAD)
                    {
                        fault_process = FAULT_PROCESS_SPIKE;
                    }
                }
                break;

                case FAULT_PROCESS_SPIKE:
                {
                    if(sensor_currents.bus14 < SENSOR_UNDERLOAD)
                    {
                        fault_process = FAULT_PROCESS_DROP;
                    }
                }
                break;

                case FAULT_PROCESS_DROP:
                {
                    if(sensor_currents.bus14 > SENSOR_OVERLOAD)
                    {
                        fault_process = FAULT_PROCESS_INIT;
                    }
                }
                break;
            }
        }
        break;

        case READER_PROCESS_CASE2:
        {
            output_currents.bus14 = sensor_currents.bus14;
            reader_process = READER_PROCESS_CASE3;
        }
        break;

        case READER_PROCESS_CASE3:
        {
            output_currents.bus16 = sensor_currents.bus16;
            reader_process = READER_PROCESS_DONE;
        }
        break;

        case READER_PROCESS_DONE:
        {
            output_currents.bus22 = sensor_currents.bus22;
            output_currents.bus24 = sensor_currents.bus24;
            reader_process = READER_PROCESS_READ;
        }
        break;
    }
}

/* ----------------------------------- EOF ---------------------------------- */
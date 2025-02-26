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

SENSOR_CURRENTS sensor_currents;
SENSOR_CURRENTS output_currents;
READER_PROCESS fault_process = READER_PROCESS_READ;
READER_PROCESS reader_process = READER_PROCESS_READ;

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

void setup()
{
    Serial.begin(9600);
    if(!Sensors_Init())
    {
        Serial.println("Sensor initialization failed");
        while(1);
    }
}

void loop()
{
    Sensors_Read();
    switch(reader_process)
    {
        case READER_PROCESS_READ:
        {
            output_currents = sensor_currents;
            fault_process = READER_PROCESS_CASE1;
            reader_process = READER_PROCESS_READ;
        }
        break;

        case READER_PROCESS_WAIT:
        {
            reader_process = fault_process; //wait for lines to fall and move to exact case
        }
        break;

        case READER_PROCESS_CASE1:
        {
            output_currents.bus87 = sensor_currents.bus87;
            reader_process = READER_PROCESS_CASE2;
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
            reader_process = READER_PROCESS_INIT;
        }
        break;
    }
}

/* ----------------------------------- EOF ---------------------------------- */
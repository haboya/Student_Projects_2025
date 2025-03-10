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
READER_PROCESS fault_sensor = READER_PROCESS_READ;
READER_PROCESS reader_process = READER_PROCESS_READ;

unsigned long last_time = 0;
unsigned long current_time = 0;
uint32_t wait_time = 0;

void Process_SendCurrents( void )
{
    #ifndef DEBUG_SENSOR_SYSTEM
        Serial.print(output_currents.bus87); Serial.print(",");
        Serial.print(output_currents.bus14); Serial.print(",");
        Serial.print(output_currents.bus16); Serial.print(",");
        Serial.print(output_currents.bus22); Serial.print(",");
        Serial.print(output_currents.bus24); Serial.print("\n");
    #endif
}

bool Process_HandleCase(READER_PROCESS curr_fault )
{
    FAULT_PROCESS fault_process = FAULT_PROCESS_INIT;

    while(1)
    {
        output_currents.bus87 = sensor_currents.bus87;
        switch(fault_process)
        {
            case FAULT_PROCESS_INIT:
            {
                Process_SendCurrents();
                delay(8);
                output_currents.bus87 = 250 + getRandomVal();
                fault_process = FAULT_PROCESS_SPIKE;
                Process_SendCurrents();
                delay(6);
            }
            break;
    
            case FAULT_PROCESS_SPIKE:
            {
                output_currents.bus87 = 552 + getRandomVal();
                fault_process = FAULT_PROCESS_DROP;
                Process_SendCurrents();
                delay(4);
            }
            break;
    
            case FAULT_PROCESS_DROP:
            {
                output_currents.bus87 = 0;
                output_currents.bus14 = 0;
                output_currents.bus16 = 0;
                output_currents.bus22 = 0;
                output_currents.bus24 = 0;
                // fault_process = FAULT_PROCESS_INIT;
                // reader_process = READER_PROCESS_READ;
                delay(200);
                Process_SendCurrents();
            }
            break;
        }
        

    }
    return true;
}

float getRandomVal( void )
{
    return (float)(random(0, 10)*1.0) / 3.0;
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
                #ifdef DEBUG_SENSOR_SYSTEM
                    Serial.println("Entering Case 1");
                #endif
            }
            else if(sensor_currents.bus22 > SENSOR_OVERLOAD/2)
            {
                fault_sensor = READER_PROCESS_CASE2;
                reader_process = READER_PROCESS_WAIT;
                #ifdef DEBUG_SENSOR_SYSTEM
                    Serial.println("Entering Case 2");
                #endif
            }
            else if(sensor_currents.bus24 > SENSOR_OVERLOAD)
            {
                fault_sensor = READER_PROCESS_CASE3;
                reader_process = READER_PROCESS_WAIT;
                #ifdef DEBUG_SENSOR_SYSTEM
                    Serial.println("Entering Case 3");
                #endif
            }
            
            wait_time = 500;
            if(current_time - last_time > wait_time)
            {
                output_currents = sensor_currents;
                if(sensor_currents.bus87 > 0.01) output_currents.bus87  = 72.0 + getRandomVal();
                if(sensor_currents.bus14 > 0.01) output_currents.bus14  = 44.0 + getRandomVal();
                if(sensor_currents.bus16 > 0.01) output_currents.bus16  = 35.0 + getRandomVal();
                if(sensor_currents.bus22 > 0.01) output_currents.bus22  = 22.0 + getRandomVal();
                if(sensor_currents.bus24 > 0.01) output_currents.bus24  = 15.0 + getRandomVal();
                temp_currents = output_currents;;
                Process_SendCurrents();
                last_time = current_time;
            }
        }
        break;

        case READER_PROCESS_WAIT:
        {
            if(sensor_currents.bus87 <= SENSOR_UNDERLOAD)
            {
                #ifdef DEBUG_SENSOR_SYSTEM
                    Serial.print("Entering Next ");
                    Serial.println(fault_sensor);
                #endif
                Process_HandleCase(fault_sensor);
                // reader_process = fault_sensor;
                reader_process = READER_PROCESS_DONE;
            }

            wait_time = 500;
            if(current_time - last_time > wait_time)
            {
                if(sensor_currents.bus87 > 0.01) output_currents.bus87  = 72.0 + getRandomVal();
                if(sensor_currents.bus14 > 0.01) output_currents.bus14  = 44.0 + getRandomVal();
                if(sensor_currents.bus16 > 0.01) output_currents.bus16  = 35.0 + getRandomVal();
                if(sensor_currents.bus22 > 0.01) output_currents.bus22  = 22.0 + getRandomVal();
                if(sensor_currents.bus24 > 0.01) output_currents.bus24  = 15.0 + getRandomVal();
                Process_SendCurrents();
                last_time = current_time;
            }
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
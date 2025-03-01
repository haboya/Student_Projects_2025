
#ifndef ARDUINO_SENSOR_CONFIG_H
    #define ARDUINO_SENSOR_CONFIG_H

    #define DEBUG_SENSOR_SYSTEM
    #define CALIBRATION_THRESHOLD   1.35
    #define SENSOR_OVERLOAD         7.1
    #define SENSOR_UNDERLOAD        0.0
    #define SENSOR_SAMPLES          20
    #define SENSOR_COUNT            5
    #define SENSOR_BUS87_PIN        A1
    #define SENSOR_BUS14_PIN        A0
    #define SENSOR_BUS16_PIN        A2
    #define SENSOR_BUS22_PIN        A3
    #define SENSOR_BUS24_PIN        A4

    typedef struct CURRENT_SESNOR {
        uint8_t ana_pin;
        uint8_t val_index;
        int max_val[SENSOR_SAMPLES];
        int old_val;
        float IRMS;
    }
    CURRENT_SESNOR;
    typedef struct SENSOR_CURRENTS
    {
        float bus87;
        float bus14;
        float bus16;
        float bus22;
        float bus24;
    }
    SENSOR_CURRENTS;

    extern SENSOR_CURRENTS sensor_currents;

    bool Sensors_Init( void );
    void Sensors_Read( void );

#endif

#ifndef VEHICLE_DETECTION_CONFIG_H
    #define VEHICLE_DETECTION_CONFIG_H

    // #define DEBUG_SYSTEM
    #define DISPLAY_UPDATE_INTERVAL     800
    #define VEHICLE_SENSOR_SAMPLES      5
    #define VEHICLE_TOTAL_IR_SENSORS    2
    #define VEHICLE_SENSOR_IR1          2
    #define VEHICLE_SENSOR_IR2          3
    #define VEHICLE_SENSOR_TRIG         4
    #define VEHICLE_SENSOR_ECHO         5
    #define VEHICLE_MAXIMUM_DISTANCE    50
    #define VEHICLE_DISTANCE_TO_SENSORS 10
    #define VEHICLE_DISTANCE_IN_SENSORS 5
    #define VEHICLE_DETECTED_BUZZER     6

    bool Incoming_Init( void );
    uint8_t Incoming_GetVehicleDistance( void );

    bool Stationary_Init( uint8_t sensor_pins[] );
    uint16_t Stationary_GetVehicleDistance( void );

    bool Disp_Init( void );
    bool Disp_Update( void );
    bool Disp_SetMessage( const char *msg, uint8_t line );
#endif

#ifndef VEHICLE_DETECTION_CONFIG_H
    #define VEHICLE_DETECTION_CONFIG_H

    bool Incoming_Init( void );
    uint8_t Incoming_GetVehicleDistance( void );

    bool Stationary_Init( void );
    uint8_t Stationary_GetVehicleDistance( void );

    bool Disp_Init( void );
    bool Disp_Update( void );
    bool Disp_SetMessage( const char *msg, uint8_t line );
#endif

#ifndef DRIPSET_MONITOR_H
    #define DRIPSET_MONITOR_H

    #define BLYNK_TEMPLATE_ID "TMPL2C2GdsF4U"
    #define BLYNK_TEMPLATE_NAME "DRIP MONITOR"
    #define BLYNK_AUTH_TOKEN "yokyjZh4Ol7eLta3Uonxbu2VpMRV91Wf"
    
    #define DRIPSET_MONITOR_STRING_LEN  32
    #define DRIPSET_IOT_UPDATE_INTERVAL 1

    typedef enum DRIPSET_STATE
    {
        /// @brief when still getting stuff ready
        DRIP_STATE_INIT,

        /// @brief when dripping stopped by the user
        DRIP_STATE_OFF,

        /// @brief When running normally and the rate is within the limits
        DRIP_STATE_DRIPPING,

        /// @brief when stopped by the system due to set limits in the parameters
        DRIP_STATE_STOPPED,

        /// @brief when there is error state, could be sensor failure or out of range parameters
        DRIP_STATE_ERROR
    }
    DRIPSET_STATE;

    typedef enum SENSOR_STATE
    {
        SENSOR_STATE_ERROR,
        SENSOR_STATE_BUSY,
        SENSOR_STATE_READY
    }
    SENSOR_STATE;

    typedef struct DRIPSET_PARAMS
    {
        /// @brief false if the dripset has been stopped
        bool drip_running;

        /// @brief the current flow rate of the dripset in drps per minute
        uint8_t drip_flow_rate;

        /// @brief the currently remaining quantity in milliliters of the fliud left in the bottle
        uint8_t drip_volume_left;

        /// @brief any message worthy to communicate to the user, it can even be an error message
        char drip_msg[DRIPSET_MONITOR_STRING_LEN];
    }
    DRIPSET_PARAMS;

    extern DRIPSET_PARAMS dripset_params;

    /// @brief This function sets the current flow rate of the dripset
    /// @retval SENSOR_STATE_READY if the value is valid and set successfully
    /// @retval SENSOR_STATE_BUSY if there is pending conversion before the value can be set
    /// @retval SENSOR_STATE_ERROR if the required value can't be set for some reason
    SENSOR_STATE infrared_SetRate( void );

    /// @brief Initialize the infrared sensor used for determining flow rate 
    /// @param sensor_pin the pin number on which the sensor is connected
    /// @retval SENSOR_STATE_READY if the initilisation is complete
    /// @retval SENSOR_STATE_ERROR if the sensor can't be set up
    SENSOR_STATE infrared_Init( uint8_t sensor_pin );

    /// @brief Initialize the load cell sensor 
    /// @param sda_pin data pin of the load sensor
    /// @param sck_pin clock pin of the load sensor
    /// @retval SENSOR_STATE_READY if the initilisation is complete
    /// @retval SENSOR_STATE_ERROR if the sensor can't be set up
    /// @note This load sensor uses the hx711 drivers
    SENSOR_STATE load_cell_Init( uint8_t sda_pin, uint8_t sck_pin );

    /// @brief This function sets the remaining volume of the liquid in the dripset_params struct
    /// @retval SENSOR_STATE_READY if the value is valid and set successfully
    /// @retval SENSOR_STATE_BUSY if there is pending conversion before the value can be set
    /// @retval SENSOR_STATE_ERROR if the required value can't be set for some reason
    SENSOR_STATE load_cell_SetVolume( void );


    /// @brief Setup connection with the IOT platform 
    /// @return false if the connection fails
    bool iot_Init( void );

    /// @brief keep update with the IOT platform 
    /// @return true at every sucessive data upload
    bool iot_UpdateStatus( void );

    void motor_Init( void );
    bool motor_OpenFlow( void );
    bool motor_CloseFlow( void );

#endif
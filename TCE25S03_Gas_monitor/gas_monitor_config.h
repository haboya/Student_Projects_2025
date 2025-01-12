
#ifndef GAS_MONITOR_H
  #define GAS_MONITOR_H

  typedef enum SENSOR_STATE{
        SENSOR_STATE_ERROR,
        SENSOR_STATE_BUSY,
        SENSOR_STATE_READY
  } SENSOR_STATE;

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

#endif
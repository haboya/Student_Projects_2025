
#ifndef GAS_MONITOR_H
  #define GAS_MONITOR_H
  #define LOAD_CELL_SCK_PIN           D3
  #define LOAD_CELL_SDA_PIN           D4
  #define threshold 200

  typedef enum SENSOR_STATES{
        SENSOR_STATE_ERROR,
        SENSOR_STATE_BUSY,
        SENSOR_STATE_READY
  } SENSOR_STATES;

  typedef enum WEIGHT_SIZES{
      NO_CAN,
      EMPTY_CAN,
      RUNNING_OUT,
      HALF_FULL,
      GETTING_FULL,
      TOP_FULL
  }WEIGHT_SIZES;

  typedef enum DEVICE_STATES{
    DEVICE_INIT,
    NO_GAS_DETECTED,
    GAS_DETECTED,
    GAS_WEIGHT_LOW,
    DEVICE_ERROR
  }DEVICE_STATES;

  typedef struct DEVICE_PARAMS{
    int sensor_value;
    WEIGHT_SIZES current_weight;
  }DEVICE_PARAMS;

  bool watch;
  extern DEVICE_PARAMS device_params;

  SENSOR_STATES load_cell_Init(uint8_t sda_pin, uint8_t sck_pin);
  SENSOR_STATES load_cell_GetWeight(void);
  SENSOR_STATES gas_sensor_Init(uint8_t sensor_pin);

  /// @brief Check the gas state
  /// @retval SENSOR_STATE_ERROR When gas is detected
  /// @retval SENSOR_STATE_READY When no gas is detected
  /// @retval SENSOR_STATE_BUSY When still getting within range
  SENSOR_STATES gas_sensor_GetState(void);
  void motor_Init(uint8_t motor_pin);
  bool read_gas_hose_status( void );
  void motor_CloseFlow( void );

#endif
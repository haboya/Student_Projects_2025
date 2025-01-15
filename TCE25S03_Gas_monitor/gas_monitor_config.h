
#ifndef GAS_MONITOR_H
  #define GAS_MONITOR_H

  #define threshold 200

  typedef enum SENSOR_STATE{
        SENSOR_STATE_ERROR,
        SENSOR_STATE_BUSY,
        SENSOR_STATE_READY
  } DEVICE_STATE;

  typedef struct DEVICE_PARAMS{
    int sensor_value;
    float weight;
  }DEVICE_PARAMS;

  SENSOR_STATE sensor_state;
  DEVICE_PARAMS device_params;

  bool load_cell_Init(uint8_t sda_pin, uint8_t sck_pin);
  SENSOR_STATE load_cell_SetVolume(void);
  void motor_Init(uint8_t motor_pin);
  bool motor_OpenFlow(void);
  bool motor_CloseFlow(void);

#endif
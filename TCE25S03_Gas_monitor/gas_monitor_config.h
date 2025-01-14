
#ifndef GAS_MONITOR_H
  #define GAS_MONITOR_H

  #define strlen 16
  #define threshold = 400;

  typedef enum SENSOR_STATE{
        DEVICE_STATE_ERROR,
        DEVICE_STATE_BUSY,
        DEVICE_STATE_READY
  } DEVICE_STATE;

  typedef enum LCD_STATE{
        LCD_READY,
        LCD_ERROR
  }LCD_STATE;

  typedef struct DEVICE_PARAMS{
    char message[strlen];
    int sensor_value;
    float weight;
  }DEVICE_PARAMS;

  DEVICE_PARAMS device_params;

  SENSOR_STATE load_cell_Init(uint8_t LOAD_CELL_SDA_PIN, uint8_t LOAD_CELL_SCK_PIN);
  SENSOR_STATE load_cell_SetVolume( void );
  DEVICE_STATE display_setup();
  void motor_Init( void );
  bool motor_OpenFlow( void );
  bool motor_CloseFlow( void );
  void display_clear();

#endif
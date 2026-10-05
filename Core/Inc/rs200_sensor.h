#ifndef RS200_SENSOR_H
#define RS200_SENSOR_H

#include "main.h"

#define RS200_FRAME_SIZE 5
#define RS200_HEADER     0x3A

typedef struct {
    uint8_t rain_level;       // 0: No rain, 1: Light, 2: Moderate, 3: Heavy
    uint8_t system_status;    // System error codes (0 = Normal, 0x04 = Optic Fault)
    uint8_t is_raining;       // Active rain flag
} RS200_Data_t;

extern RS200_Data_t rain_sensor;
extern volatile uint8_t packet_ready_flag;
extern uint8_t rx_byte;
extern uint8_t rs200_rx_buf[RS200_FRAME_SIZE];

uint8_t xCal_crc(uint8_t *ptr, uint32_t len);
void Process_RS200_Packet(uint8_t *buffer);
void RS200_Send_Calibration(UART_HandleTypeDef *huart);

#endif

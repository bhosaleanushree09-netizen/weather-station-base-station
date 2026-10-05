/*
 * rs200_sensor.c
 *
 *  Created on: 05-Oct-2026
 *      Author: User
 */


#include "rs200_sensor.h"

RS200_Data_t rain_sensor = {0};
uint8_t rx_byte = 0;
uint8_t rs200_rx_buf[RS200_FRAME_SIZE];
uint8_t rx_index = 0;
volatile uint8_t packet_ready_flag = 0;
const uint8_t CMD_CALIBRATE[5] = {0x3A, 0x83, 0x00, 0x00, 0x54};

uint8_t xCal_crc(uint8_t *ptr, uint32_t len) {
    uint8_t crc = 0xFF;
    uint8_t i;
    while(len--) {
        crc ^= *ptr++;
        for(i = 0; i < 8; i++) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else crc = (crc << 1);
        }
    }
    return crc;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        if (rx_index == 0 && rx_byte != RS200_HEADER) {
            HAL_UART_Receive_IT(huart, &rx_byte, 1);
            return;
        }
        rs200_rx_buf[rx_index++] = rx_byte;
        if (rx_index >= RS200_FRAME_SIZE) {
            rx_index = 0;
            packet_ready_flag = 1;
        }
        HAL_UART_Receive_IT(huart, &rx_byte, 1);
    }
}

void Process_RS200_Packet(uint8_t *buffer) {
    if (xCal_crc(&buffer[1], 4) != 0) return;
    uint8_t frame_flag = buffer[1];
    uint8_t data_val   = buffer[2];

    switch (frame_flag) {
        case 0x81:
            rain_sensor.rain_level = data_val;
            rain_sensor.is_raining = (data_val > 0) ? 1 : 0;
            break;
        case 0x82:
            rain_sensor.system_status = data_val;
            break;
        case 0x83:
            break;
        default:
            break;
    }
}

void RS200_Send_Calibration(UART_HandleTypeDef *huart) {
    HAL_UART_Transmit(huart, (uint8_t *)CMD_CALIBRATE, RS200_FRAME_SIZE, 100);
}

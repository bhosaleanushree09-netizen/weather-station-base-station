#ifndef BME680_SENSOR_H
#define BME680_SENSOR_H

#include "main.h"
#include "bme68x.h"

extern struct bme68x_dev bme_dev;
extern struct bme68x_conf bme_conf;
extern struct bme68x_heatr_conf bme_heatr_conf;
extern struct bme68x_data bme_data;
extern uint8_t g_bme_sensor_ready;

void Init_BME680(I2C_HandleTypeDef *hi2c);

#endif

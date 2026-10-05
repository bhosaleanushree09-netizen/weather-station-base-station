#ifndef WIND_SENSOR_H
#define WIND_SENSOR_H

#include "main.h"

#define VREF 3.3f
#define ADC_MAX 4095.0f
#define VOLTAGE_DIVIDER_RATIO 1.6667f
#define MAX_WIND_SPEED 30.0f
#define WIND_ADC_OVERSAMPLE_COUNT 8

float Read_Wind_Speed(ADC_HandleTypeDef *hadc);

#endif
